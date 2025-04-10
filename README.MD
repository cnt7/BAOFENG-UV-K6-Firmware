# SOURCE CODE FOR BAOFENG UV-K6x Radios-CNT7 UPLOAD
## Introduction

This repository contains the firmware source code for the Baofeng UV-K6x series of radios. The code is written in C and is intended to be compiled with the GCC compiler. The code is released under the BFPL-1.0 license.

## Development Environment

### 1. Install Visual Studio Code
First, you need to install Visual Studio Code (VS Code). You can download and install it from the [Visual Studio Code official website](https://code.visualstudio.com/).

### 2. Install PlatformIO IDE Extension
The code is intended to be compiled with the PlatformIO IDE extension for VS Code. To install the PlatformIO IDE extension, follow these steps:
1. Open VS Code.
2. Click on the Extensions icon in the left sidebar.
3. Search for "PlatformIO IDE" in the Extensions Marketplace.
4. Click the "Install" button to install the PlatformIO IDE extension.

### 3. Radio specific Platform Installation:
Click "PlatformIO Core CLI" from VSCode PlatformIO Panel -> Quick Access -> Miscellaneous.
Enter below install commands:
```
pio pkg install -g -p https://github.com/cnt7/BAOFENG-UV-K6-Firmware
```

When you are using Linux, you need to install the udev rules for OpenOCD
Copy the 60-openocd.rules file under tool-openocd-kd32 package to /etc/udev/rules.d/ directory.
```
sudo cp ~/.platformio/packages/tool-openocd-kd32/contrib/60-openocd.rules  /etc/udev/rules.d/
```
Refresh the udev rules.
```
sudo udevadm control --reload-rules && sudo udevadm trigger
```

## Building

To build the code, follow these steps:
1. Open the project in Visual Studio Code.
2. Click on the PlatformIO icon in the left sidebar.
3. Click on the "Build" button to build the code.
4. The code will be compiled, and the output binary file will put into ./out/ folder.

## Uploading

To upload the code to the radio, you will need to use a programming cable. You can find programming cables for the Baofeng UV-K6x series of radios on eBay or Amazon. Once you have the programming cable, you can upload the code by running the BFK6_Bootloader.exe program on a Windows computer.

To upload the code to the radio, follow these steps:
1. Power off your UV-K6 radio.
2. Connect the programming cable to the radio and the computer.
3. Press and hold 2 ORANGE side buttons on the radio, then turn on the radio, you will see the screen shows "UPDATE" and the radio will be in the update mode.
4. Run the BFK6_Bootloader.exe program on your computer.
5. Click the "..." button and select the output binary file from the ./out/ folder.
6. Select the COM port that the programming cable is connected to.
6. Click the large "按两个侧键开机进入升级(Power on with side buttons pressed to Enter Upgrade)" button to upload the code to the radio.

## License

The code is released under the BFPL-1.0 license. for more information, please see the LICENSE file.

## Copyright
Copyright © 2024 Baofeng. All rights reserved.

## Contributors
- BD4VOW ;  Starry Night Quintet- CNT7 DarkSoma OHMYTIME  X 
    - provided gcc, openocd and PlatformIO support.
