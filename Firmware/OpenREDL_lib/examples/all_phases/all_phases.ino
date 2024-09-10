//before committing, test:
//- if sample max and min are working correctly for diff channels (changed from float to int)
//- if size of channels objects can still be reduced, to add a 3rd phase

#define FIRMWARE_DESCRIPTION "Continuous sampling of 3 AC phases on 6 differential channels, "\
                             "storing at 10 seconds interval."

#include "datalogger_config.h"


//Setup function runs one time at the microporocessor start-up
void setup() {
  
  format_log = TIMESERIES; // TIMESERIES or JSON
  storage_interval_s = 10; // Default 10 (s) 

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

//diff1.gain = ADS1115_PGA_6P144;
//diff2.gain = ADS1115_PGA_6P144;
//diff3.gain = ADS1115_PGA_6P144;
//diff4.gain = ADS1115_PGA_6P144;
//diff5.gain = ADS1115_PGA_6P144;
//diff6.gain = ADS1115_PGA_6P144;

//#### Multiplication factor (Sensor calibration factor) ####
//todo: fix not working properly if not decimals
// Callibration for each input channel. Value_unit = (mult_factor x VoltageMeasured) + offset

//diff1.mult_factor = 1.;
//diff2.mult_factor = 1.;
//diff3.mult_factor = 1.;
//diff4.mult_factor = 1.;
//diff5.mult_factor = 1.;
//diff6.mult_factor = 1.;

//#### OFFSET (Offset to add, should be in the final calculation units e.g. °C, V. mV) ####
//diff1.offset_unit = 0.;
//diff2.offset_unit = 0.;
//diff3.offset_unit = 0.;
//diff4.offset_unit = 0.;
//diff5.offset_unit = 0.;
//diff6.offset_unit = 0.;
//##############################################################################

  init_dl(); // initialize the DL routines and parameters
 
}


//#############################################################################
// Samples to be taken in each loop
// Uncomment lines required to be logged.
//#############################################################################
void samples_to_take(){

// ##################################################
// Take continuous samples on AC Phases connected to differential channels
// IF ACTIVE, DIFFERENTIAL SAMPLES IN THEIR DIFF CHANNELS ARE NOT REQUIRED
// (as they are internally done)
// ##################################################
// [phase].get_continuous_sample([sample_window_cycles],[grid_frequency]);
    phase1.get_continuous_sample(25,50); // phase1(I_channel = diff1, V_channel = diff4)
    phase2.get_continuous_sample(25,50); // phase2(I_channel = diff2, V_channel = diff5)
    phase3.get_continuous_sample(); // phase3(I_channel = diff3, V_channel = diff6)

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
    // Log for phases measurements
//#############################################################################

    //# Phase1 ##################################################
    add_to_log(F("I1rms"), F("A"), phase1.calculate_Irms(),3);    
    add_to_log(F("V1rms"), F("V"), phase1.calculate_Vrms(),decimal_digits);
    add_to_log(F("P1rms"), F("W"), phase1.calculate_Prms(),decimal_digits);
    add_to_log(F("P1real"), F("W"), phase1.calculate_Preal(),decimal_digits);
    add_to_log(F("PF1"), F(""), phase1.calculate_PF(),decimal_digits);
    add_to_log(F("f1"), F("Hz"), phase1.calculate_f_Hz(),3);
    
    //add_to_log(F("I1mean"), F("mA"), phase1.calculate_Imean(),3);        
    //add_to_log(F("I1max"), F("A"), phase1.calculate_Imax(),3);    
    //add_to_log(F("I1min"), F("A"), phase1.calculate_Imin(),3);    
    //add_to_log(F("V1mean"), F("mV"), phase1.calculate_Vmean(),decimal_digits);
    //add_to_log(F("V1max"), F("V"), phase1.calculate_Vmax(),decimal_digits);
    //add_to_log(F("V1min"), F("V"), phase1.calculate_Vmin(),decimal_digits);
    //add_to_log(F("P1mean"), F("mW"), phase1.calculate_Pmean()/1000,3);

////
    //# Phase2 ##################################################
    add_to_log(F("I2rms"), F("A"), phase2.calculate_Irms(),3);    
    add_to_log(F("V2rms"), F("V"), phase2.calculate_Vrms(),decimal_digits);
    add_to_log(F("P2rms"), F("W"), phase2.calculate_Prms(),decimal_digits);
    add_to_log(F("P2real"), F("W"), phase2.calculate_Preal(),decimal_digits);
    add_to_log(F("PF2"), F(""), phase2.calculate_PF(),decimal_digits);
    add_to_log(F("f2"), F("Hz"), phase2.calculate_f_Hz(),3);

    //add_to_log(F("I2mean"), F("mA"), phase2.calculate_Imean(),3);        
    //add_to_log(F("I2max"), F("A"), phase2.calculate_Imax(),3);    
    //add_to_log(F("I2min"), F("A"), phase2.calculate_Imin(),3);    
    //add_to_log(F("V2mean"), F("mV"), phase2.calculate_Vmean(),decimal_digits);
    //add_to_log(F("V2max"), F("V"), phase2.calculate_Vmax(),decimal_digits);
    //add_to_log(F("V2min"), F("V"), phase2.calculate_Vmin(),decimal_digits);
    //add_to_log(F("P2mean"), F("mW"), phase2.calculate_Pmean()/1000,3);

    //# Phase3 ##################################################
    add_to_log(F("I3rms"), F("A"), phase3.calculate_Irms(),3);    
    add_to_log(F("V3rms"), F("V"), phase3.calculate_Vrms(),decimal_digits);
    add_to_log(F("P3rms"), F("W"), phase3.calculate_Prms(),decimal_digits);
    add_to_log(F("P3real"), F("W"), phase3.calculate_Preal(),decimal_digits);
    add_to_log(F("PF3"), F(""), phase3.calculate_PF(),decimal_digits);
    add_to_log(F("f3"), F("Hz"), phase3.calculate_f_Hz(),3);

    //add_to_log(F("I3mean"), F("mA"), phase3.calculate_Imean(),3);        
    //add_to_log(F("I3max"), F("A"), phase3.calculate_Imax(),3);    
    //add_to_log(F("I3min"), F("A"), phase3.calculate_Imin(),3);    
    //add_to_log(F("V3mean"), F("mV"), phase3.calculate_Vmean(),decimal_digits);
    //add_to_log(F("V3max"), F("V"), phase3.calculate_Vmax(),decimal_digits);
    //add_to_log(F("V3min"), F("V"), phase3.calculate_Vmin(),decimal_digits);
    //add_to_log(F("P3mean"), F("mW"), phase3.calculate_Pmean()/1000,3);

//#############################################################################

}
