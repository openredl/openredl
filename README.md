# OpenREDL
**OpenREDL** is a project to develop an Open Source Datalogger for Renewable Energy applications. It consist of three parts:

- Hardware: The integration of open source hardware (in the moment mainly arduino and arduino compatible shields) into a flexible and robust datalogger.
- Firmware: Open source Arduino IDE software which runs on the hardware to measure and log data.
- Operation Software: Open source Python tools for Real Time Plotting, and data analysis.

## Description
The aim is to have a datalogger(DL) for renewable energy learning applications based on open source hardware, which is robust, economic, simple to manufacture, use and repair. The DLs were developed to be used in applications related to renewable energy learning activities, specifically if they require high resolution analog voltage measurements.
It can operate either for real-time monitoring, real time plotting, or as stand along system.


## Usage
Please have a look at the documentation:

 - 0. Download or clone the git repository.
 - [1. Quick Start](Documentation/UserManual_1_Intro.md) to start operating the DLs.
 - [2. Hardware](Documentation/UserManual_2_Hardware.md) describes the _DataloggerArduino21_ and _DataloggerGBE23_ hardware.
 - [3. Firmware](Documentation/UserManual_3_Firmware.md) explains the DLs firmware (an Arduino IDE .ino file) and how to configure it.
 - [4. Real-Time Plotting](Documentation/UserManual_4_RealTimePlotting.md) explains the use and configuration of the Python script to plot the measurements in real time.
 - [5. Validation](Documentation/UserManual_5_Validation.md) present initial validation data, comparing measurements taken with a professional datalogger.
 - [6. Learning Tests](Documentation/UserManual_6_LearningTests.md) follow these tests to train openREDL basic skills.

## Project status, contributing and support
The project is on active development! These and previous versions are being used on renewable energy learning laboratories and pilot tests in community energy projects.
Please report any issue and request functions required for your use cases.

## Roadmap
We are working on developing the open source datalogger software and hardware.
As soon as we can, we will develop the following updates:
 - update the hardware to a new MCU (cheaper, faster and with internet access),
 - create harware gerber files to enable easy hardware reproduction,
 - improve the firmware code,
 - add functions for different use cases,
 - and more.

## Authors and acknowledgment

The project is being developed by the Postgraduate Programme Renewable Energy, Energy Technology Group of the University of Oldenburg, with support from the projects:
- the PPRE Team in the University of Oldenburg,
- the University of Oldenburg Electronics Workshop,
- GIZ Grüne Bürger Energie, with the project "Open Source Datalogger" in collaboration with the DKUT in Kenya.
- DAAD with the project "Open data for Energy access"


## License
MIT


