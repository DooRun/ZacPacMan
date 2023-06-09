//  ZacPacManAtmega

/*    
 * Atmega chip code for Zach's PacMan lighted shelf.
 * Thanks to Nick Kantack for flicker code / algorhythm along with much electrical circuit support. 
*/

//----- DEFINITIONS -----//
//----- PACMAN AND MS PACMAN SONG INFO -----//
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
//----- end DEFINITIONS
int tempo = 30;// change this to make the song slower or faster
// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
// Pacman intro song/Score available at https://musescore.com/user/85429/scores/107109
int melody[] = {  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_B5,  32, NOTE_FS5, -16, NOTE_DS5, 8, 
                  NOTE_C5, 16, NOTE_C6, 16, NOTE_G5,  16, NOTE_E5,  16, NOTE_C6,  32, NOTE_G5,  -16, NOTE_E5,  8,
                  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_B5,  32, NOTE_FS5, -16, NOTE_DS5, 8,  
                  NOTE_DS5,32, NOTE_E5, 32, NOTE_F5,  32, NOTE_F5,  32, NOTE_FS5, 32, NOTE_G5,   32, NOTE_G5, 32, NOTE_GS5, 32, NOTE_A5, 16, NOTE_B5,8}; // mr pacman

int melody2[] = { NOTE_G4, 32, NOTE_A4, 32, NOTE_B4, 32, NOTE_C5,  8, NOTE_E5,  8, NOTE_D5,  8, NOTE_F5, 8, 
                  NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_E5, 16, NOTE_D5,  8, NOTE_F5,  8,
                  NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_A5, 16, NOTE_B5, 16,
                  NOTE_C6,  8, NOTE_B5,  8, NOTE_C6,  8}; // ms pacman

int melody3[] = { NOTE_DS4,16, NOTE_F4, 16, REST,    16, NOTE_GS4, 16, REST,    8, NOTE_DS4, 8, REST,     8, NOTE_C4,  8, NOTE_AS3, 16, NOTE_C4, 16, NOTE_DS4, 8,
                  NOTE_AS3,16, NOTE_C4, 16, REST,    16, NOTE_DS4, 16, REST,   16, NOTE_A3, 16, NOTE_AS3, 8, NOTE_C4,  8, NOTE_DS4,  8, NOTE_F4,  8, NOTE_GS4, 8}; // staying alive

int melody4[] = { NOTE_GS5, 2, // trill
                  NOTE_E5,  16, NOTE_FS5, 16, NOTE_GS5,  2, NOTE_CS5, 2} /*,
                  NOTE_CS6, 102, NOTE_CS6, 4,  NOTE_B5, 8, NOTE_CS6, 8, NOTE_B5,  8, NOTE_A5,  16, NOTE_A5,  2, REST,    2,
                  NOTE_GS5, 2, NOTE_DS5, 2, NOTE_E5, 2, NOTE_FS5, 4, NOTE_CS5, 4,
                  NOTE_CS5, 16, NOTE_DS5, 16, // trill
                  REST,    8,  NOTE_CS5, 8, NOTE_DS5, 2, REST, 4,
                  NOTE_GS4, 4, NOTE_GS5, 2, 
                  NOTE_E5, 16, NOTE_FS5, 16, NOTE_GS5, 16, NOTE_A5, 16, NOTE_GS5, -4, NOTE_CS5, 4, NOTE_CS6, 4,
                  NOTE_CS7, 8, NOTE_D7,  8, NOTE_B6,  8, NOTE_A6,  8, NOTE_GS6, 8, NOTE_FS6, 8,
                  NOTE_F6, 16, NOTE_D6, 16, NOTE_CS6,16, NOTE_B4, 16, NOTE_A4, 16, NOTE_GS4,16, NOTE_FS4,16, NOTE_F4,16, NOTE_DS4,16, NOTE_CS4,16, NOTE_B3,16,
                  NOTE_A3, -2, REST,     4, NOTE_E3,  2, NOTE_D5, -4, NOTE_CS5, 8, //trill
                  NOTE_A4, 8, NOTE_B4, 32, NOTE_C5, 32, NOTE_CS5,32, NOTE_DS5,32, NOTE_E5,  4, NOTE_DS5, 8, NOTE_CS5, 8, NOTE_GS4,8, REST, 8, NOTE_A4, 2, NOTE_GS4, 4,NOTE_CS4, 1}; // Chopin Nocturne in C sharp Minor
                  */  // Temporarily disabled most of this array as it is too big and causing performance problems.
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;
int notes3 = sizeof(melody3) / sizeof(melody3[0]) / 2;
int notes4 = sizeof(melody4) / sizeof(melody4[0]) / 2;
//byte notelights[62] = {0,0, 4,4, 2,2, 0,0 , 4,4, 2,2, 0,0, 1,1, 5,5, 3,3, 1,1, 5,5, 3,3, 1,1, 0,0, 4,4 ,2,2, 0,0, 4,4, 2,2, 0,0, 0,0, 1,1, 1,1, 2,2, 2,2, 3,3, 3,3, 4,4, 4,4, 5,5};

byte notelights[31] = {0,4,2,0,4,2,0,1,5,3,1,5,3,1,0,4,2,0,4,2,0,0,1,1,2,2,3,3,4,4,5};
byte notelights2[24] = {0,1,2,0,2,1,3,2,3,4,3,2,4,0,1,2,1,2,3,4,5,7,6,7};
byte notelights3[22] = {3,4,6,5,6,3,6,2,1,2,3,1,2,6,3,6,0,1,2,3,4,5};
byte notelights4[66] = {4,  2,3,4,0,5,5,4,5,4,3,3,6,5,2,3,4,2,  2,3,6,1,3,2,0,1,2,3,4,3,0,3,4,4,5,4,3,2,1,5,4,3,2,1,0,5,4,3,2,1,0,0,3,2,  0,1,2,3,4,5,4,3,2,3,2,0};


int wholenote = (60000 * 4) / tempo;  // this calculates the duration of a whole note in ms
int divider = 0, noteDuration = 0;
//----- end PACMAN AND MS PACMAN SONG INFO

//----- GENERAL VARIABLES -----//

//----- MOTION RELATED -----//
const int MOTION_PIN = A4; // motion detector
bool MOTION_DETECTED;
bool MOTION_DETECTED_HOLD;
const int MOTION_LIGHT_DURATION = 2000;    // duration of shelf lit after motion detected.
unsigned long MOTION_OFF_TIME;
bool MO_EN_PIN = 1;
bool MO_EN_CLY = 1;
bool MO_EN_CHE = 1;
bool MO_EN_PAC = 1;
bool MO_EN_BLI = 1;
bool MO_EN_INK = 1;
short MOTION_TRIGGER_COUNTS;
bool REDUCE_MOTION_TRIGGER_COUNTS = 1;
unsigned long REDUCE_COUNTS_AT_TIME = millis();
//----- end MOTION RELATED

//----- LIGHT SENSOR RELATED -----//
const int PHOTO_RESISTOR = A5; // photoresistor for shop brightness (input)
int ROOM_LIGHT;  // value of current room light
bool ROOM_DARK;
int DARK_TRIGGER = 500;  // Value below which room is considered dark (ROOM_DARK IS THEN SET TO 1)
//----- end LIGHT SENSOR RELATED

const int NUMBER_OF_FLICKER_PINS = 6;
const int PINS_FOR_FLICKER[NUMBER_OF_FLICKER_PINS] = {11, 10, 9, 6, 5, 3};  // in order, Pinky, Clyde, Cherries, PacMan, Blinky, Inky.
bool M_EN = 1;    // Master enable   11
bool L_EN = 1;    // Light  enable   12
bool S_EN = 1;    // Sound  enable   13
bool MO_EN = 1;   // Motion enable   14
bool CL_EN = 1;   // Clock  enable   15
bool PIN_EN = 1;  // PINKY  enable   16
bool CLY_EN = 1;  // CLYDE  enable   17
bool CHE_EN = 1;  // CHERRY enable   18
bool PAC_EN = 1;  // PACMAN enable   19
bool BLI_EN = 1;  // BLINKY enable   20
bool INK_EN = 1;  // INKY   enable   21
bool LS_EN = 1;   // LIGHT SENSOR enable 22
//  DARK_TRIGGER DEFINE ABOVE IS 23
int PERF_NUM = 0; // PERFORMANCE NUMBER (to trigger events like PACMAN or MSPACMAN songs) 24

int i;
int j;
byte PIN;
byte TOGGLE;

//----- end GENERAL VARIABLES

//----- LIBRARIES -----//
#include <math.h>
#include "FlickerController.h"
#include <SoftwareSerial.h>    // Include software serial library, ESP8266 library dependency
//----- end LIBRARIES

//----- COMMUNICATIONS RELATED -----//  
  //  Arduino pin 0 (RX) to ESP8266 TX
  //  Arduino pin 1 to voltage divider then to ESP8266 RX
  //  Connect GND from the Arduiono to GND on the ESP8266
  //  Pull ESP8266 CH_PD HIGH
SoftwareSerial mySerial(0,1);  // RX,TX  (7,8 FOR arduino board but NOT FOR ATMEGA CHIP!!! WHICH IS 0,1)
long SERIAL_TIMEOUT = 350; // This is needed for serial read otherwise, may not read all the data.
bool MESSAGE_RECEIVED;
String MESSAGE_INCOMING = ""; 
int message_length;
String CMD_CAT_STRING;  // <== command_category_string
int CMD_CAT_VAL;        // <== command_category_value
String CMD_VAL_STRING;  // <== command_value_string
int CMD_VAL_VAL;        // <== command_value_value
int CMD_VAL_LENGTH[99];
short data_count;                    // counter for number of characters stored or printed.
unsigned char c;                   // char read by client from server http reply
unsigned char data_line [601];     // this will be an array holding the invidiually read data points in ASCII value.
int ZPMZ_end;  // This is the first position in the data_line array after ZPMZ has been found.
bool break_out = 0;  // used to exit several nested loops once all important part of message is received externally from serial buffer.
//----- end COMMUNICATIONS RELATED

FlickerController flicker_controller(PINS_FOR_FLICKER, NUMBER_OF_FLICKER_PINS);  //---Nick's program for flickering LEDs as if short circuiting.

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);   // Start the software serial for communication with the ESP8266
  //debugSerial.println("Start setup");
  for(i=0; i<6; i+=1){pinMode(PINS_FOR_FLICKER[i],OUTPUT);}
  pinMode(13,OUTPUT); //---sound pin
  pinMode(PHOTO_RESISTOR, INPUT);
  pinMode(MOTION_PIN, INPUT);
  flicker_controller.setup_controller();
  CMD_VAL_LENGTH[11] = 1;  // Master enable
  CMD_VAL_LENGTH[12] = 1;  // Light enable
  CMD_VAL_LENGTH[13] = 1;  // Sound enable
  CMD_VAL_LENGTH[14] = 1;  // Motion enable
  CMD_VAL_LENGTH[15] = 1;  // Clock enable
  CMD_VAL_LENGTH[16] = 1;  // Light Pinky  PIN
  CMD_VAL_LENGTH[17] = 1;  // Light Clyde  CLY
  CMD_VAL_LENGTH[18] = 1;  // Light Cherry CHE
  CMD_VAL_LENGTH[19] = 1;  // Light PacMan PAC
  CMD_VAL_LENGTH[20] = 1;  // Light Blinky BLI
  CMD_VAL_LENGTH[21] = 1;  // Light Inky   INK
  CMD_VAL_LENGTH[22] = 1;  // Light Sensor enable
  CMD_VAL_LENGTH[23] = 4;  // Light Sensor trigger value
  CMD_VAL_LENGTH[24] = 1;  // Night Light enable
  CMD_VAL_LENGTH[25] = 1;  // Location (SF=0, Brkgs=1) (ip address)
  CMD_VAL_LENGTH[26] = 1;  // Alarm enable
  CMD_VAL_LENGTH[27] = 6;  // Alarm date
  CMD_VAL_LENGTH[28] = 6;  // Alarm time
  CMD_VAL_LENGTH[29] = 2;  // Performance number

  all_lights_off(); 
  //play_PacMan_intro_song(3);
  //play_MsPacMan_intro_song(3);
  //play_Stayin_Alive_song(3);
  //play_Chopin(3);
  //flicker_controller.do_flicker(true);
}

void loop() 
{
  MasterEnableOffResumePosition:  // this goto label is to bypass the majority of the main loop after communication if the main power is shut off by Android app (master enable M_EN==0).
  //----- READ/STORE WANTED DATA FROM RTL8720DN_BW PLACED IN SERIAL BUFFER IF IT EXISTS -----//
  ZPMZ_end = 0;
  if(mySerial.available())
  {
    long lastTimeSerialAvailable = millis();
    while (millis() - lastTimeSerialAvailable < SERIAL_TIMEOUT)
    { 
      data_count = 0;
      while (mySerial.available())
      {
        MESSAGE_RECEIVED = true;
        data_count +=1;
        char c = mySerial.read(); // read a byte
        data_line[data_count]=c;
        delay(1);
        Serial.write(c);   //<---Z?
        if(c == 90)   // Z was found
        {
          data_count +=1;
          c = mySerial.read();    // read a byte 
          data_line[data_count]=c;
          delay(1);
          Serial.write(c);  //<---P?
          if(c == 80)    // P was found
          {
            data_count +=1;
            c = mySerial.read();    // read a byte 
            data_line[data_count]=c;
            delay(1);
            Serial.write(c); //<---M?
            if(c == 77)   // M was found
            {
              data_count +=1;
              c = mySerial.read();    // read a byte 
              data_line[data_count]=c;
              delay(1);
              Serial.write(c); //<---Z?
              if(c == 90)   // Z was found for a total of ZPMZ
              {
                Serial.println("");
                //Serial.println("ZPMZ WAS found");
                ZPMZ_end = data_count;
                Serial.write(90);  // Z <======= Need to write ZPMZ since it was found and removed from serial buffer...
                Serial.write(80);  // P
                Serial.write(77);  // M
                Serial.write(90);  // Z
                while (mySerial.available())  // while there are bytes to read from the client,
                {
                  data_count +=1;
                  c = mySerial.read(); // read a byte
                  data_line[data_count]=c;
                  Serial.write(c);  //<======= DO NOT comment this Serial.write statement as part of disabling DEBUG
                  if(c==120){break_out=1;}  // 120 is the ASCII value for lower case x.
                  if(break_out==1){break;}
                  delay(2);
                }
              }
            }
          }
          if(break_out==1){break;}
        }  
        if(break_out==1){break;}
      }
      if(break_out==1){break;}
    }
    break_out = 0;
  }
  //----- end READ/STORE WANTED DATA FROM RTL8720DN_BW PLACED IN SERIAL BUFFER IF IT EXISTS

  //----- INTERPRET MESSAGE -----//
  if(data_count >0)
  {
    MESSAGE_RECEIVED == true;
    //debugSerial.println("");
    //debugSerial.println(data_count);

    //debugSerial.println("");
    //debugSerial.print("ZPMZ_end =");
    //debugSerial.println(ZPMZ_end);

    //debugSerial.println("");
    //debugSerial.print("data line of ZPMZ =");
    //debugSerial.println(data_line[ZPMZ_end+1]);

    CMD_CAT_VAL = (data_line[ZPMZ_end+1]-48)*10 + data_line[ZPMZ_end+2]-48;
    //debugSerial.print("CMD_CAT_VAL = ");
    //debugSerial.println(CMD_CAT_VAL);

    CMD_VAL_VAL = data_line[ZPMZ_end+4]-48;
    //debugSerial.print("CMD_VAL_VAL = ");
    if(CMD_VAL_LENGTH[CMD_CAT_VAL]>1)
    {
      for (i = 1; i<CMD_VAL_LENGTH[CMD_CAT_VAL]; i++)
      {
        CMD_VAL_VAL = CMD_VAL_VAL*10+data_line[ZPMZ_end+4+i]-48;
      }
    }
    //debugSerial.println(CMD_VAL_VAL);

    //debugSerial.println("");
    //debugSerial.println("");
    mySerial.flush();
    data_count=0;
    //----- end INTERPRET MESSAGE
  }

  //---UPDATE VARIABLES IF MESSAGE WAS RECEIVED---//
  if(MESSAGE_RECEIVED == true)  
  {
    if(CMD_CAT_VAL == 11){if(CMD_VAL_VAL == 1){M_EN = 1;}else{M_EN = 0;}}  // Master enable
    if(CMD_CAT_VAL == 12){if(CMD_VAL_VAL == 1){L_EN = 1;}else{L_EN = 0;}}  // Light enable
    if(CMD_CAT_VAL == 13){if(CMD_VAL_VAL == 1){S_EN = 1;}else{S_EN = 0;}}  // Sound enable<---Change sound enable else S_EN=0 eventually
    if(CMD_CAT_VAL == 14){if(CMD_VAL_VAL == 1){MO_EN = 1;}else{MO_EN = 0;}}  // Motion enable
    if(CMD_CAT_VAL == 15){if(CMD_VAL_VAL == 1){CL_EN = 1;}else{CL_EN = 0;}}  // Clock enable
    if(CMD_CAT_VAL == 16){if(CMD_VAL_VAL == 1){PIN_EN = 1;}else{PIN_EN = 0;}}  // Light Pinky  PIN flickerPin0
    if(CMD_CAT_VAL == 17){if(CMD_VAL_VAL == 1){CLY_EN = 1;}else{CLY_EN = 0;}}  // Light Clyde  CLY flickerPin1
    if(CMD_CAT_VAL == 18){if(CMD_VAL_VAL == 1){CHE_EN = 1;}else{CHE_EN = 0;}}  // Light Cherry CHE flickerPin2
    if(CMD_CAT_VAL == 19){if(CMD_VAL_VAL == 1){PAC_EN = 1;}else{PAC_EN = 0;}}  // Light PacMan PAC flickerPin3
    if(CMD_CAT_VAL == 20){if(CMD_VAL_VAL == 1){BLI_EN = 1;}else{BLI_EN = 0;}}  // Light Blinky BLI flickerPin4
    if(CMD_CAT_VAL == 21){if(CMD_VAL_VAL == 1){INK_EN = 1;}else{INK_EN = 0;}}  // Light Inky   INK flickerPin5
    if(CMD_CAT_VAL == 22){if(CMD_VAL_VAL == 1){LS_EN = !LS_EN;}}    // LIGHT SENSOR enable
    if(CMD_CAT_VAL == 23){if(CMD_VAL_VAL == 100){PAC_EN = 0; CHE_EN=1;}}  // Light Sensor Trigger Value
    if(CMD_CAT_VAL == 23){if(CMD_VAL_VAL == 500){PAC_EN = 1; CHE_EN=0;}}  // Light Sensor Trigger Value
    if(CMD_CAT_VAL == 24){}  // Night Light Enable
    if(CMD_CAT_VAL == 25){}  // (ip address preset)
    if(CMD_CAT_VAL == 26){}  // Alarm Enable
    if(CMD_CAT_VAL == 27){}  // Alarm Date
    if(CMD_CAT_VAL == 28){}  // Alarm Time
    if(CMD_CAT_VAL == 29){PERF_NUM = CMD_VAL_VAL;}      // Performance (song/light show)
       
    MESSAGE_RECEIVED = false;
  }
  //---end UPDATE VARIABLES IF MESSAGE WAS RECEIVED  }
  if(M_EN==0){all_lights_off();goto MasterEnableOffResumePosition;}

  
  delay(1);
  //-----PLAY SELECTED PERFORMANCE-----//
  if(CMD_CAT_VAL = 29)
  {
    if((PERF_NUM == 71) && (M_EN == 1)){play_PacMan_intro_song(3);   PERF_NUM = 0;}
    if((PERF_NUM == 72) && (M_EN == 1)){play_MsPacMan_intro_song(3); PERF_NUM = 0;}
    if((PERF_NUM == 73) && (M_EN == 1)){play_Stayin_Alive_song(4);   PERF_NUM = 0;}
    if((PERF_NUM == 74) && (M_EN == 1))
    {
      all_lights_off();
      flicker_controller.do_flicker(true);
      PERF_NUM = 0;
    }
    if((PERF_NUM == 75) && (M_EN == 1))
    {
      all_lights_on(); 
      flicker_controller.do_flicker(false);
      PERF_NUM = 0;
    }
    if((PERF_NUM == 76) && (M_EN == 1)){play_all(); PERF_NUM = 0;}  //<--not in android studio yet and need to refine here first.
    if((PERF_NUM == 77) && (M_EN == 1)){play_Chopin(3); PERF_NUM = 0;}
  }
  //-----end PLAY SELECTED PERFORMANCE
  
  //----- RESPOND TO ESP8266 -----//
  //mySerial.print("THANKS FROM PACMAN"); // ESP8266 will request next node.js string of data if it sees anything come in on RX pin.  "T" is for 'thanks' but it could be anything.
  //-----end RESPOND TO ESP8266

  //..........measure room brightness.................................................................................
  ROOM_LIGHT = analogRead(PHOTO_RESISTOR);
  if(ROOM_LIGHT < DARK_TRIGGER){ROOM_DARK = 1;}else{ROOM_DARK = 0;}

  //..........check for motion at all sensors................................................................
  if(MO_EN == 1)
  {
    MOTION_DETECTED = digitalRead(MOTION_PIN);
    if(MOTION_DETECTED == 1){MOTION_TRIGGER_COUNTS = MOTION_TRIGGER_COUNTS + 5;}
  }
  MOTION_TRIGGER_COUNTS = MOTION_TRIGGER_COUNTS - 1;
  if(MOTION_TRIGGER_COUNTS <0){MOTION_TRIGGER_COUNTS = 0;}
  if(MOTION_TRIGGER_COUNTS > 3000){MOTION_TRIGGER_COUNTS = 3000;}

  if(MOTION_TRIGGER_COUNTS * MO_EN>   0){MO_EN_PIN=1;}else{MO_EN_PIN=1-MO_EN;}
  if(MOTION_TRIGGER_COUNTS * MO_EN> 400){MO_EN_CLY=1;}else{MO_EN_CLY=1-MO_EN;}
  if(MOTION_TRIGGER_COUNTS * MO_EN> 800){MO_EN_CHE=1;}else{MO_EN_CHE=1-MO_EN;}
  if(MOTION_TRIGGER_COUNTS * MO_EN>1200){MO_EN_PAC=1;}else{MO_EN_PAC=1-MO_EN;}
  if(MOTION_TRIGGER_COUNTS * MO_EN>1600){MO_EN_BLI=1;}else{MO_EN_BLI=1-MO_EN;}
  if(MOTION_TRIGGER_COUNTS * MO_EN>2000){MO_EN_INK=1;}else{MO_EN_INK=1-MO_EN;}
    
  digitalWrite(PINS_FOR_FLICKER[0], 1 * MO_EN_PIN * L_EN * PIN_EN);
  digitalWrite(PINS_FOR_FLICKER[1], 1 * MO_EN_CLY * L_EN * CLY_EN);
  digitalWrite(PINS_FOR_FLICKER[2], 1 * MO_EN_CHE * L_EN * CHE_EN);
  digitalWrite(PINS_FOR_FLICKER[3], 1 * MO_EN_PAC * L_EN * PAC_EN);
  digitalWrite(PINS_FOR_FLICKER[4], 1 * MO_EN_BLI * L_EN * BLI_EN);   
  digitalWrite(PINS_FOR_FLICKER[5], 1 * MO_EN_INK * L_EN * INK_EN); 
  
  delay(5);
}


void fade_out_one_LED(){
  int random_LED = random(0,6);
  for (i = 255; i>0; i=i-1)
  {  
    boolean state;
    if ((random_LED == 0) || (random_LED == 5))
    {
      int temp = random(0,255);
      if(temp > i)
      {   
        boolean state = 0;
        digitalWrite(PINS_FOR_FLICKER[random_LED],LOW);
        }
        else
        {
          boolean state = 1;
          digitalWrite(PINS_FOR_FLICKER[random_LED],HIGH);
        }
    }
    else
    {
      analogWrite(PINS_FOR_FLICKER[random_LED],i);
    }
    if(S_EN==1){tone(13, i*3);}
    delay(15);
    if(S_EN==1){noTone(13);}
    if((random_LED == 0) || (random_LED == 5)){digitalWrite(PINS_FOR_FLICKER[random_LED],state);}
  }  
  digitalWrite(PINS_FOR_FLICKER[random_LED],0);
}

void play_PacMan_intro_song(byte added_divider) {
  all_lights_off();
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) 
  {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider / added_divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider) / added_divider;
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    digitalWrite(PINS_FOR_FLICKER[notelights[thisNote/2]],HIGH);
    delay(3);
    if(S_EN==1){tone(13, melody[thisNote], noteDuration * 0.9);}
    // Wait for the specified duration before playing the next note.
    delay(noteDuration/2);
    if((thisNote > 42) && thisNote < 2 * notes - 2){digitalWrite(PINS_FOR_FLICKER[notelights[thisNote/2]],LOW);}
    // stop the waveform generation before the next note.
    delay(noteDuration/2);
    if(S_EN==1){noTone(13);}
    delay(3);
    digitalWrite(PINS_FOR_FLICKER[notelights[thisNote/2]],LOW);
  //---end PLAY PACMAN INTRO SONG
  }
}

void play_MsPacMan_intro_song(byte added_divider) {
  all_lights_off();
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes2 * 2; thisNote = thisNote + 2) 
  {
    // calculates the duration of each note
    divider = melody2[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider / added_divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider) / added_divider;
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    if(S_EN==1){tone(13, melody2[thisNote], noteDuration * 0.9);}
    if(notelights2[thisNote/2] < 6){digitalWrite(PINS_FOR_FLICKER[notelights2[thisNote/2]],HIGH);} else
    {
      if((thisNote == 42) || (thisNote == 46)){all_lights_on();}        
      if(thisNote > 42){digitalWrite(PINS_FOR_FLICKER[3], HIGH);}  
    }    
    // Wait for the specified duration before playing the next note.
    delay(noteDuration);
    if(S_EN==1){noTone(13);}
    for(int i=0; i<6; i+=1){digitalWrite(PINS_FOR_FLICKER[i],LOW);}
  //---end PLAY MsPACMAN INTRO SONG
  }
}

void play_Stayin_Alive_song(byte added_divider) 
{
  all_lights_off();
  byte set = 42;
  Most_staying_alive_notes(added_divider, set);
  //two long notes
  all_lights_off();
  byte BRIGHT[6];
  for (int i = 2*NOTE_GS4; i<2*NOTE_AS4; i+=2)
  {
    if(S_EN==1){tone(13, i/2, 8); delay(5);}
    long temp = (i-54) % 25;
  }
  if(S_EN==1){tone(13,NOTE_AS4,6500);}
  short int trigger = 800;
  short int main_counter;    // for change in tempo of strobe
  short int counter_shutoff = 780; // for strobe effect (shutoff)
  bool shutoff = 1;
  for (int i = 0; i<3000; i=i+10)
  {
    int temp2 = 2000;
    delay(10);
    if(i<1800)
    {
      BRIGHT[1]=pow((2000-(pow(pow(temp2-i,2),.5))),0.8)*255/pow(temp2,0.8);
      BRIGHT[2]=pow((2000-(pow(pow(temp2-i,2),.5))),2.2)*255/pow(temp2,2.2);
      BRIGHT[3]=pow((2000-(pow(pow(temp2-i,2),.5))),3.4)*255/pow(temp2,3.4);
      BRIGHT[4]=pow((2000-(pow(pow(temp2-i,2),.5))),4.6)*255/pow(temp2,4.6);
    }
    else
    {
      BRIGHT[4]=BRIGHT[4]*.98;
      BRIGHT[3]=BRIGHT[3]*.94;
      BRIGHT[2]=BRIGHT[2]*.90;
      BRIGHT[1]=BRIGHT[1]*.88;
    }
    main_counter += 10;
    counter_shutoff +=10;  
    if((trigger == 800) && (i >= 1500)){trigger = 500; main_counter = 0;}
    if(counter_shutoff > 50){shutoff = 0;}
    if(counter_shutoff > trigger){counter_shutoff = 0; shutoff = 1;}
    if((trigger == 500) && (counter_shutoff % 450 == 0)){counter_shutoff = 0; shutoff = 1;}

    digitalWrite(PINS_FOR_FLICKER[0], shutoff);
    analogWrite(PINS_FOR_FLICKER[1], BRIGHT[1]);
    analogWrite(PINS_FOR_FLICKER[2], BRIGHT[2]);
    analogWrite(PINS_FOR_FLICKER[3], BRIGHT[3]);
    analogWrite(PINS_FOR_FLICKER[4], BRIGHT[4]);
    digitalWrite(PINS_FOR_FLICKER[5], shutoff);

  }
  all_lights_off();
  set = 44; 
  Most_staying_alive_notes(added_divider, set);
}
void Most_staying_alive_notes(byte added_divider, byte set) {
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < set; thisNote = thisNote + 2) 
  {
    // calculates the duration of each note
    divider = melody3[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider / added_divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider) / added_divider;
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    if(S_EN==1){tone(13, melody3[thisNote], noteDuration*.9);}
    digitalWrite(PINS_FOR_FLICKER[notelights3[thisNote/2]],HIGH);
    // Wait for the specified duration before playing the next note.
    delay(noteDuration/2);
    if((thisNote > 42) && thisNote < 2 * notes - 2){digitalWrite(PINS_FOR_FLICKER[notelights3[thisNote/2]],LOW);}
    // stop the waveform generation before the next note.
    delay(noteDuration/2);
    if(S_EN==1){noTone(13);}
    digitalWrite(PINS_FOR_FLICKER[notelights3[thisNote/2]],LOW);
    
  //---end PLAY PACMAN INTRO SONG
  }
}

void play_Chopin(byte added_divider)
{
  all_lights_off();
  delay(2000);
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote4 = 0; thisNote4 < notes4 * 2; thisNote4 = thisNote4 + 2) 
  {
    // Articulation helps create space between notes, if appropriate
    double articulation = 0.9;
    // calculates the duration of each note
    divider = melody4[thisNote4 + 1];
    if (divider > 100) {
      articulation = 1.1;
      divider -= 100;
    }

    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider / added_divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider) / added_divider;
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    digitalWrite(PINS_FOR_FLICKER[notelights4[thisNote4/2]],HIGH);
    if(S_EN==1){tone(13, melody4[thisNote4], noteDuration * articulation);}
    // Wait for the specified duration before playing the next note.
    delay(noteDuration);
    digitalWrite(PINS_FOR_FLICKER[notelights4[thisNote4/2]],LOW);
    if((S_EN==1)&&(articulation !=1.1)){noTone(13);}

    short int trillDuration = wholenote / added_divider / 45;

    //play trills if present...
    if(thisNote4==0)//play 1ST trill
    {
      for (int i = 0; i<12; i=i+1)
      {
        tone(13, NOTE_FS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[1],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[1],LOW);
        tone(13, NOTE_GS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],LOW);
      }
    }
    if(thisNote4==36)//play 2ND trill
    {      
      for (int i = 0; i<16; i=i+1)
      {
        tone(13, NOTE_E5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],LOW);
        tone(13, NOTE_DS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[3],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[3],LOW);
      }
    }
    if(thisNote4==50)//play 3RD trill
    {
      for (int i = 0; i<12; i=i+1)
      {
        tone(13, NOTE_FS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[2],LOW);
        tone(13, NOTE_GS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[3],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[3],LOW);
      }
    }
    if(thisNote4==110)//play 4TH trill
    {
      for (int i = 0; i<6; i=i+1)
      {
        tone(13, NOTE_B4, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[0],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[0],LOW);
        tone(13, NOTE_CS5, trillDuration);
        digitalWrite(PINS_FOR_FLICKER[1],HIGH);
        delay(trillDuration);
        digitalWrite(PINS_FOR_FLICKER[1],LOW);
      }
    }
    //for(int i=0; i<6; i+=1){digitalWrite(PINS_FOR_FLICKER[i],LOW);}
  //---end PLAY CHOPIN SONG
  }
}

void play_all()
{
  flicker_controller.do_flicker(true);
  delay(2000);
  flicker_controller.do_flicker(false);
  delay(2000);
  fade_out_one_LED();
  delay(2000);
  play_PacMan_intro_song(3);
  delay(2000);
  play_MsPacMan_intro_song(3);
  delay(2000);
  play_Stayin_Alive_song(4);
  delay(2000);
  play_Chopin(4);
  delay(2000);
  // Now fade in PacMan
  byte j;
  for (i = 0; i<=255; i=i+1)
  {
    //j = i + int ((i*i*i)/1000000) + int (abs(255-i)/100);
    j = i + int(i^(3/5)) + int (abs(255-i)/50);
    if(j>250){
      j=255;
      i=255;
    }
    analogWrite(PINS_FOR_FLICKER[3],j);
    delay(66);
  }
  analogWrite(PINS_FOR_FLICKER[3],255);
  // Now fade in the rest
  for (i = 0; i<=255; i=i+1)  
  {
    //j = i + int((i*i*i)/1000000) + int (abs(255-i)/100);
    j = i + int(i^(3/5)) + int (abs(255-i)/50);
    if(j>250){
      j=255;
      i=255;
    }
    analogWrite(PINS_FOR_FLICKER[0],j);
    analogWrite(PINS_FOR_FLICKER[1],j);
    analogWrite(PINS_FOR_FLICKER[2],j);
    analogWrite(PINS_FOR_FLICKER[4],j);
    analogWrite(PINS_FOR_FLICKER[5],j);
    delay(66);
  }
  //---end STARTUP SEQUENCE
  digitalWrite(PINS_FOR_FLICKER[0],1);
  digitalWrite(PINS_FOR_FLICKER[1],1);
  digitalWrite(PINS_FOR_FLICKER[2],1);
  digitalWrite(PINS_FOR_FLICKER[3],1);
  digitalWrite(PINS_FOR_FLICKER[4],1);
  digitalWrite(PINS_FOR_FLICKER[5],1);
}

void all_lights_on(){
  digitalWrite(PINS_FOR_FLICKER[0], HIGH);
  digitalWrite(PINS_FOR_FLICKER[1], HIGH);
  digitalWrite(PINS_FOR_FLICKER[2], HIGH);
  digitalWrite(PINS_FOR_FLICKER[3], HIGH);
  digitalWrite(PINS_FOR_FLICKER[4], HIGH);
  digitalWrite(PINS_FOR_FLICKER[5], HIGH);
}

void all_lights_off(){
  digitalWrite(PINS_FOR_FLICKER[0], LOW);
  digitalWrite(PINS_FOR_FLICKER[1], LOW);
  digitalWrite(PINS_FOR_FLICKER[2], LOW);
  digitalWrite(PINS_FOR_FLICKER[3], LOW);
  digitalWrite(PINS_FOR_FLICKER[4], LOW);
  digitalWrite(PINS_FOR_FLICKER[5], LOW);
}