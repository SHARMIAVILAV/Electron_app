#include "firstCpp.h"
// #include <opencv2/opencv.hpp>
// #include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>

std::string firstCpp::hello()
{
    return "Hello World";
}

Napi::String firstCpp::HelloWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::String returnValue = Napi::String::New(env, firstCpp::hello());

    return returnValue;
}



int firstCpp::add(int a, int b){
    return a+b;
}

Napi::Number firstCpp::AddWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

    Napi::Number first = info[0].As<Napi::Number>();
    Napi::Number second = info[1].As<Napi::Number>();

    int returnValue = firstCpp::add(first.Int32Value(), second.Int32Value());

    return Napi::Number::New(env, returnValue);
}


Napi::Buffer<uint8_t> firstCpp::ConvertToGrayscale(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Extract image data from JavaScript Buffer
    Napi::Buffer<uint8_t> inputBuffer = info[0].As<Napi::Buffer<uint8_t>>();
    uint8_t* inputData = inputBuffer.Data();
    size_t inputLength = inputBuffer.Length();

    // Use OpenCV to process the image (convert to grayscale)
    cv::Mat inputImage = cv::imdecode(cv::Mat(1, inputLength, CV_8U, inputData), cv::IMREAD_COLOR);
    cv::Mat grayscaleImage;
    cv::cvtColor(inputImage, grayscaleImage, cv::COLOR_BGR2GRAY);
    cv::Mat dst;
    cv::equalizeHist(grayscaleImage,dst);

    cv::imwrite("./output.jpg",dst);

    // Create a new Buffer to hold the grayscale image data
    Napi::Buffer<uint8_t> outputBuffer = Napi::Buffer<uint8_t>::NewOrCopy(env, grayscaleImage.data, grayscaleImage.total());

    // Copy the data to a new buffer (avoid using external buffer)
    Napi::Buffer<uint8_t> resultBuffer = Napi::Buffer<uint8_t>::Copy(env, outputBuffer.Data(), outputBuffer.Length());

    return resultBuffer;
}


Napi::Object firstCpp::ConvertToGrayscaleWrapped(const Napi::CallbackInfo& info) {
    Napi::Buffer<uint8_t> resultBuffer = firstCpp::ConvertToGrayscale(info);

    // Create a new Buffer and copy the data
    Napi::Buffer<uint8_t> resultCopy = Napi::Buffer<uint8_t>::Copy(info.Env(), resultBuffer.Data(), resultBuffer.Length());

    Napi::Object resultObject = Napi::Object::New(info.Env());
    resultObject.Set("data", resultCopy);
    resultObject.Set("length", Napi::Number::New(info.Env(), resultBuffer.Length()));

    return resultObject;
}
Napi::Buffer<uint8_t> firstCpp::BBHE(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Extract image data from JavaScript Buffer
    Napi::Buffer<uint8_t> inputBuffer = info[0].As<Napi::Buffer<uint8_t>>();
    uint8_t* inputData = inputBuffer.Data();
    size_t inputLength = inputBuffer.Length();

    // Use OpenCV to process the image (convert to BBHE)
    cv::Mat inputImage = cv::imdecode(cv::Mat(1, inputLength, CV_8U, inputData), cv::IMREAD_COLOR);
    cv::Mat grayscaleImage;
    cv::cvtColor(inputImage, grayscaleImage, cv::COLOR_BGR2GRAY);


    cv::Mat hist;
    int histSize = 256;  // number of bins
    float range[] = { 0, 256 };
    const float* histRange = { range };
    int c1=0;
    int c2=0;
    calcHist(&grayscaleImage, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
    // cout<<hist;
    // BBHE
    // Calculate mean of the histogram
    int bbhe_mean = cvRound(cv::mean(grayscaleImage)[0]);

    // // Divide the histogram on the basis of the mean into two parts
    cv::Mat H = hist.col(0);
    cv::Mat H1 = H.rowRange(0, bbhe_mean);
    cv::Mat H2 = H.rowRange(bbhe_mean, 256);

    // cout<<H1.size<<endl;
    // cout<<H2.size<<endl;

    H1 = H1 / cv::sum(H1)[0];
    H2 = H2 / cv::sum(H2)[0];



    // Calculate cumulative distribution functions (CDFs)
    cv::Mat C1 = cv::Mat::zeros(H1.size(), H1.type());  // Initialize C1 with zeros

    // Calculate CDF for H1
    for (int i = 0; i < bbhe_mean; ++i) {
        if (i == 0) {
            C1.at<float>(i) = H1.at<float>(i);
        } else {
            C1.at<float>(i) = C1.at<float>(i - 1) + H1.at<float>(i);
        }
        c1++;
    }

    // Now C1 contains the cumulative distribution function for the first part of the histogram

    // Similar process for H2
    // cout<<"c1 ="<< c1;
    cv::Mat C2 = cv::Mat::zeros(H2.size(), H2.type());  // Initialize C2 with zeros

    for (int i = 0; i < (255-bbhe_mean+1); ++i) {
        if (i == 0) {
            C2.at<float>(i) = H2.at<float>(i);
        } else {
            C2.at<float>(i) = C2.at<float>(i - 1) + H2.at<float>(i);
        }
        c2++;
    }
    // cout<<"c2 ="<< c2;
    // cout<<" "<<C1.at<float>(3)<<" ";

    cv::Mat C1n = cv::Mat::zeros(C1.size(), C1.type());  // Initialize C1n with zeros

    for (int i = 0; i < bbhe_mean; ++i) {

        C1n.at<float>(i) = static_cast<float>(bbhe_mean * C1.at<float>(i));
    }
    
    cv::Mat C2n = cv::Mat::zeros(C2.size(), C2.type());  // Initialize C2n with zeros

    for (int i = 0; i < (255-bbhe_mean+1); ++i) {
        C2n.at<float>(i) = static_cast<float>(bbhe_mean + (255 - bbhe_mean) * C2.at<float>(i));
    }


    cv::Mat img_sum1 = cv::Mat::zeros(C1n.size(), C1n.type());
    cv::vconcat(C1n, C2n, img_sum1);



    cv::Mat lut = cv::Mat::zeros(1, histSize, CV_8U);

    for (int i = 0; i < histSize; ++i) {
        lut.at<uchar>(i) = static_cast<uchar>(img_sum1.at<float>(i));
    }
    
    // Apply LUT for histogram equalization
    cv::Mat equalized_image;
    cv::LUT(grayscaleImage, lut, equalized_image);
    

    cv::imwrite("./BBHE.jpg",equalized_image);

    // Create a new Buffer to hold the grayscale image data
    Napi::Buffer<uint8_t> outputBuffer = Napi::Buffer<uint8_t>::NewOrCopy(env, grayscaleImage.data, grayscaleImage.total());

    // Copy the data to a new buffer (avoid using external buffer)
    Napi::Buffer<uint8_t> resultBuffer = Napi::Buffer<uint8_t>::Copy(env, outputBuffer.Data(), outputBuffer.Length());

    return resultBuffer;
}

Napi::Object firstCpp::BBHEWrapped(const Napi::CallbackInfo& info) {

    Napi::Buffer<uint8_t> resultBuffer = firstCpp::BBHE(info);

    // Create a new Buffer and copy the data
    Napi::Buffer<uint8_t> resultCopy = Napi::Buffer<uint8_t>::Copy(info.Env(), resultBuffer.Data(), resultBuffer.Length());

    Napi::Object resultObject = Napi::Object::New(info.Env());
    resultObject.Set("data", resultCopy);
    resultObject.Set("length", Napi::Number::New(info.Env(), resultBuffer.Length()));

    return resultObject;
}

Napi::Buffer<uint8_t> firstCpp::DSIHE(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Extract image data from JavaScript Buffer
    Napi::Buffer<uint8_t> inputBuffer = info[0].As<Napi::Buffer<uint8_t>>();
    uint8_t* inputData = inputBuffer.Data();
    size_t inputLength = inputBuffer.Length();

    // Use OpenCV to process the image (convert to BBHE)
    cv::Mat inputImage = cv::imdecode(cv::Mat(1, inputLength, CV_8U, inputData), cv::IMREAD_COLOR);
    cv::Mat grayscaleImage;
    cv::cvtColor(inputImage, grayscaleImage, cv::COLOR_BGR2GRAY);


    cv::Mat hist;
    int histSize = 256;  // number of bins
    float range[] = { 0, 256 };
    const float* histRange = { range };
    int c1=0;
    int c2=0;
    calcHist(&grayscaleImage, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
    // cout<<hist;

    cv::Mat cdf(hist.size(), hist.type(), cv::Scalar(0));
    float sum = 0;
    for (int i = 0; i < histSize; ++i)
    {
        sum += hist.at<float>(i);
        cdf.at<float>(i) = sum;
    }
    
    // Calculate median of the histogram
    int X = grayscaleImage.rows;
    int Y = grayscaleImage.cols;

    int lm =0;
    float minDiff = std::numeric_limits<float>::max();

    for (int i = 0; i < histSize; ++i) {
        float value = std::abs(hist.at<float>(i) - (X * Y / 2));

        if (value < minDiff) {
            minDiff = value;
            lm = i;
        }
    }

    cv::Mat H = hist.col(0);
    cv::Mat H1 = H.rowRange(0, lm);
    cv::Mat H2 = H.rowRange(lm, 256);

    // cout<<H1.size<<endl;
    // cout<<H2.size<<endl;

    // Divide histograms by their sums to get PDFs
    H1 = H1 / cv::sum(H1)[0];
    H2 = H2 / cv::sum(H2)[0];




    // Calculate cumulative distribution functions (CDFs)
    cv::Mat C1 = cv::Mat::zeros(H1.size(), H1.type());  // Initialize C1 with zeros

    // Calculate CDF for H1
    for (int i = 0; i < lm; ++i) {
        if (i == 0) {
            C1.at<float>(i) = H1.at<float>(i);
        } else {
            C1.at<float>(i) = C1.at<float>(i - 1) + H1.at<float>(i);
        }
        c1++;
    }

    // Now C1 contains the cumulative distribution function for the first part of the histogram

    // Similar process for H2

    cv::Mat C2 = cv::Mat::zeros(H2.size(), H2.type());  // Initialize C2 with zeros

    for (int i = 0; i < (255-lm+1); ++i) {
        if (i == 0) {
            C2.at<float>(i) = H2.at<float>(i);
        } else {
            C2.at<float>(i) = C2.at<float>(i - 1) + H2.at<float>(i);
        }
        c2++;
    }


    cv::Mat C1n = cv::Mat::zeros(C1.size(), C1.type());  // Initialize C1n with zeros

    for (int i = 0; i < lm; ++i) {

        C1n.at<float>(i) = static_cast<float>(lm * C1.at<float>(i));
    }
    
    cv::Mat C2n = cv::Mat::zeros(C2.size(), C2.type());  // Initialize C2n with zeros

    for (int i = 0; i < (255-lm+1); ++i) {
        C2n.at<float>(i) = static_cast<float>(lm + (255 - lm ) * C2.at<float>(i));
    }


    cv::Mat img_sum1 = cv::Mat::zeros(C1n.size(), C1n.type());
    cv::vconcat(C1n, C2n, img_sum1);




    cv::Mat lut = cv::Mat::zeros(1, histSize, CV_8U);

    for (int i = 0; i < histSize; ++i) {
        lut.at<uchar>(i) = static_cast<uchar>(img_sum1.at<float>(i));
    }

    // Apply LUT for histogram equalization
    cv::Mat equalized_image;
    cv::LUT(grayscaleImage, lut, equalized_image);


    cv::imwrite("./DSIHE.jpg",equalized_image);

    // Create a new Buffer to hold the grayscale image data
    Napi::Buffer<uint8_t> outputBuffer = Napi::Buffer<uint8_t>::NewOrCopy(env, grayscaleImage.data, grayscaleImage.total());

    // Copy the data to a new buffer (avoid using external buffer)
    Napi::Buffer<uint8_t> resultBuffer = Napi::Buffer<uint8_t>::Copy(env, outputBuffer.Data(), outputBuffer.Length());

    return resultBuffer;
}

Napi::Object firstCpp::DSIHEWrapped(const Napi::CallbackInfo& info) {

    Napi::Buffer<uint8_t> resultBuffer = firstCpp::DSIHE(info);

    // Create a new Buffer and copy the data
    Napi::Buffer<uint8_t> resultCopy = Napi::Buffer<uint8_t>::Copy(info.Env(), resultBuffer.Data(), resultBuffer.Length());

    Napi::Object resultObject = Napi::Object::New(info.Env());
    resultObject.Set("data", resultCopy);
    resultObject.Set("length", Napi::Number::New(info.Env(), resultBuffer.Length()));

    return resultObject;
}

Napi::Object firstCpp::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, firstCpp::HelloWrapped));
    exports.Set("add", Napi::Function::New(env, firstCpp::AddWrapped));
    exports.Set("convertToGrayscale", Napi::Function::New(env, firstCpp::ConvertToGrayscaleWrapped));
    exports.Set("BBHE", Napi::Function::New(env, firstCpp::BBHEWrapped));
    exports.Set("DSIHE", Napi::Function::New(env, firstCpp::DSIHEWrapped));



    return exports;
}