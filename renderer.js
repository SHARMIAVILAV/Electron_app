// const { ipcRenderer } = require('electron');

// const btn = document.getElementById('btn')
// const filePathElement = document.getElementById('filePath')

// btn.addEventListener('click', async () => {
//   const filePath = await window.electronAPI.openFile()
//   filePathElement.innerText = filePath
// })



// // Assume imageData is the data you want to process
// const imageData = filePath;

// // Send the data to the main process for NAPI processing
// ipcRenderer.invoke('processImage', imageData)
//   .then(result => {
//     // Handle the result and update the UI
//     console.log('Processed result:', result);
//     // Update UI logic here
//   })
//   .catch(error => {
//     console.error('Error processing image:', error);
//   });

// const imageInput = document.getElementById('imageInput');
// const outputContainer = document.getElementById('outputContainer');



// document.getElementById('imageInput').addEventListener('change', function (event) {
//   const fileInput = event.target;
//   const file = fileInput.files[0];

//   if (file) {
//     const reader = new FileReader();

//     reader.onload = function (e) {
//       const imageDataUrl = e.target.result;
//       console.log(imageDataUrl);
//       displayImage('inputImageContainer', imageDataUrl);
//     }
//   };
// });

// function displayImage(containerId, imageDataUrl) {
//   const imageContainer = document.getElementById(containerId);
//   const imageElement = document.createElement('img');

//   imageElement.src = imageDataUrl;
//   imageContainer.innerHTML = '';
//   imageContainer.appendChild(imageElement);
// }


// async function processImage() {
//   const filePath = imageInput.files[0]?.path;
//   if (!filePath) {
//     alert('Please select an image.');
//     return;
//   }

//   try {
//     const buffer = await window.electronAPI.getImageBuffer(filePath);
//     const result = await window.electronAPI.processImage(buffer);

//     // Display the result (you can customize this part based on your needs)
//     const imgElement = document.createElement('img');
//     imgElement.src = `/home/sharmi/my-electron-app/output.jpg`;
//     outputContainer.innerHTML = '';
//     outputContainer.appendChild(imgElement);
//   } catch (error) {
//     console.error(error.message);
//   }
// }

const imageInput = document.getElementById('imageInput');
const outputContainer = document.getElementById('outputContainer');

document.getElementById('imageInput').addEventListener('change', function (event) {
  const fileInput = event.target;
  const file = fileInput.files[0];

  if (file) {
    const reader = new FileReader();

    reader.onload = function (e) {
      const imageDataUrl = e.target.result;
      console.log(imageDataUrl);
      displayImage('inputImageContainer', imageDataUrl);
    };

    reader.readAsDataURL(file); // This line was missing
  }
});

function displayImage(containerId, imageDataUrl) {
  const imageContainer = document.getElementById(containerId);
  const imageElement = document.createElement('img');

  imageElement.src = imageDataUrl;
  imageContainer.innerHTML = '';
  imageContainer.appendChild(imageElement);
}
function selectFunction() {
  var selectedFunction = document.getElementById("functionSelect").value;

  switch (selectedFunction) {
      case "processImage":
          processImage();
          break;
      case "BBHE_function":
          BBHE_function();
          break;
      case "DSIHE_function":
          DSIHE_function();
          break;
      default:
          processImage();
          break;
  }
}

async function processImage() {
  const filePath = imageInput.files[0]?.path;
  if (!filePath) {
    alert('Please select an image.');
    return;
  }

  try {
    const buffer = await window.electronAPI.getImageBuffer(filePath);
    const result = await window.electronAPI.processImage(buffer);

    // Display the result (you can customize this part based on your needs)
    const imgElement = document.createElement('img');
    imgElement.src = `/home/sharmi/my-electron-app/output.jpg`;
    outputContainer.innerHTML = '';
    outputContainer.appendChild(imgElement);
  } catch (error) {
    console.error(error.message);
  }
}

async function BBHE_function() {
  const filePath = imageInput.files[0]?.path;
  if (!filePath) {
    alert('Please select an image.');
    return;
  }

  try {
    const buffer = await window.electronAPI.getImageBuffer(filePath);
    const result = await window.electronAPI.BBHE_function(buffer);

    // Display the result (you can customize this part based on your needs)
    const imgElement = document.createElement('img');
    imgElement.src = `/home/sharmi/my-electron-app/BBHE.jpg`;
    outputContainer.innerHTML = '';
    outputContainer.appendChild(imgElement);
  } catch (error) {
    console.error(error.message);
  }
}

async function DSIHE_function() {
  const filePath = imageInput.files[0]?.path;
  if (!filePath) {
    alert('Please select an image.');
    return;
  }

  try {
    const buffer = await window.electronAPI.getImageBuffer(filePath);
    const result = await window.electronAPI.DSIHE_function(buffer);

    // Display the result (you can customize this part based on your needs)
    const imgElement = document.createElement('img');
    imgElement.src = `/home/sharmi/my-electron-app/DSIHE.jpg`;
    outputContainer.innerHTML = '';
    outputContainer.appendChild(imgElement);
  } catch (error) {
    console.error(error.message);
  }
}