//Author: AJ, University of Oldenburg - Energy Technology - Institute of Physics
//Last Update 2026 10 15

#define FIRMWARE_DESCRIPTION "Winter Lab - Battery Storage. "\
                             "Continuous sampling of 4 differential channels: "\
                             "DIFF1: V_1, "\
                             "DIFF2: I_1, "\
                             "DIFF3: V_2, "\
                             "DIFF4: V_3. "\
                             "Storing mean value at 2 seconds interval."

#include "datalogger_config.h"

//Setup function runs one time at the microporocessor start-up
void setup() {
  
  format_log = TIMESERIES; // TIMESERIES or JSON
  // storage_interval_s: Default 10 (s), minimum ~0.1(s)
  // below 1s has lower time precision.
  storage_interval_s = 2; 

//##############################################################################
// CONFIGURATION OF ANALOG CHANNELS
//##############################################################################
//For each differential channel you can change:
//diffx.gain: ADC meassuring range, defined by the sensor maximum possible output.
//Default ADS1115_PGA_2P048
//diffx.offset: float, an addig offset of the sensor. Default 0.
//diffx.mult_factor: FLOAT, the multiplication factor to convert measurements,
//from the sensor callibration. Default 1.
//diffx.units: STRING, describes the measurement units. Default "mV".

// The ADC input range (or gain) can be changed via the following
// options, but be careful never to exceed VDD +0.3V max
// (5V if using USB power), or to exceed the upper and lower limits
// if you adjust the input range!
// Setting these values incorrectly may destroy your ADC!
//Gain sets the range to Possible values to be measured. 4p096 meand a range of 4.096V
//------------------|----------|------------|-------------------
//       Gain       |          |    Range   |      Resolution      
//------------------|----------|------------|-------------------
//ADS1115_PGA_6P144 | 2/3x gain| +/- 6.144V | 1 bit = 0.1875mV (default)
//ADS1115_PGA_4P096 | 1x gain  | +/- 4.096V | 1 bit = 0.125mV
//ADS1115_PGA_2P048 | 2x gain  | +/- 2.048V | 1 bit = 0.0625mV
//ADS1115_PGA_1P024 | 4x gain  | +/- 1.024V | 1 bit = 0.03125mV
//ADS1115_PGA_0P512 | 8x gain  | +/- 0.512V | 1 bit = 0.015625mV
//ADS1115_PGA_0P256 | 16x gain | +/- 0.256V | 1 bit = 0.0078125mV
//------------------|----------|------------|-------------------
//#### GAIN (Range of measurements inputs) ####

diff1.gain = ADS1115_PGA_4P096;
diff2.gain = ADS1115_PGA_4P096;
diff3.gain = ADS1115_PGA_4P096;
diff4.gain = ADS1115_PGA_4P096;

//#### Multiplication factor (Sensor calibration factor) ####
// Callibration for each input channel. Value_unit = (mult_factor x VoltageMeasured) + offset

//diff1.mult_factor = 1.;
//diff2.mult_factor = 1.;
//diff3.mult_factor = 1.;
//diff4.mult_factor = 1.;

//#### OFFSET (Offset to add, should be in the final calculation units e.g. °C, V. mV) ####
//diff1.offset_unit = 0.;
//diff2.offset_unit = 0.;
//diff3.offset_unit = 0.;
//diff4.offset_unit = 0.;
//##############################################################################
  init_dl(); // initialize the DL routines and parameters
 }


//#############################################################################
// Samples to be taken in each loop
// Uncomment lines required to be logged.
//#############################################################################
void samples_to_take(){

// ##################################################
// Take continuous samples on the 6 diff channels
// NOT REQUIRED FOR CHANNELS BEING MEASSURED AS PHASES
// ##################################################
//    [channel].get_continuous_sample([sample_window_cycles],[grid_frequency]);
    diff1.get_continuous_sample();
    diff2.get_continuous_sample();
    diff3.get_continuous_sample();        
    diff4.get_continuous_sample();
//#############################################################################

}


//#############################################################################
// Parameters to store in the log: SD card and/or serial port.
// Uncomment lines required to be logged.
//#############################################################################
void to_log_content(){
  
// add_to_log(F("[LABEL]"), F("[UNIT]"), ([VARIABLE], [decimals]);
// All text must be inside F("")
    byte decimal_digits=2;

//#############################################################################
// Log for differential channels
//#############################################################################
    add_to_log(F("I_1"), F("mV"), diff1.mean_sample_unit(),decimal_digits);
    add_to_log(F("V_1"), F("mV"), diff2.mean_sample_unit(),decimal_digits);
    add_to_log(F("I_2"), F("mV"), diff3.mean_sample_unit(),decimal_digits);
    add_to_log(F("V_2"), F("mV"), diff4.mean_sample_unit(),decimal_digits);

}
