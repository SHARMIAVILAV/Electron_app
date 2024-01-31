// #include <opencv2/opencv.hpp>
// #include <napi.h>

// Napi::Buffer<uint8_t> ProcessImage(const Napi::Env& env, const Napi::Buffer<uint8_t>& inputBuffer) {
//     cv::Mat inputImage = cv::imdecode(cv::Mat(1, inputBuffer.Length(), CV_8UC1, inputBuffer.Data()), cv::IMREAD_UNCHANGED);

//     // Perform your OpenCV processing on inputImage
//     cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);

//     // Encode the result image
//     std::vector<uint8_t> outputBuffer;
//     cv::imencode(".png", inputImage, outputBuffer);

//     return Napi::Buffer<uint8_t>::Copy(env, outputBuffer.data(), outputBuffer.size());
// }

// Napi::Object Init(Napi::Env env, Napi::Object exports) {
//     exports.Set("processImage", Napi::Function::New(env, ProcessImage));
//     return exports;
// }

// NODE_API_MODULE(opencvAddon, Init)

#include <napi.h>
#include "firstCpp.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return firstCpp::Init(env, exports);
}

NODE_API_MODULE(opencvAddon, InitAll)

// #include <napi.h>
// #include <opencv2/opencv.hpp>
// #include "firstCpp.h"

// // Napi::Buffer<uint8_t> ProcessImage(const Napi::Env& env, const Napi::Buffer<uint8_t>& inputBuffer) {
// //     // Decode the input image buffer into an OpenCV Mat
// //     cv::Mat inputImage = cv::imdecode(cv::Mat(1, inputBuffer.Length(), CV_8UC1, inputBuffer.Data()), cv::IMREAD_UNCHANGED);

// //     // Process the image (convert to grayscale in this example)
// //     cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);

// //     // Encode the processed image into a buffer
// //     std::vector<uint8_t> outputBuffer;
// //     cv::imencode(".png", inputImage, outputBuffer);

// //     // Return the processed image buffer
// //     return Napi::Buffer<uint8_t>::Copy(env, outputBuffer.data(), outputBuffer.size());
// // }

// Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
//     exports.Set("processImage", Napi::Function::New(env, firstCpp::ProcessImage));
//     return exports;
// }

// NODE_API_MODULE(opencvAddon, InitAll)
