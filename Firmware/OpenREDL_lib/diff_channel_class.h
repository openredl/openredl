//#######################
// Load libraries
//#######################

#include "RTClib.h"
//#include "ADS1115.h" // Look details at https://www.i2cdevlib.com/docs/html/class_a_d_s1115.html#a98f2fcf1b4cb309ad41e12be7cc78f75
#include <EEPROM.h>

//#######################
// Declare ADC cards
//#######################
#ifndef ADS1115
#include "ADS1115.h"
#endif  

ADS1115 ads4A(0x48);
ADS1115 ads4B(0x49);
ADS1115 ads4C(0x4A);
//ADS1115 ads4A(0x48);
//ADS1115 ads4B(0x4A);
//ADS1115 ads4C(0x49);
//#######################

//#######################
// Declare SD File
//#######################
#ifndef SPI
#include <SPI.h>
#endif 
#ifndef SD
#include <SD.h>
#endif 
File myFile;
//#######################

//#ifndef diff_channel_class
//#define diff_channel_class

//#######################
// Declare constants
//#######################

const char start_message[] PROGMEM = {"I<Initializing Arduino>\n"};
const char firmware_version[] PROGMEM = {"I<firmware version: v1.15>\n"};
#ifndef FIRMWARE_DESCRIPTION
const char firmware_description[] PROGMEM = {"Continuous sampling, storing at the given interval. With differential channel and phases functions."};
#else
const char firmware_description[] PROGMEM = {FIRMWARE_DESCRIPTION};
#endif
//const char hardware_requirements[] PROGMEM = {"I<hardware_requirements: 2xADS1115, RTC and SD Card.>\n"};
const PROGMEM int LED_pin=9;
const PROGMEM int infoLED_pin=8;
const PROGMEM int SD_pin = 10;
const PROGMEM byte full=255;
const PROGMEM byte mid=40;
const PROGMEM byte off=0;
byte LED_bright = 255;
//#######################


//#######################
// Declare variables
//#######################
//float result=0; 
long meassurements_counter=0;// count the amount of meassurements taken
unsigned long millis_loop_start=0;
unsigned long millis_last_stored=0;
long previousTime=0;
//int count_store=0;
RTC_DS1307 rtc;
bool SD_connected=0;
bool RTC_connected=0;
bool stored_in_SD = 0;
char dt[14]; //to concatenate date
char tm[14]; //to concatenate time for sampling time
//char tm2[14]; //to concatenate time to store in SD card
long s_start_last_sample=0;//Marker for last stored time
long last_stored_s=0;//Marker for last stored time
long current_stored_s=0;//Marker for last stored time
unsigned long millis_last_update_sampling_start_time = 0;
DateTime datetime_now;
unsigned long millis_start_last_sample;
unsigned long millis_last_second_update;
//#######################



int count_sampling_rounds=0; // sampling rounds done in the current period


int signOfX(float x){ return (x > 0) - (x < 0);}

byte ch_diff1 = 1;
byte ch_diff2 = 2;
byte ch_diff3 = 3;
byte ch_diff4 = 4;
byte ch_diff5 = 5;
byte ch_diff6 = 6;

//##################################
//Mapping of channels to get samples
//##################################
int16_t get_sample_map(byte channel, uint8_t gain){
    if (channel == ch_diff1){
        //ads4A.setGain(gain);
        return ads4A.getConversionP0N1();
    }
    else if (channel == ch_diff2){
        //ads4A.setGain(gain);
        return ads4A.getConversionP2N3();
    }
    else if (channel == ch_diff3){
        //ads4B.setGain(gain);
        return ads4B.getConversionP0N1();
    }
    else if (channel == ch_diff4){
        //ads4B.setGain(gain);
        return ads4B.getConversionP2N3();
    }
    else if (channel == ch_diff5){
        //ads4A.setGain(gain);
        return ads4C.getConversionP0N1();
    }
    else if (channel == ch_diff6){
        //ads4A.setGain(gain);
        return ads4C.getConversionP2N3();
    }
        
}

//void get_sample_map(String channel){
//    if (channel == "DIFF1"){
//        //ads4A.setGain(gain);
//        return ads4A.getConversionP0N1();
//    }
//    else if (channel == "DIFF2"){
//        //ads4A.setGain(gain);
//        return ads4A.getConversionP2N3();
//    }
//    else if (channel == "DIFF3"){
//        //ads4B.setGain(gain);
//        return ads4B.getConversionP0N1();
//    }
//    else if (channel == "DIFF4"){
//        //ads4B.setGain(gain);
//        return ads4B.getConversionP2N3();
//    }
//    else if (channel == "d1"){
//        pinMode(LED_pin, OUTPUT);
//        digitalWrite(LED_pin, HIGH);
//      
//        return ;
//    }

//}

//#######################################
// GPIO configuration, for DL23 GBE
//#######################################
//byte d_ch1 = 3;
//byte d_ch2 = 5;
//byte d_ch3 = 6;
//byte d_ch4 = 14;
//byte d_ch5 = 15;
byte d_ch1 = 3;
byte d_ch2 = 4;
byte d_ch3 = 5;
byte d_ch4 = 14;
byte d_ch5 = 15;
//#######################################
//Channel instructions that must be executed in the setup method
//#######################################



//#######################################
//Mapping of channels preparation before sampling
//#######################################
void prepare_channel_map(byte channel, uint8_t gain){
    if (channel == ch_diff1){
        if (ads4A.getGain() != gain) {ads4A.setGain(gain);}
        return;
    }
    else if (channel == ch_diff2){
        if (ads4A.getGain() != gain) {ads4A.setGain(gain);}
        return;
    }
    else if (channel == ch_diff3){
        if (ads4B.getGain() != gain) {ads4B.setGain(gain);}
        return;
    }
    else if (channel == ch_diff4){
        if (ads4B.getGain() != gain) {ads4B.setGain(gain);}
        return;
    }
    else if (channel == ch_diff5){
        if (ads4C.getGain() != gain) {ads4C.setGain(gain);}
        return;
    }
    else if (channel == ch_diff6){
        if (ads4C.getGain() != gain) {ads4C.setGain(gain);}
        return;
    }
}


//#######################################
//Mapping of channels to get mV per count
//#######################################
float get_mVperCount_map(byte channel, uint8_t gain){
    if (channel == ch_diff1){
        ads4A.setGain(gain);
        return ads4A.getMvPerCount();
    }
    else if (channel == ch_diff2){
        ads4A.setGain(gain);
        return ads4A.getMvPerCount();
    }
    else if (channel == ch_diff3){
        ads4B.setGain(gain);
        return ads4B.getMvPerCount();
    }
    else if (channel == ch_diff4){
        ads4B.setGain(gain);
        return ads4B.getMvPerCount();
    }
    else if (channel == ch_diff5){
        ads4C.setGain(gain);
        return ads4C.getMvPerCount();
    }
    else if (channel == ch_diff6){
        ads4C.setGain(gain);
        return ads4C.getMvPerCount();
    }
    
}


//byte _mV = 1;
//byte _V = 2;
//byte _mA = 3;
//byte _A = 4;
//byte _mW = 5;
//byte _W = 6;
//
//String units_map(byte _units){
//    switch(_units){
//        case 1: return "mV";
//        case 2: return "V";
//        case 3: return "mA";
//        case 4: return "A";        
//        case 5: return "mW";
//        case 6: return "W";
////        case 7: return "mA";
////        case 8: return "A";
//    }
//    return "";
//}

   

class DIFFchannel{
    public:
        uint8_t gain;
        float offset_unit;
        ADS1115 board;
        //String adc_pins;
        float mult_factor;
        byte channel;
        //byte units;
        float mVPerCount;
        int sample_count;
        float sample_sum;
        //long sum_store;
        int sample_last;
        int sample_min;
        int sample_max;
        float sample_sqsum;
        int sqsum_divider;
        int change_of_sign;
        //int sample_list[5];
        
        
        DIFFchannel(
            byte ch = 1,
            uint8_t ga = ADS1115_PGA_6P144,
            float of = 0,
            float fa = 1
            //byte un = _mV
            ){
            channel = ch;
            gain = ga;
            offset_unit = of;
            mult_factor = fa;
            //units = un;
            mVPerCount = 0;
            sample_count = 0;
            sample_sum = 0;
            //sum_store = 0;
            sample_min = 0;
            sample_max = 0;
            sample_sqsum = 0;
            sqsum_divider = 1000;
            change_of_sign = 0;
            
            }


            //############################
            //Get sample using the mapping
            //############################
            int get_sample(){
                return get_sample_map(channel, gain);
            }
            
            float add_sample(int new_sample){                


                //cleaning for noise
                if (abs(new_sample) < 3) new_sample = 0;
                
                if (mVPerCount == 0) mVPerCount = get_mVperCount_map(channel, gain);

                int sign_of_prev=0;
                if (sample_last != 0) sign_of_prev = signOfX(sample_last);
                sample_last = new_sample;
                if (sample_count == 0){
                    sample_max = sample_last;
                    sample_min = sample_last;
                }
                else{
                    sample_max = max(sample_last, sample_max);
                    sample_min = min(sample_last, sample_min);
                }
                sample_sqsum += sq(float(sample_last)/float(sqsum_divider));
                               
                if (sign_of_prev != 0 && sign_of_prev != signOfX(sample_last)){
                    change_of_sign++; 
                }
                sample_sum += float(sample_last);
                sample_count += 1;
                return sample_last;
            }

            float get_single_sample(){
                prepare_channel_map(channel, gain);    
                long new_sample = get_sample();
                return add_sample(new_sample);
            }

        void get_continuous_sample(int cycles, int frequency){

            prepare_channel_map(channel, gain);
            float sample;
            unsigned long deltat_measurement_us = 1000000*cycles/frequency; //
            //bool flag=0;
            //bool is_firstsample=1;
            //float tstart_firstcycle_s;
            //float tstart_lastcycle_s;
            unsigned long micros_lastread = 0;

            //getting sample once to reduce error in first sample
            sample = get_sample();
            unsigned long time_sampling_start_us=micros();

            while ((micros() - time_sampling_start_us < deltat_measurement_us)){

                if (micros() >= (micros_lastread + 1000000 / 860)) {
                    micros_lastread = micros();
                   
                    sample = get_sample();
                    add_sample(sample);
                    //if (flag == 0) flag = 1;
                  

                }
            }
        }

        // Default continuous sample call, with cycles=13, frequency=50
        void get_continuous_sample(){
             get_continuous_sample(13,50);
        }


        float mean_sample_unit() {return sample_sum*mVPerCount/float(sample_count*1.0)*mult_factor + offset_unit;}
        //float max_sample_unit() {return float(sample_max)*float(mVPerCount)/float(sample_count)*float(mult_factor) + offset_unit;}
        float max_sample_unit() {return float(sample_max*1.0)*(mVPerCount)*float(mult_factor) + (offset_unit);}
        float min_sample_unit() {return float(sample_min*1.0)*mVPerCount*float(mult_factor) + offset_unit;}
        float rms_sample_unit() {return sqrt((float(sample_sqsum)/float(sample_count)))*float(sqsum_divider)*mVPerCount * float(mult_factor) + offset_unit; }
        
//        String get_units(){units_map(units);}
        
        void reset_sample(){
            sample_sum = 0;
            sample_count = 0;
            sample_min = 0;
            sample_max = 0;
            sample_sqsum = 0;
            change_of_sign = 0;

            //sum_store = 0;
          
        }     
     
};


class AC_phase{
    public:
        DIFFchannel *I_channel;
        DIFFchannel *V_channel;
        float realpower_value;
        float realpower_sum;
        float realpower_min;
        float realpower_max;
        float realpower_count;
        float sampling_time_sum_s;
        float sum_deltat_first_last_cycle_s;
        int change_of_sign;


        AC_phase (DIFFchannel *I_ch, DIFFchannel *V_ch){
          I_channel = I_ch;
          V_channel = V_ch;
          change_of_sign=0;
        }


        void get_continuous_sample(int cycles, int frequency){
          //TODO: Test
            prepare_channel_map((*V_channel).channel, (*V_channel).gain);
            prepare_channel_map((*I_channel).channel, (*I_channel).gain);
            float sample_I;
            float sample_V;
            unsigned long deltat_measurement_us = 1000000*cycles/frequency; //

            bool flag=0;
            bool is_firstsample=1;
//            float tstart_firstcycle_s;
//            float tstart_lastcycle_s;
            unsigned long tstart_firstcycle_us;
            unsigned long tstart_lastcycle_us;
            unsigned long micros_lastread = 0;

            //getting sample once to reduce error in first sample
            sample_I = (*I_channel).get_sample();
            sample_V = (*V_channel).get_sample();
            unsigned long time_sampling_start_us=micros();

            while ((micros() - time_sampling_start_us < deltat_measurement_us)){

                if (micros() >= (micros_lastread + 1000000 / 860)) {
                    micros_lastread = micros();
                    //delayMicroseconds(1000000/860/2);
                    int sign_of_prev = signOfX((*V_channel).sample_last);
                    
                    sample_I = (*I_channel).get_sample();
                    sample_V = (*V_channel).get_sample();
    
                    (*I_channel).add_sample(sample_I);
                    (*V_channel).add_sample(sample_V);
    
                    if (!is_firstsample && sign_of_prev != signOfX(sample_V)){
    
                        change_of_sign += 1;
//                        if (flag == 0) {flag = 1;tstart_firstcycle_s = float(micros())/1000000;}
                        if (flag == 0) {flag = 1;tstart_firstcycle_us = micros();}
                        else {
                            tstart_lastcycle_us = micros();
                            
                        }
                    }
                    is_firstsample = 0;
        
                    realpower_value = sample_I * sample_V;
                    realpower_sum += realpower_value;
                    realpower_min = max(realpower_value, realpower_min);
                    realpower_max = max(realpower_value, realpower_max);
                    realpower_count += 1;            
                }
            }
            float time_sampling_end_s=float(micros())/1000000;
            sum_deltat_first_last_cycle_s += float(tstart_lastcycle_us - tstart_firstcycle_us)/1e6;

            sampling_time_sum_s += (time_sampling_end_s-float(time_sampling_start_us)/1000000);

        }


      void get_continuous_sample(){
          get_continuous_sample(18,50);
        }


        void get_n_continuous_sample(byte n, int *Vout, int *Iout){
          //TODO: Test
            prepare_channel_map((*V_channel).channel, (*V_channel).gain);
            prepare_channel_map((*I_channel).channel, (*I_channel).gain);
            int sample_I;
            int sample_V;
            int cycles = 100;
            int frequency =50;
            unsigned long deltat_measurement_us = 1000000*cycles/frequency; //

            bool flag=0;
            bool is_firstsample=1;
            float tstart_firstcycle_s;
            float tstart_lastcycle_s;
            unsigned long micros_lastread = 0;
            byte count = 0;

            //getting sample once to reduce error in first sample
            sample_I = (*I_channel).get_sample();
            sample_V = (*V_channel).get_sample();            
            unsigned long time_sampling_start_us=micros();

            while ((micros() - time_sampling_start_us < deltat_measurement_us)){

                if (micros() >= (micros_lastread + 1000000 / 860)) {
                    micros_lastread = micros();
                    //delayMicroseconds(1000000/860/2);
                    int sign_of_prev = signOfX((*V_channel).sample_last);
                    
                    sample_I = (*I_channel).get_sample();
                    sample_V = (*V_channel).get_sample();
    
                    (*I_channel).add_sample(sample_I);
                    (*V_channel).add_sample(sample_V);
    
                    if (!is_firstsample && sign_of_prev != signOfX(sample_V)){
    
                        change_of_sign += 1;
                        if (flag == 0) {flag = 1;tstart_firstcycle_s = float(micros())/1000000;}
                        else {
                            tstart_lastcycle_s = float(micros())/1000000;
                            
                        }
                    }
                    is_firstsample = 0;
        
                    realpower_value = sample_I * sample_V;
                    realpower_sum += realpower_value;
                    realpower_min = max(realpower_value, realpower_min);
                    realpower_max = max(realpower_value, realpower_max);
                    realpower_count += 1;  

                    Vout[count] = sample_V;
                    Iout[count] = sample_I;                    
                    count++;
                    if (count >= n) return;
                              
                }
            }
            float time_sampling_end_s=float(micros())/1000000;
            sum_deltat_first_last_cycle_s += tstart_lastcycle_s - tstart_firstcycle_s;

            sampling_time_sum_s += (time_sampling_end_s-float(time_sampling_start_us)/1000000);

        }


       void reset_sample(){
             //I_channel.reset_sample();
             //V_channel.reset_sample();
           
             realpower_value = 0;
             realpower_sum = 0;
             realpower_count = 0;
             realpower_min = 0;
             realpower_max = 0;
             sampling_time_sum_s = 0;
             sum_deltat_first_last_cycle_s = 0;
             change_of_sign = 0;

        }  


        float calculate_Preal(){
            return (realpower_sum/realpower_count) *
                        (*I_channel).mVPerCount * (*I_channel).mult_factor *
                        (*V_channel).mVPerCount * (*V_channel).mult_factor;  
        }

        float calculate_Pmean(){
            return calculate_Imean()*calculate_Vmean();
        }

        float calculate_Prms(){
            return (*I_channel).rms_sample_unit() * (*V_channel).rms_sample_unit();  
        }        

        float calculate_PF(){
            return calculate_Preal() / calculate_Prms();  
        }        

//        float calculate_freq_hz(int num){
//          if (num == 0) return float((*I_channel).change_of_sign)/2/sampling_time_sum_s;
//          else if (num == 1) return float((*V_channel).change_of_sign)/2/sampling_time_sum_s;
//          }

//        float f2(){
//            return ((*V_channel).cycles-count_sampling_rounds*1.0)/2/sum_deltat_first_last_cycle_s;
//        }        


        //## Frequency calculation with correction from doing multiple sampling rounds
        float calculate_f_Hz(){
            return (float(change_of_sign)-count_sampling_rounds*1.0)/2/sum_deltat_first_last_cycle_s;
        }        


        // Voltage methods, redirecting to V channel methods
        float calculate_Vmean() {return (*V_channel).mean_sample_unit();}
        float calculate_Vmax() {return (*V_channel).max_sample_unit();}
        float calculate_Vmin() {return (*V_channel).min_sample_unit();}
        float calculate_Vrms() {return (*V_channel).rms_sample_unit();}

        // Current methods, redirecting to I channel methods
        float calculate_Imean() {return (*I_channel).mean_sample_unit();}
        float calculate_Imax() {return (*I_channel).max_sample_unit();}
        float calculate_Imin() {return (*I_channel).min_sample_unit();}
        float calculate_Irms() {return (*I_channel).rms_sample_unit();}
        
};

int log_serial = 1;
int log_file = 2;
int select_log;

template <typename T>
void add_to_log(T message){
    if (select_log==log_serial) {Serial.print(message); }//Serial.print(F(","));}
    else if (select_log==log_file) {myFile.print(message);}// myFile.print(F(","));
}

//template <typename T>
//void add_to_log(T message, int n){
//    if (select_log==log_serial) {Serial.print(message);} //Serial.print(F(","));} // Log to serial port
//    else if (select_log==log_file) {myFile.print(message);}// myFile.print(F(","));} // Log to file in SD Card
//    }

//void add_to_log(String name, String unit){
//    if (select_log==log_serial) {Serial.print(F(",")); Serial.print(name); Serial.print(F("[")); Serial.print(unit); Serial.print(F("]"));}
//    else if (select_log==log_file) {myFile.print(F(",")); myFile.print(name); myFile.print(F("[")); myFile.print(unit); myFile.print(F("]"));}
//    }

//template <typename T>
//void add_to_log(String name, String unit, T message){
//    if (select_log==log_serial) {Serial.print(F(",")); Serial.print(name); Serial.print(F("[")); Serial.print(unit); Serial.print(F("]:"));Serial.print(message);}
//    else if (select_log==log_file)  {myFile.print(F(",")); myFile.print(name); myFile.print(F("[")); myFile.print(unit); myFile.print(F("]:"));myFile.print(message);}
//}

// template <typename T>
// void add_to_log(String name, String unit, T message, int n){
//     if (select_log==log_serial) {Serial.print(F(",")); Serial.print(name); Serial.print(F("[")); Serial.print(unit); Serial.print(F("]:"));Serial.print(message, n);}
//     else if (select_log==log_file)  {myFile.print(F(",")); myFile.print(name); myFile.print(F("[")); myFile.print(unit); myFile.print(F("]:"));myFile.print(message, n);}
// }

int TIMESERIES = 0;
int TIMESERIES_HEADERS = 100;
byte JSON = 1;
byte json = 1;
byte Json = 1;
int format_log = TIMESERIES;
template <typename T>
void add_to_log(String name, String unit, T message, int n){
    if (format_log ==JSON){
        if (select_log==log_serial) {
            Serial.print(F(",\""));
            Serial.print(name);
            Serial.print(F("\":{\"value\":\""));
            Serial.print(message, n);
            Serial.print(F("\",\"unit\":\""));
            Serial.print(unit);
            Serial.print(F("\"}"));            
        }
        else if (select_log==log_file)  {
            myFile.print(F(",\""));
            myFile.print(name);
            myFile.print(F("\":{\"value\":\""));
            myFile.print(message, n);
            myFile.print(F("\", \"unit\":\""));
            myFile.print(unit);
            myFile.print(F("\"}")); 

            // myFile.print(F(",")); myFile.print(name);
            // myFile.print(F("[")); myFile.print(unit);
            // myFile.print(F("]:"));myFile.print(message, n);
        }
    }

    else if (format_log == TIMESERIES_HEADERS){ //Print timeseries headers
        if (select_log==log_serial) {
            Serial.print(name); Serial.print(F("[")); Serial.print(unit); Serial.print(F("],"));
        }
        else if (select_log==log_file)  {
            myFile.print(name); myFile.print(F("[")); myFile.print(unit); myFile.print(F("],")); 
        }
    }


    else { // Default: (format_log == TIMESERIES) or any other
        if (select_log==log_serial) {
            Serial.print(message, n); Serial.print(",");
            }
        else if (select_log==log_file)  {
            myFile.print(message, n); myFile.print(",");
        }
    }
}


//byte n = 30;
//float I_meas[n];
////float V_shift_meas[n];
//float V_meas[n];
//
//
//phase1.get_n_continuous_sample(n, I_meas, V_meas);
//select_log=1;
//add_to_log("\n"); 
//for( int i=0; i<n; i++){
//    add_to_log(F("V_meas"), F("count"), V_meas[i], 0);
////    add_to_log(F("V_shift_meas"), F("count"), V_shift_meas[i], 0);
//    add_to_log(F("I_meas"), F("count"), I_meas[i], 0);
//    add_to_
//
//

float PT100 = 100.;
float PT1000 = 1000.;

float calculate_Pt_RtoC(float R,float R0) {
    //Get Temperature in celcius from:
    // ONLY VALID FOR T > 0 °C
    // (IEC/Callendar–Van Dusen): R(T) = R0 [1 + A·T + B·T^2 + C·(T − 100)·T^3]
    // For T ≥ 0°C the C term is zero and it reduces to a quadratic
    //    R: Measured resistance in Ohms
    //    R0: Platinium Sensor Reference resistance in Ohms at 0°C
    // Discarding C factor. Error for-100C->0.222C, for 100C->0.015C.
    float A=3.9083E-3; 
    float B=-5.775E-7; 
    return ((-A + sqrt((A*A) - 4.0 * B * (1.0 - R/R0))) / (2.0 * B));
}
