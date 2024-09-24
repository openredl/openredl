## 0. Introduction 
The user manual introduce in detail the use and construction of the _DataloggerArduino21_ and the _DataloggerGBE23_, developed by the PPRE of the Energy Technology Group in the University of Oldenburg. The aim is to have a datalogger(DL) for renewable energy learning applications based on open source hardware, which is robust, economic, simple to manufacture, use and repair. These DLs compile the experience gained during the previous years with open source and commercial DL, specifically within learning laboratories. The DLs are designed to keep a simple and modular approach, integrating available open source hardware, plus self-designed filtering components and a robust hardware interface. The firmware is written with the Arduino IDE, using open source libraries. As their name indicate, the _DataloggerArduino21_ is the first version from 2021, and the _DataloggerGBE23_ is the second version, developed in 2023 with the support of GBE from GIZ.

We understand a DL as a "device that record data over time... either with a buit-in instrument... or via external instruments...", "...typically with slower sampling rates (>1Hz)..." (See [https://en.wikipedia.org/wiki/Data_logger]). DL are essential in data acquisition systems to learn, understand and research energy systems. They are able to take measurements from sensors, perform previously programmed calculations and store the data in memory.

The DLs were developed to be used in applications related to renewable energy learning activities, specifically if they require high resolution analog voltage measurements. All instrumentation (sensors, transducers and signal adaptation) specific to each application to be measured are considered as external to the DL (and let out of its basic design), with exception to noise filters.

Although these DLs do not aim to replace professional and industrial DL applications, using the right external instrumentation and the right code, they can fulfil accurately specific tasks similar to them, e.g. energy meters.

The documentation consist in the following sections:

 - [1. Quick Start](#1-quick-guide) (See below) to start operating the DLs, either for real-time monitoring, plotting, or as stand along system.
 - [2. Hardware](UserManual_2_Hardware.md) is a detail description of the _DataloggerArduino21_ and _DataloggerGBE23_ hardware.
 - [3. Firmware](UserManual_3_Firmware.md) is a detailed explanation of the DLs firmware, and Arduino IDE .ino file and how to configure it.
 - [4. Real-Time Plotting](UserManual_4_RealTimePlotting.md) explains the use and configuration of the Python script.
 - [5. Validation](UserManual_5_Validation.md) present initial validation data, comparimg measurements with a professional datalogger.

## 1. Quick Guide

The _DataloggerArduino21_ and _DataloggerGBE23_ can be used as:
- a Stand Alone Monitoring System, storing the measurements in the SD Card or as
- a Real-Time Monitoring System, reading the measurements via the USB port, processing, storing or plotting the data as required (for example with the Arduino IDE serial monitoring or with the real_time_plot.py).

The operation flow of the DLs can be summarized in the following steps:

> a. Preparation > b. Configuration > c. Connection > d. Monitoring

### a. Preparation
Follow the next steps to prepare your computer to use the DLs:

1. Install Arduino IDE. [You can download it from here.](https://www.arduino.cc/en/software)
2. Make a copy of the repository on your computer.
3. Install the required arduino libraries. The simplest method is to use the Libraries from the Firmware folder in the repository. Then upload each of them (OpenREDL_lib, RTClib, I2Cdev and ADS1115) to the Arduino IDE: 
>Arduino IDE > Sketch > Include Library > Add .ZIP Library.
4. In case you will monitor in real time with the Python Script, create the required environment s specified in [4. Real-Time Plotting](UserManual_4_RealTimePlotting.md)


### b. Configuration
To take measurements, the DLs execute a script (its firmware, which is an .ino file programmed in the ArduinoIDE) in a loop. The script runs in the DL microprocessor, inside an Arduino UNO.
Once the firmware is configured to take the required measurements, it has to be uploaded to the DL using the Arduino IDE software. **If you do not know exactly which firmware configuration was uploaded to the Datalogger, you should always upload a new one**. If not, you risk to take measurements configured by an unknown configuration for measurements and logging. 

5. With the Arduino IDE, open an ino file, for example a File > Examples > OpenREDL_lib > all_possibilities. Configure the parameters, samples to take and measurements to log as described in [3. Firmware](UserManual_3_Firmware.md)

6. Define the configuration parameters you require: logging interval, sensors ranges, values to log, ac or dc measurements.

### c. Connection
7. (Dis)connect the noise filter inside the DLs as required, as shown in [Figure 3 of 2. Hardware](UserManual_2_Hardware.md).

8. Connect the required inputs and outputs, as shown in [Figure 1 of 2. Hardware](UserManual_2_Hardware.md).

#### d. Monitoring
##### Stand Alone Monitoring
Connect the DL to a USB connector, such as a celphone charger. The measurements start and will be automatically stored in the SD-Card inserted in the back part of the datalogger. The measurements can be retrieved removing the card and copying the generated text files.

##### Real-Time Monitoring
Connect the DL to a computer with the USB cable. The measurements start and will be automatically stored in the SD-Card. Real-Time Monitoring consist in reading the measurements in the moment they are taken. You can use any software that read the serial port where the arduino is connected. The default serial port Baud Rate is 115200 bps.
Attention: The DL can only stablish communication with one program at a time. If it can not connect, pay attention that no other connection to the Arduino is active. 

###### With Arduino IDE Serial Monitor
A simple option to see the measurements in real time is to use the Arduino IDE Serial Monitor. To use it, you need to:
 - 1. Open the Arduino IDE
 - 2. Click Menu Tools > Board select Arduino Uno
 - 3. Click In Menu Tools > Port select the port that has the Arduino Uno connected
 - 4. In Menu Tools > open the Serial Monitor
 - 5. In the Serial Monitor bottom right select 115200 Baud

###### With the Python Script
A Python script (or any other programming language) can read the serial port and plot it. In the Tools folder you can find a specially made script called real_time_plot.py. For details on how to use and configure it see [4. Real-Time Plotting](UserManual_4_RealTimePlotting.md).


