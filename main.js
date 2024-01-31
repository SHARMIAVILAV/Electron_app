
const { app, BrowserWindow, dialog, ipcMain } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const opencvAddon = require('./build/Release/opencvAddon.node');

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: false, // Set to false to ensure context isolation
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // mainWindow.webContents.openDevTools();

  mainWindow.loadFile(path.join(__dirname, 'index.html'));

  mainWindow.on('closed', function () {
    mainWindow = null;
  });
}

app.whenReady().then(createWindow);

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit();
});

app.on('activate', function () {
  if (mainWindow === null) createWindow();
});

ipcMain.handle('get-image-buffer', async (_event, imageLocation) => {
  try {
    const buffer = await fs.readFile(imageLocation);
    return buffer;
  } catch (error) {
    throw new Error(`Failed to read image from ${imageLocation}. Error: ${error.message}`);
  }
});

ipcMain.handle('process-image', async (_event, buffer) => {
  try {
    const result = opencvAddon.convertToGrayscale(buffer); // Adjust the actual function name if needed
    return result;
  } catch (error) {
    throw new Error(`Failed to process image. Error: ${error.message}`);
  }
});

ipcMain.handle('BBHE-image', async (_event, buffer) => {
  try {
    const result = opencvAddon.BBHE(buffer); // Adjust the actual function name if needed
    return result;
  } catch (error) {
    throw new Error(`Failed to process image. Error: ${error.message}`);
  }
});

ipcMain.handle('DSIHE-image', async (_event, buffer) => {
  try {
    const result = opencvAddon.DSIHE(buffer); // Adjust the actual function name if needed
    return result;
  } catch (error) {
    throw new Error(`Failed to process image. Error: ${error.message}`);
  }
});

console.log('addon', opencvAddon)

module.exports = opencvAddon;
