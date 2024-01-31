
// document.getElementById('imageInput').addEventListener('change', function (event) {
//   const fileInput = event.target;
//   const file = fileInput.files[0];

//   if (file) {
//     const reader = new FileReader();

//     reader.onload = function (e) {
//       const imageDataUrl = e.target.result;
//       displayImage('inputImageContainer', imageDataUrl);
     
//       document.getElementById('functionSelect').addEventListener('change', function (event) {
//         const selectedFunction = event.target.value;
//         // switch (selectedFunction) {
//         //   case "BBHE":
//         //     opencvAddon.BBHE()
//         //     break;
//         //   case "DSIHE":
//         //     opencvAddon.DSIHE()
//         //     break;
//         //   case "THE":
//         //     opencvAddon.THE()
//         //     break;
//         //   default:
//         //     opencvAddon.BBHE()
//         //     break;
//         // }
//         document.getElementById('outputImageContainer').innerHTML = `<img src="${selectedFunction}_output.jpg" alt="${selectedFunction} Output">`;
//         displayImage('outputImageContainer', "./output.jpg");
//       });
//     };

//     reader.readAsDataURL(file);
//   }




// });

// // document.getElementById('functionSelect').addEventListener('change', function(event) {
// //   const selectedFunction = event.target.value;
// //   document.getElementById('outputImageContainer').innerHTML = `<img src="${selectedFunction}_output.jpg" alt="${selectedFunction} Output">`;
// // });

// function displayImage(containerId, imageDataUrl) {
//   const imageContainer = document.getElementById(containerId);
//   const imageElement = document.createElement('img');

//   imageElement.src = imageDataUrl;
//   imageContainer.innerHTML = '';
//   imageContainer.appendChild(imageElement);
// }
// --------------
// const { contextBridge, ipcRenderer } = require('electron/renderer')

// contextBridge.exposeInMainWorld('electronAPI', {
//   openFile: () => ipcRenderer.invoke('dialog:openFile')
// })

// -----

const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
  getImageBuffer: async (imageLocation) => {
    try {
      const buffer = await ipcRenderer.invoke('get-image-buffer', imageLocation);
      return buffer;
    } catch (error) {
      throw new Error(`Failed to get image buffer. Error: ${error.message}`);
    }
  },
  processImage: async (buffer) => {
    try {
      const result = await ipcRenderer.invoke('process-image', buffer);
      return result;
    } catch (error) {
      throw new Error(`Failed to process image. Error: ${error.message}`);
    }
  },
  BBHE_function: async (buffer) => {
    try {
      const result = await ipcRenderer.invoke('BBHE-image', buffer);
      return result;
    } catch (error) {
      throw new Error(`Failed to process image. Error: ${error.message}`);
    }
  },
  DSIHE_function: async (buffer) => {
    try {
      const result = await ipcRenderer.invoke('DSIHE-image', buffer);
      return result;
    } catch (error) {
      throw new Error(`Failed to process image. Error: ${error.message}`);
    }
  },
});
