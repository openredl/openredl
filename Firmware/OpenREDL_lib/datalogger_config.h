#ifndef diff_channel_class
#include "diff_channel_class.h"
#endif

#ifndef datalogger_config
#define datalogger_config
#endif


//-----------------------------------------------------------
// CHANNELS CONFIGURATION
// Define the parameters for each channel
//-----------------------------------------------------------
DIFFchannel diff1(
    ch_diff1,            //String channel = "DIFF1";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

DIFFchannel diff2(
    ch_diff2,            //String channel = "DIFF2";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

DIFFchannel diff3(
    ch_diff3,            //String channel = "DIFF3";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

DIFFchannel diff4(
    ch_diff4,            //String channel = "DIFF4";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

DIFFchannel diff5(
    ch_diff5,            //String channel = "DIFF4";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

DIFFchannel diff6(
    ch_diff6,            //String channel = "DIFF4";
    ADS1115_PGA_6P144,  //uint8_t gain = ADS1115_PGA_6P144;
    0.,                  //float offset = 0;
    1.                  //float mult_factor = 1;
    //_mV                //String units = "mV"
);

//-----------------------------------------------------------
// Parameters Setup
//-----------------------------------------------------------
//Rate = Samplings per second. Lower rate reduce noise, but reduce the amount of measurements per second.
//    ADS1115_RATE_8 
//    ADS1115_RATE_16 
//    ADS1115_RATE_32 
//    ADS1115_RATE_64 
//    ADS1115_RATE_128 
//    ADS1115_RATE_250 
//    ADS1115_RATE_475 
//    ADS1115_RATE_860 
const PROGMEM uint8_t ads_rate = ADS1115_RATE_860;
//const int sampling_interval_ms = 1000;

// storage_interval_s : how often should the data be sent to logs
// 0 for no data storage, min ~0.1s (depending on data sampled and stored)
float storage_interval_s = 10;

//String file_n_prefix = "dl_data";
String file_n = "dl_data.txt";

AC_phase phase1(&diff1,&diff4); // (I_channel, V_channel) - their offset and multiplication factor must deliver a value in V and A.
AC_phase phase2(&diff2,&diff5); // (I_channel, V_channel) - their offset and multiplication factor must deliver a value in V and A.
AC_phase phase3(&diff3,&diff6); // (I_channel, V_channel) - their offset and multiplication factor must deliver a value in V and A.


void update_datetime_text(){
    //Update date and time text for logs
    sprintf(dt, "%02d/%02d/%02d ", datetime_now.year(),datetime_now.month(),datetime_now.day());
    sprintf(tm, "%02d:%02d:%02d", datetime_now.hour(),datetime_now.minute(),datetime_now.second());
}


void update_sampling_start_time(){
    // Give interval index : from RTC or counter
    //################## 
//    Serial.print("[debug]Starting update_sampling_time. RTC = ");
//    Serial.println(RTC_connected);
    millis_start_last_sample = millis();       
    if (RTC_connected==1){
        Wire.setClock(200000); //speed reduction is required to work in GBE23 equipment
      // get Datetime from rtc. It is more precise than millis, which can have some minutes of error a day.
        int datetime_prev = datetime_now.second();
        datetime_now = rtc.now();

        // update millis if we are in a new second?
        if (datetime_prev != datetime_now.second())
            millis_last_second_update = millis();
        
        s_start_last_sample = datetime_now.unixtime();
        ///long unixtime_last_loop = 
        Wire.setClock(400000); //speed increase

    }       

// update file to store
// String update_filename(String file_n){
//       char date[12];
//       return String(dt).replace("/", "") + file_n;
}



int readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}


void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}


void serial_print_progmem(const char txt[]){
    char temp_Char;
    for (byte k = 0; k < strlen_P(txt); k++) {
        temp_Char = pgm_read_byte_near(txt + k);
        Serial.print(temp_Char);
    }
}


void myfile_print_progmem(char txt[]){
   char temp_Char;
   for (byte k = 0; k < strlen_P(txt); k++) {
       temp_Char = pgm_read_byte_near(txt + k);
       myFile.print(temp_Char);


   }}




void to_log_content();
//void print_log(int i);
void samples_to_take();


void headers_to_log(int log_num){
     if (format_log == TIMESERIES){
        format_log = TIMESERIES_HEADERS;
        select_log = log_num;
        add_to_log(F("H,datetime,"));
        to_log_content();
        add_to_log(F("notes>\n"));
        format_log = TIMESERIES;
     }
}


void init_SDcard(){
  // initialize SD card.
    if (!SD.begin(SD_pin)) {
        Serial.println(F("[ALERT] SD Card not detected. Data will not be stored."));
        SD_connected=0;
        LED_bright = mid;
    }    
    else {
        Serial.println(F("I<SD Card initialized correctly.>"));
        SD_connected=1;
        LED_bright = full;
        myFile = SD.open(file_n, FILE_WRITE);
        if (!myFile) Serial.println(F("[ALERT] SD Card file initialization failed."));
        else Serial.println(F("I<SD Card file initialized correctly.>"));
        select_log = log_file;
        add_to_log(F("I<Starting Meassurement>\n"));
        headers_to_log(log_file);
        myFile.close();   
        //Serial.println("([debug] SD Card and file initialized correctly."); 
      }
}


boolean update_filename(){
    //Update date and time text for logs
    String prev_file_n = file_n;
    char n_buf[21];
    sprintf(n_buf, "m%02d%02d%02d.txt", datetime_now.year()-2000,datetime_now.month(),datetime_now.day());
    //sprintf(n_buf, "m%02d%02d%02d.txt", datetime_now.year()-2000,datetime_now.month(),datetime_now.minute());    
    file_n = String(n_buf);
    //Serial.println(file_n);
//    if (prev_file_n != file_n && !millis()<1000){
//        myFile = SD.open(file_n, FILE_WRITE);
//        if (!myFile) Serial.println(F("[ALERT] SD Card file initialization failed."));
//        headers_to_log(log_file);
//        myFile.close();
//    }
    if (prev_file_n != file_n) return true;
    else return false;

}


void to_log_start(){
    // add_to_log(F("M>"));//Meassurement line indicator
    // add_to_log(dt);
    // add_to_log(tm);
    if (format_log ==JSON){
        add_to_log(F("M,{\"datetime\":\""));//Meassurement line indicator
        add_to_log(dt);
        add_to_log(tm);
        
        // add ms to time if storage_interval_s<1
        if (storage_interval_s<1){
            unsigned long lapse = millis()-millis_last_second_update;
            add_to_log(".");
            if (lapse<100) add_to_log(0);
            else if (lapse>999) lapse = 999;
            add_to_log(lapse);
            //add_to_log("\"");
            }        
    }
    else{ // TIMESERIES FORMAT as default for any other value
        add_to_log(F("M,"));//Meassurement line indicator
        add_to_log(dt);
        add_to_log(tm);

        // add ms to time if storage_interval_s<1
        if (storage_interval_s<1){
            unsigned long lapse = millis()-millis_last_second_update;
            add_to_log(".");
            if (lapse<100) add_to_log(0);
            else if (lapse>999) lapse = 999;
            add_to_log(lapse);
            //add_to_log("\"");
            }        
        add_to_log(F(","));
    }

}


void to_log_end(){
    // add_to_log(F(","));        
    //add_to_log(count_sampling_rounds);
    // add_to_log(F(","));
    if (select_log == 1){
       if (format_log == JSON){
          if (stored_in_SD) add_to_log(F(", \"note\":\"Stored in SD\""));
          else add_to_log(F(", \"note\":\"NOT stored in SD\""));
       }
       else{
          if (stored_in_SD) add_to_log(F("Stored in SD"));
          else add_to_log(F("NOT stored in SD"));
       }
    }

    if (format_log ==JSON) add_to_log(F("}"));//Meassurement line indicator

    add_to_log(F(">\n"));

}


void print_log(int log_num){
  // lognum=1 is Serial, lognum=2 is myfile in SD card
  
    select_log = log_num;
    to_log_start();
    to_log_content();
    to_log_end();
  }


// handre serial input instrucctions
void handle_serial_input(){
    String input = Serial.readStringUntil('\n');

    // store note receied for the log
    if (input.startsWith("N:")){
        Serial.print(F("I<NOTE: "));
        Serial.print(input.substring(2));
        Serial.print(F(">\n"));
        
        select_log = log_file;
        add_to_log(F("I<NOTE: "));
        add_to_log(input.substring(2, input.length()));
        add_to_log(F(">\n"));
    }
}


void send_log_to_serial(){
//    Serial.println((now.second()+60*now.minute())%(storage_interval_s));
    print_log(1);  
}


 void send_log_to_sdcard(){
 //    Serial.println("[debug]Starting to store in SD.");

    myFile = SD.open(file_n, FILE_WRITE);
    if (Serial.available()) handle_serial_input();


    // Serial.print(F("\n[debug] File_n exists in SD Card: "));
    // Serial.println(SD.exists(file_n));

    if (!myFile) {
        // Serial.println(F("[debug] Myfile is false"));
         Serial.println(F("[ALERT] Sample Not Stored, file in SD Card cannot be opened."));
         stored_in_SD = 0;
         init_SDcard();
    }

     else { 
//         Serial.println(F("[debug] Myfile is true")); 
        print_log(2);
        stored_in_SD = 1;
    }
    myFile.close();

}


void get_samples(){

    //Testing, to increase precision in sampling time for slow samples
    unsigned long mean_sampling_time = ((millis() - millis_last_stored)/
                                        count_sampling_rounds);
    // sample flag:
    // - Sample at least one time.
    // - do not sample if sampling would end after next storing time
    bool sample_flag = ((count_sampling_rounds==0) ||
                        (millis_last_stored + storage_interval_s * 1000 >= 
                         millis() + mean_sampling_time));
    //Serial.print(F("sample_flag: "));
    //Serial.println(sample_flag);
    //Serial.print(F("mean_sampling_time: "));
    //Serial.println(mean_sampling_time);
    
    if (sample_flag){
        samples_to_take();
        count_sampling_rounds += 1;
    }
}


void reset_measurements(){
        phase1.reset_sample();
        phase2.reset_sample();
        phase3.reset_sample();
        diff1.reset_sample();
        diff2.reset_sample();
        diff3.reset_sample();
        diff4.reset_sample();
        diff5.reset_sample();
        diff6.reset_sample();        
}


void init_dl() {
  pinMode(LED_pin, OUTPUT);
  pinMode(infoLED_pin, OUTPUT);

  Wire.begin();  // join I2C bus

  //#######################
  // Initialize Serial Port
  //#######################
  Serial.begin(115200);   // Open serial communications and wait for port to open:
//    Serial.begin(500000);   // Open serial communications and wait for port to open:
  delay(100);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
  }

  //#######################
  // Send initial info
  //#######################
  Serial.println();
  serial_print_progmem(start_message);
  serial_print_progmem(firmware_version);
  Serial.print(F("I<Firmware description: "));
  serial_print_progmem(firmware_description);
  Serial.println(F(">"));
  //serial_print_progmem(hardware_requirements); 
  //#######################

   
  //#######################
  // Initialize ADC boards
  //#######################
  ads4A.initialize(); // initialize ADS1115 16 bit A/D chip
  ads4B.initialize(); // initialize ADS1115 16 bit A/D chip
  ads4C.initialize(); // initialize ADS1115 16 bit A/D chip
  ads4A.setMode(ADS1115_MODE_CONTINUOUS);
  ads4B.setMode(ADS1115_MODE_CONTINUOUS);
  ads4C.setMode(ADS1115_MODE_CONTINUOUS);
  ads4A.setRate(ads_rate);
  ads4B.setRate(ads_rate);
  ads4C.setRate(ads_rate);
  //#######################

  //#######################
  // Initialize Real Time Clock
  //##################
  if (! rtc.begin()) {
      Serial.println(F("I<[ALERT] RTC was not found>"));
      RTC_connected=0;
  }   
  else{
      //serial_print_progmem(init_rtc_correct);
      Serial.print(F("I<Initializing RTC>"));
      RTC_connected=1;
      // Update RTC time if set time is newer.
      DateTime now = rtc.now();    
      DateTime a = DateTime(F(__DATE__), F(__TIME__)).unixtime()+12; //Adding offset to RTC to compensate for arduino compilation time.
      unsigned int a_sum = a.year() + a.hour() + a.minute() + a.second();

      byte addr = 100;    
      if (readIntFromEEPROM(addr) != a_sum ){
          rtc.adjust(a);
          writeIntIntoEEPROM(addr, a_sum);
          Serial.print(F("I<RTC time has been updated.> "));
      }
      else{
          Serial.print(F("I<RTC already had latest time.> "));
          }

      now = rtc.now();
      update_sampling_start_time();
      update_datetime_text();
      update_filename();

      //sprintf(dt, "%02d/%02d/%02d ", now.year(),now.month(),now.day());
      //sprintf(tm, "%02d:%02d:%02d", now.hour(),now.minute(),now.second());

      Serial.print(F("I<Current time: "));
      Serial.print(dt);
      Serial.print(tm);
      Serial.println(F(">"));
      }
  //#######################

  // MUST be after RTC declaration, or it get reduced and ADC meassurements get bottleneck.
  Wire.setClock(400000);


  //###################
  // Initialize SD card
  //###################
  init_SDcard();
//  if (!SD.begin(SD_pin)) {
//      Serial.println(F("[ALERT] SD Card not detected. Data will not be stored."));
//      SD_connected=0;
//  }    
//  else {
//      Serial.println(F("I<SD Card initialized correctly.>"));
//      SD_connected=1;
//      myFile = SD.open(file_n, FILE_WRITE);
//      if (!myFile) Serial.println(F("[ALERT] SD Card file initialization failed."));
//      else Serial.println(F("I<SD Card file initialized correctly.>"));
//      select_log = 2;
//      add_to_log(F("I<Starting Meassurement>\n"));
//      headers_to_log(log_file);
//      myFile.close();   
//      //Serial.println("([debug] SD Card and file initialized correctly."); 
//    }

  //##################
  //Printing titles and descriptions in serial output
  //###########g#######
  
  Serial.print(F("I<Starting Meassurement>"));
  Serial.println();
   
  headers_to_log(log_serial);
  
  //#####################
  // Initialize time count for the first loop
  //#####################
  update_sampling_start_time(); // Start time for the first sampling interval
  last_stored_s=s_start_last_sample;
  millis_last_stored = millis();
  //#####################
  
}

void digital_pin(){
//    Serial.print(F("I<Toggling\n>"));
    // dummy program that toggles the value
//    digitalWrite(d_ch1, !digitalRead(d_ch1));
//    digitalWrite(d_ch2, !digitalRead(d_ch2));
//    digitalWrite(d_ch3, !digitalRead(d_ch3));
//    digitalWrite(d_ch4, !digitalRead(d_ch4));
//    digitalWrite(d_ch5, !digitalRead(d_ch5));


   //if (digitalRead(16) == HIGH) Serial.println(F("REMOVE BUTON PRESSED"));

   
}


//#####################
//Loop function runs continuously, in a loop after the microporocessor setup
//#####################
void loop_dl() {
    //digitalWrite(LED_pin, HIGH); // Turn on LED
    analogWrite(LED_pin, LED_bright); // Turn on LED
    digitalWrite(infoLED_pin, HIGH); // Turn on LED

    get_samples();

    //#############################################
    // Check sampling time and deliver measurements
    //#############################################

    // Update time only each 100ms, not each cycle (RTC has 1s resolution);
    millis_loop_start=millis();  //update time
    if (millis_loop_start-millis_last_update_sampling_start_time>100) { 
        millis_last_update_sampling_start_time = millis_loop_start;
        update_sampling_start_time();
    }

    // Is it time to store measurements?
    bool store_flag;
    if (storage_interval_s < 1){
        store_flag = (millis_last_stored + storage_interval_s * 1000 <= millis());
        }
    else {store_flag = (last_stored_s + int(storage_interval_s) <= s_start_last_sample);}
    
    if (store_flag){
        //digitalWrite(LED_pin, LOW); // blink info led      
        analogWrite(LED_pin, off); // blink info led      
        digitalWrite(infoLED_pin, LOW); // blink info led      


        update_datetime_text();
        // update filename if it is a new day
        if (update_filename()){
            Serial.print(F("I<Initializing new file: "));
            Serial.print(file_n);
            Serial.println(">");
            myFile = SD.open(file_n, FILE_WRITE);
            
            if (!myFile){
                Serial.println(F("[ALERT] SD Card file initialization failed."));
                init_SDcard();
            }
            headers_to_log(log_file);
            myFile.close();        
        }

        digital_pin();        

        send_log_to_sdcard();  
        send_log_to_serial(); 

        last_stored_s=s_start_last_sample;
        millis_last_stored = millis_start_last_sample; // As backup against crashing

        meassurements_counter+=1;
        
        reset_measurements();
        count_sampling_rounds = 0;

        delay(10); // For LED Blink

    }
}

//Loop function runs continuously
void loop() {
  loop_dl();
}
