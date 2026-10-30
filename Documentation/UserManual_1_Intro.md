## 0. Introduction 
The user manual introduce in detail the use and construction of the _DataloggerArduino21_ and the _DataloggerGBE23_, developed by the PPRE of the Energy Technology Group in the University of Oldenburg. The aim is to have a datalogger(DL) for renewable energy learning applications based on open source hardware, which is robust, economic, simple to manufacture, use and repair. These DLs compile the experience gained during the previous years with open source and commercial DL, specifically within learning laboratories. The DLs are designed to keep a simple and modular approach, integrating available open source hardware, plus self-designed filtering components and a robust hardware interface. The firmware is written with the Arduino IDE, using open source libraries. As their name indicate, the _DataloggerArduino21_ is the first version from 2021, and the _DataloggerGBE23_ is the second version, developed in 2023 with the support of GBE from GIZ.

We understand a DL as a "device that record data over time... either with a buit-in instrument... or via external instruments...", "...typically with slower sampling rates (>1Hz)..." (See [https://en.wikipedia.org/wiki/Data_logger]). DL are essential in data acquisition systems to learn, understand and research energy systems. They are able to take measurements from sensors, perform previously programmed calculations and store the data in memory.

The DLs were developed to be used in applications related to renewable energy learning activities, specifically if they require high resolution analog voltage measurements. All instrumentation (sensors, transducers and signal adaptation) specific to each application to be measured are considered as external to the DL (and let out of its basic design), with exception to noise filters.

Although these DLs do not aim to replace professional and industrial DL applications, using the right external instrumentation and the right code, they can fulfil accurately specific tasks similar to them, e.g. energy meters.

The documentation consist in the following sections:

 - [1. Quick Start](#1-quick-start) (See below) to start operating the DLs, either for real-time monitoring, plotting, or as stand along system.
 - [2. Hardware](UserManual_2_Hardware.md) is a detail description of the _DataloggerArduino21_ and _DataloggerGBE23_ hardware.
 - [3. Firmware](UserManual_3_Firmware.md) is a detailed explanation of the DLs firmware, and Arduino IDE .ino file and how to configure it.
 - [4. Real-Time Plotting](UserManual_4_RealTimePlotting.md) explains the use and configuration of the Python script.
 - [5. Validation](UserManual_5_Validation.md) present initial validation data, comparimg measurements with a professional datalogger.
 - [6. Learning Tests](UserManual_6_LearningTests.md) follow these tests to train openREDL basic skills.

-----
-----

## 1. Quick Start

The operation flow of the DLs can be summarized in the following steps:

> a. Installation > b. Configuration > c. Connection > d. Monitoring

-----

### a. Installation
**Only to be done the first time**. Follow the next steps to prepare your computer:

0. a. Open the File Manager (Windows) or Finder (Mac) and create a folder to store the Lab documents, for example Documents\Labs.
b. Go into the Documents\Labs folder and copy its path: right click in the File Manager top bar and copy (Ctr + C). 
c. Open a Terminal in which you can execute conda (e.g. Miniforge Prompt) and go to the path. For example
```console
cd [your_path]\Documents\Labs
```

1. Make a copy of the repository.

Option A: Clone the openREDL repository with git. 
```console
git clone https://github.com/openredl/openredl.git
```

Option B: Download the repository from https://github.com/openredl/openredl.
    
Click in Code > Download ZIP (Store it in the Labs folder and extract it) 

2. Create the python environment to plot in real time. [See more details in 4. Real-Time Plotting](UserManual_4_RealTimePlotting.md).
```console
conda create -n openredl-env matplotlib pandas pyserial
```

2. 1 Activate the environment.
```console
conda activate openredl-env
```

3. Install the _pyopenredl_ library from the repository folder.

Option A: If you cloned it.
```console
python -m pip install ./openredl/Tools/pyopenredl/
```

Option B: If you downloaded it, verify the path, some times openredl-main is 2 times in it.
    
```console
python -m pip install ./openredl-main/Tools/pyopenredl/
```

or sometimes:

```console
python -m pip install .openredl-main/openredl-main/Tools/pyopenredl/
```

4. [Download the Arduino IDE from here](https://www.arduino.cc/en/software) and install it. Although it is normal to get requests to install many libraries, pay attention to what you accept.

5. Install the Arduino IDE libraries from the folder openredl\Firmware. You need to upload one by one (OpenREDL_lib, RTClib, I2Cdev and ADS1115) as follows:

> Arduino IDE > Sketch > Include Library > Add .ZIP Library.

6. See the example script in the folder ***WinterLab_CodeTemplates***.
There are 2 files in each folder, an .ino file to program the data logger and a .py file for real time plotting.

-----

## b. To Program the data logger   
1. Open the file Firmware > [your_firmware].ino with Arduino IDE.
2. Adapt the script as required.
3. In Menu Tools > Board select Arduino Uno.
4. In Menu Tools > Port select the port that has the Arduino Uno connected.
5. In Menu Sketch> Upload (Or click in the Upload button – an arrow to the right in Menu). 

[See full details in 3. Firmware](UserManual_3_Firmware.md).

-----

## c. To Plot in Real Time
The *real_time_plot_[experiment_name].py* file is prepared to plot what is required for each experiment. It is executed from a Terminal (e.g. Miniforge Prompt) as in the example below. **The folder and file names are different for each experiment**. 

Activate the environment:
```console
conda activate openredl-env
```
Go to the right folder (Get the right path from the file manager):
```console
cd WinterLab_CodeTemplates/winterlab_intro/
```
Execute the python plotting script (Get the right file name from the file manager):
```console
python real_time_plot_winterlab_intro.py
```

-----

## d. Try the Winterlab_Intro Examples
1. Connect the Solar cell to measure Voltage.
2. Connect the Solar cell to measure Current with a shunt resistor. Insert the shunt resistor calibration value and change the channel Label.
3. Connect the PT100/0 to the 1mA current source. Measure the Pt100/0 voltage.
4. Explore the solar cell and solar collector scripts.

**TIPs**
- Validate the measurements with a multimeter.
- Before connecting anything draw the circuit on a paper.

## NOTES

a) The measurements plotted by the real time plot script are stored in the same script folder in the file openredl_log.csv

b) The Real Time Plot has in the bottom a text box where you can:
 - 1. Write notes to be stored in the log (e.g. Start experiment1).
 - 2. Write **:PAUSE** to get 10 seconds (You can use it to remove safely the SDCard).
 - 3. Write **:Start** or **:Stop** to decide the break time by yourself (You can use it to remove safely the SDCard).

c) Always upload a firmware configuration to the data logger. You do not know what the previous user programmed it for.


The _openREDL-Arduino21_ and _openREDL-GBE23_ can be used as:
- a Stand Alone Monitoring System, storing the measurements in the SD Card or as
- a Real-Time Monitoring System, reading the measurements via the USB port as text or plotting them with .


d) Stand Alone Monitoring
Connect the DL to a USB connector, such as a celphone charger. The measurements start and will be automatically stored in the SD-Card inserted in the back part of the datalogger. The measurements can be retrieved removing the card and copying the generated text files.

e) Real-Time Monitoring
Connect the DL to a computer with the USB cable. The measurements start and will be automatically stored in the SD-Card. Real-Time Monitoring consist in reading the measurements in the moment they are taken. Use the python plotting tool (the real_time_plot.py script), or the Arduino IDE serial Monitor at a Baud Rate of 115200 bps.

**Attention** The DL can only establish communication with one program at a time. If it can not connect, pay attention that no other connection to the Arduino is active. 

f) Monitor Values with Arduino IDE Serial Monitor
A simple option to see the measurements in real time is to use the Arduino IDE Serial Monitor. To use it, you need to:
 1. Open the Arduino IDE
 2. Click Menu Tools > Board select Arduino Uno
 3. Click In Menu Tools > Port select the port that has the Arduino Uno connected
 4. In Menu Tools > open the Serial Monitor
 5. In the Serial Monitor bottom right select 115200 Baud



