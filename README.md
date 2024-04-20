# Utopia Capstone Trail Counter

## Overview
This repository contains the code and installation instructions to build a person counter on the [Nvidia Jetson Nano](https://developer.nvidia.com/embedded/jetson-nano-developer-kit). The counter uses the COCO detection model and python to track objects. The counter currently outputs a live video feed with a count overlayed showing the number of people counted since the program began.

## Required Hardware
- Nvidia Jetson Nano
- [Arducam Mini 8MP IMX219 Camera Module](https://www.amazon.com/Arducam-Camera-Module-NVIDIA-Distortion/dp/B082W4ZSM9/ref=sr_1_2?keywords=mipi%2Bcamera&qid=1694528693&sr=8-2&th=1) (or the [official store here](https://www.amazon.com/stores/Arducam/page/35052708-55DC-4832-A0B6-A9451F99DF23?ref_=ast_bln))
- keyboard, mouse, and monitor (for initial setup)
- Micro SD card (at least 32GB)
- [2.5A 5.1V power supply](https://www.pishop.us/product/wall-adapter-power-supply-micro-usb-2-4a-5-25v/) or optionally a [5V 4A barrel jack power supply](https://www.adafruit.com/product/1466) for higher power draw.

## Installation
### Jetson Jetpack
- Flash most recent version of [jetpack](https://developer.nvidia.com/jetson-nano-sd-card-image) onto your Micro SD card. I used version 4.6.1.
    - For a step by step guide on flashing the image to your SD card, see [here](https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit).
- Insert the SD card into the Jetson Nano and power it on.
- Follow the on screen instructions to setup the Jetson Nano. 
- Run the following commands to update and upgrade the system:
    ```bash
    sudo apt-get update
    sudo apt-get upgrade -y
    ```
- Build the Jetson inference repository from source: Either follow the instructions below, or follow [this guide](https://github.com/dusty-nv/jetson-inference/blob/master/docs/building-repo-2.md)
- Download this repo and run the counter.py program using python3
    ```bash
    git clone https://github.com/nairbdryb/utopia-cam-v2
    cd utopia-cam-v2
    python3 -m pip3 install -r requirements.txt
    python3 counter.py
    ```

## Installation of jetson-inference from source:
```bash
sudo apt-get update
sudo apt-get install git cmake libpython3-dev python3-numpy
git clone --recursive --depth=1 https://github.com/dusty-nv/jetson-inference
cd jetson-inference
mkdir build
cd build
cmake ../
make -j$(nproc)
sudo make install
sudo ldconfig
```

## Setup
- Connect the Arducam camera to the Jetson Nano's camera port.
- Connect the Jetson Nano to a monitor, keyboard, and mouse.
- Power on the Jetson Nano.
- Log in
- Clone this repository and run the counter.py program using python3
    ```bash
    git clone https://github.com/nairbdryb/jetson-nano-counter-v2
    ```
- create the service file
    ```bash
    sudo cp ./Service/counter.service /etc/systemd/system/counter.service
    ```
- enable the service
    ```bash
    sudo systemctl enable counter.service
    ```
- start the service
    ```bash
    sudo systemctl start counter.service
    ```
- check the status of the service
    ```bash
    sudo systemctl status counter.service
    ```
- If the service is running correctly, you may now disconnect the monitor, keyboard, and mouse. The counter will now run on boot.

## Troubleshooting
If you get an error that says /dev/ttyUSB0 does not have permission, you may need to add your user to the dialout group. Run the following command to add your user to the dialout group:
```bash
sudo usermod -a -G dialout {USER}
```

## Description of Files
- counter.py: The main program that runs the counter. This program tracks the number of people on the screen, current bounding boxes, the video input and output stream, and the current frame.
- centroidtracker.py: This file contains the logic for tracking objects. It calculates the distance between a bounding box in this frame and previous frames to determine if it is a new object or an existing object.
- requirements.txt: This file contains the python packages required to run the counter.py program.
- README.md: This file contains the instructions for installing and running the counter.py program. You are looking at this file right now.
- chirpstack.py: This file has not been implemented into the main program yet. It contains logic for sending data through the serial port to a LoRa compatible Arduino. This Arduino would then send the data to a LoRa gateway to be forwarded to a server. This feature will be implemented in a future version.