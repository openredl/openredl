### 3. Firmware

The DLs function executing its firmware in a microcontroler within an Arduino UNO board. The firmware is an .ino script writen in the Arduino IDE, using an adaptation of C programming language for the Arduino IDE. The firmware has some parameters that must be configured according to each application needs.
Once configured, it has to be uploaded to the Datalogger using the Arduino IDE software. If you do not know the uploaded script, you risk to take measurements configured by other user configuration. To have reliable measurements, you must be sure of what is being measured and logged.

#### 3.1 Upload a script to the the DL 
To upload the firmware of the DL, you need to follow these steps:

 - 1. Open the file Firmware > [your_firmware].ino with Arduino IDE.
 - 2. Adapt the script as required.
 - 3. In Menu Tools > Board select Arduino Uno.
 - 4. In Menu Tools > Port select the port that has the Arduino Uno connected.
 - 5. In Menu Sketch> Upload (Or click in the Upload button – an arrow to the right in Menu).
 
#### 3.2 Firmware Adaptation
You can find some application examples in:
 > Arduino IDE > File > Examples > OpenREDL_lib
The examples show a general configuration of the datalogger, which you should adapt for your specific applications.

The configuration consist in the methods:
- __setup()__, where the parameters are initialized.
- __samples_to_take()__, where the sampling routines are specified,
- __to_log_content()__, where the data to log (serial port and sdcard file) as timeseries is defined.

After changing the required parameters, upload the script to the Datalogger.

##### 3.2.1 setup()
This function is executed once at the datalogger startup, and provide all initial configurations. You can configure the parameters in the firmware script, according to your needs.

General parameters:
 - format_log = Log format as TIMESERIES or JSON.
 - storage_interval_s = seconds. Default 10s. Decimal values are accepted in multiples of 0.1s, with lower time keeping precision.
 
For each differential channel:
 - diffx.gain: the ADC measuring range, defined by the sensor maximum possible output. Default ADS1115_PGA_2P048. More options are given in Table 3.
 - diffx.mult_factor: FLOAT, the multiplication factor to convert measurements, from the sensor callibration. Default 1.
 - diffx.offset: float, an addig offset of the sensor. Default 0.
 - diffx.units: STRING, describes the measurement units. Default "mV".

The measurement result is calculated by the formula:

$` Result = Measurement * diffx.mult_factor + diffx.offset`$

<table>
<caption> Table 3: ADC gain options and details.</caption>

<tr>
<th>Gain Code</th>
<th>Gain (Internal Multiplier)</th>
<th>Range</th>
<th>Resolution (mV/bit)</th>
</tr>

<tr>
<td>ADS1115_PGA_6P144</td>
<td>2/3x</td>
<td>+/- 6.144V</td>
<td>0.1875mV (default)</td>
</tr>

<tr>
<td>ADS1115_PGA_4P096</td>
<td>1x</td>
<td>+/- 4.096V</td>
<td>0.125mV</td>
</tr>

<tr>
<td>ADS1115_PGA_2P048</td>
<td>2x</td>
<td>+/- 2.048V</td>
<td>0.0625mV</td>
</tr>

<tr>
<td>ADS1115_PGA_1P024</td>
<td>4x</td>
<td>+/- 1.024V</td>
<td>0.03125mV</td>
</tr>

<tr>
<td>ADS1115_PGA_0P512</td>
<td>8x</td>
<td>+/- 0.512V</td>
<td>0.015625mV</td>
</tr>

<tr>
<td>ADS1115_PGA_0P256</td>
<td>16x</td>
<td>+/- 0.256V</td>
<td>0.0078125mV</td>
</tr>
</table>




> :warning: ATTENTION :warning:
> If you measure a value out of the maximum range, the Arduino will give the maximum value. (E.g. if you connect 280 mV in an arduino configured in the range of +/- 0.256V, you will get 255?/256? mV and not a real measurement)
> The from LED light indicates when the DL is connected, and blinks when a value is stored in the memory card (this can vary according to the script uploaded)

##### 3.2.2 samples_to_take()
This function should contain all sampling functions, and is executed many times. The exact execution times depend on the __storage_interval_s__ and the execution time of its content. The following standard functions are defined.

- __diffx.get_single_sample()__
  Is executed independently for each differential channel.
  It take a single sample and store it to the measurements memory.

- __diffx.get_continuous_sample(int cycles, int frequency)__
  Is executed independently for each differential channel.
  It take continuous samples and store them in the measurements memory.
  This function is the standard used to reduce noise, as it average multiple continuous samples. The amount of samples is defined by its parameters:
  __cycles__ means ac signal cycles (default 13)
  __frequency__ means the grid frequency (default 50hz)
  if cycles and frequency are not given, the default values are used.
    

- __phasex.get_continuous_sample(int cycles, int frequency)__
  Each power phase has a voltage and a current differential channel.
  To take correct measurements each channel must be in a different ADC card.
  The standard configuration is: 
  phase1(I_channel = diff1, V_channel = diff4)
  phase2(I_channel = diff2, V_channel = diff5)
  phase3(I_channel = diff3, V_channel = diff6)
  
  The differential channels for each phase can be changed if required, for example ig multiple current measurements are being taken from the same voltage line. To do it, you could insert in setup() after init_dl(): "phase2.V_channel = &diff4;"
__cycles__ and __frequency__ parameters work exactly as for _get_continuous_sample_.

- __phasex_get_n_continuous_sample(byte n, int *Vout, int *Iout)__
  Method on test, due to memory limits in the datalogger. It stores the value of each voltage and current measurement, and send it to log. It allows to see the shape of AC signal, with a maximum of 860 samples per second. It require 2 arrays to return samples, one for Vout and one for Iout. Due to arduino Uno limitations,  the maximum array size is 40, which is ca. 2 AC cycles

> :warning: ATTENTION :warning:
> The sampling results are stores in a memory given to each channel. Therefore it is only allowed to execute one function per channel. If there are phase measurements, the corresponding differential channels must be sampled with phase functions and not independently.




##### 3.2.3 to_log_content()

This function should contain all data to store in the log.
Calculations can be done with adhoc variables and results before storing them.
To effectivelly add data to the log, it should be inside the __add_to_log__ function, as below:

> add_to_log(F("[LABEL]"), F("[UNIT]"), ([VARIABLE], [decimals]);
Example:
> add_to_log(F("I1rms"), F("A"), phase1.calculate_Irms(),3);  

- [LABEL] and [UNIT] specify the stored content and will be used to generate the column headers with the format "LABEL[UNIT]".
- [VARIABLE] can be any required calculation or parameter. Both differential channels and phases have specific values and functions that can be stored as variables. (For raw values see the parameters from classes DIFFchannel and AC_phase)

For each differential channel:
 - diffx.mean_sample_unit()
 - diffx.max_sample_unit()
 - diffx.min_sample_unit()
 - diffx.rms_sample_unit()
(unit mean that is calculated after applying multiplication factor and offset)

For AC_phases:
 - phasex.calculate_Preal()
 - phasex.calculate_Pmean()
 - phasex.calculate_Prms()
 - phasex.calculate_PF()
 - phasex.calculate_f_Hz()
 - phasex.calculate_Vmean()
 - phasex.calculate_Vmax()
 - phasex.calculate_Vmin()
 - phasex.calculate_Vrms()
 - phasex.calculate_Imean()
 - phasex.calculate_Imax()
 - phasex.calculate_Imin()
 - phasex.calculate_Irms()

[decimals] is the ammount of decimals to be used to store variable values.

> :warning: ATTENTION :warning:
> When storing values, pay attention that all the information is correct!

#### 3.3 Firmware Interaction
##### 3.3.1 Notes
You can introduce notes in the datalogging file by sending via serial communication a text.
To be recognized as note, it must start with "N:" and end with an enter ("\n").
The note will be added to the log as:
I<NOTE:note content >
