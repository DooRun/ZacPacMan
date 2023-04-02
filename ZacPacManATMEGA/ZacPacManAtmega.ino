//  ZacPacManAtmega

/*    
 * Atmega chip code for Zach's PacMan lighted shelf.
 * Thanks to Nick Kantack for flicker code / algorhythm along with much electrical circuit support. 
*/

//----- DEFINITIONS start -----//
//----- PACMAN AND MS PACMAN SONG INFO start -----//
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
                  NOTE_DS5,32, NOTE_E5, 32, NOTE_F5,  32, NOTE_F5,  32, NOTE_FS5, 32, NOTE_G5,   32, NOTE_G5, 32, NOTE_GS5, 32, NOTE_A5, 16, NOTE_B5,8};

int melody2[] = { NOTE_G4, 32, NOTE_A4, 32, NOTE_B4, 32, NOTE_C5,  8, NOTE_E5,  8, NOTE_D5,  8, NOTE_F5, 8, 
                  NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_E5, 16, NOTE_D5,  8, NOTE_F5,  8,
                  NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_E5, 16, NOTE_F5, 16, NOTE_G5, 16, NOTE_A5, 16, NOTE_B5, 16,
                  NOTE_C6,  8, NOTE_B5,  8, NOTE_C6,  8};
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;
byte notelights[62] = {0,0, 4,4, 2,2, 0,0 , 4,4, 2,2, 0,0, 1,1, 5,5, 3,3, 1,1, 5,5, 3,3, 1,1, 0,0, 4,4 ,2,2, 0,0, 4,4, 2,2, 0,0, 0,0, 1,1, 1,1, 2,2, 2,2, 3,3, 3,3, 4,4, 4,4, 5,5};
byte notelights2[54] = {0,0, 1,1, 2,2, 0,0, 2,2, 1,1, 3,3, 2,2, 3,3, 4,4, 3,3, 2,2, 4,4, 0,0, 1,1, 2,2, 1,1, 2,2, 3,3, 4,4, 5,5, 7,7, 6,6, 7,7};
// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
//----- end PACMAN AND MS PACMAN SONG INFO

//----- GENERAL VARIABLES -----//
//----- MOTION VARIABLES -----//
const int MOTION_PIN = A4;    // motion detector
bool MOTION_DETECTED;
bool MOTION_DETECTED_HOLD;
const int MOTION_LIGHT_DURATION = 5000;    // duration of shelf lit after motion detected.
unsigned long MOTION_OFF_TIME;
//----- end MOTION VARIABLES
//----- ROOM BRIGHTNESS VARIABLES -----//
const int PHOTO_RESISTOR = A5; // photoresistor for shop brightness (input)
int ROOM_LIGHT;  // value of current room light
bool ROOM_DARK;
int DARK_TRIGGER = 500;  // Value below which room is considered dark (ROOM_DARK IS THEN SET TO 1)

const int NUMBER_OF_FLICKER_PINS = 6;
const int PINS_FOR_FLICKER[NUMBER_OF_FLICKER_PINS] = {11, 10, 9, 6, 5, 3};  // in order, Pinky, Clyde, Cherries, PacMan, Blinky, Inky.

bool M_EN = 1; // Master enable:  Enables all lights and sound for all modes.
bool L_EN = 1; // Light enable:  Enables all lights.
bool S_EN = 1;  // Sound enable:  Enables sound (Allows for disableing sound for night modes or other times silence is desired).
bool CHAR_EN [6] = {1,1,1,1,1,1};  // Character enable: Enables lighting up charactors Pinky, Clyde, Cherries, PacMan, Blinky, Inky, respectively.
bool CLCK_EN = 1;  // Clock enable.
int i;
int j;
byte PIN;
byte TOGGLE;
//----- end GENERAL VARIABLES

//----- LIBRARIES start -----//
#include <math.h>
#include "FlickerController.h"
#include <SoftwareSerial.h>    // Include software serial library, ESP8266 library dependency
//----- end LIBRARIES

//----- COMMUNICATIONS RELATED -----//
long SERIAL_TIMEOUT = 150; // This is needed for serial read otherwise, may not read all the data.
//unsigned char c;                   // char read by client from server http reply
bool ESP_SPOKE;
SoftwareSerial mySerial(0,1);  // RX,TX
String MESSAGE_INCOMING = ""; 
int message_length;

String CMD_CAT_STRING;
int CMD_CAT_VAL;

String CMD_VAL_STRING;
int CMD_VAL_VAL;

int MESSAGE_PART_LENGTH[99];
  //  Arduino pin 0 (RX) to ESP8266 TX
  //  Arduino pin 1 to voltage divider then to ESP8266 RX
  //  Connect GND from the Arduiono to GND on the ESP8266
  //  Pull ESP8266 CH_PD HIGH
//----- end COMMUNICATIONS RELATED

FlickerController flicker_controller(PINS_FOR_FLICKER, NUMBER_OF_FLICKER_PINS);  //---Nick's program for flickering LEDs as if short circuiting.

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);   // Start the software serial for communication with the ESP8266
  //Serial.println("Start setup");
  for(i=0; i<6; i+=1){pinMode(PINS_FOR_FLICKER[i],OUTPUT);}
  pinMode(13,OUTPUT); //---sound pin
  pinMode(PHOTO_RESISTOR, INPUT);
  pinMode(MOTION_PIN, INPUT);
  flicker_controller.setup_controller();
  //Startup_Sequence();
  MESSAGE_PART_LENGTH[11] = 5;
  MESSAGE_PART_LENGTH[12] = 5;
  MESSAGE_PART_LENGTH[13] = 5;
  digitalWrite(PINS_FOR_FLICKER[0], 1);
  digitalWrite(PINS_FOR_FLICKER[1], 1);
  digitalWrite(PINS_FOR_FLICKER[2], 1);
  digitalWrite(PINS_FOR_FLICKER[3], 1);
  digitalWrite(PINS_FOR_FLICKER[4], 1);   
  digitalWrite(PINS_FOR_FLICKER[5], 1); 
}

void loop() {
  //Check to see if anything is available in the serial receive buffer
  MESSAGE_INCOMING = "";
  while (mySerial.available())
  {
    delay(7);  // 5 seems to work.  I put it at 7.  If garbage is communicated, increase this value.  10 is shown online for a good value.
    ESP_SPOKE = true;
    while (mySerial.available() > 0)
    {
      char c = mySerial.read();  // read a byte
      MESSAGE_INCOMING += c;
    }
  }

  if(ESP_SPOKE == true)  
  {
    CMD_CAT_STRING = "";  // reset
    CMD_CAT_STRING = MESSAGE_INCOMING.substring(0,2);
    CMD_CAT_VAL = CMD_CAT_STRING.toInt();

    CMD_VAL_STRING = "";  // reset
    CMD_VAL_STRING = MESSAGE_INCOMING.substring(4,MESSAGE_PART_LENGTH[CMD_CAT_VAL]-2);
    CMD_VAL_VAL = CMD_VAL_STRING.toInt();
     
    Serial.println();
    Serial.println("cmd_cat_val = ");
    Serial.println(CMD_CAT_VAL);
   
    Serial.println();
    Serial.println("cmd_val_val = ");
    Serial.println(CMD_VAL_VAL);
  
    
    if(CMD_CAT_VAL == 11){if(CMD_VAL_VAL == 1){M_EN = 1;}else{M_EN = 0;}}  // Master enable
    if(CMD_CAT_VAL == 12){if(CMD_VAL_VAL == 1){L_EN = 1;}else{S_EN = 0;}}  // Master enable
    if(CMD_CAT_VAL == 13){if(CMD_VAL_VAL == 1){S_EN = 1;}else{L_EN = 0;}}  // Master enable
    int temp1 = M_EN * L_EN;
    digitalWrite(PINS_FOR_FLICKER[0], temp1);
    digitalWrite(PINS_FOR_FLICKER[1], 1 * M_EN * L_EN);
    digitalWrite(PINS_FOR_FLICKER[2], 1 * M_EN * L_EN);
    digitalWrite(PINS_FOR_FLICKER[3], 1 * M_EN * L_EN);
    digitalWrite(PINS_FOR_FLICKER[4], 1 * M_EN * L_EN);   
    digitalWrite(PINS_FOR_FLICKER[5], 1 * M_EN * S_EN);   //change S_EN back to L_EN 
    ESP_SPOKE = false;
  }
  delay(5);
  digitalWrite(13,M_EN);
  Serial.flush();
}


/*
    c = client.read();  // read a byte
    MESSAGE_TO_ATMEGA += c;
    Serial.write(c);
    COMMAND_CAT += c - 48;
    message_length = MESSAGE_PART_LENGTH[COMMAND_CAT];
    for(int i=0; i<data_count + 1; i++)
    {
      COMMAND_VALUE = COMMAND_VALUE * 10 + 
      report = report + char(data_line [i]);
      //digitalWrite(PINS_FOR_FLICKER[3],1);
      //delay(100);
      //digitalWrite(PINS_FOR_FLICKER[3],0);
      //delay(400);
      }
    //debug Serial.println(report);
    byte START = report.indexOf('cmd:');
    if (report.indexOf("GPIO 5 on") >= 0) {play_PacMan_intro_song(3);}
    //debug Serial.println(START);
    //String COMMAND = report.substring(START+4, START+5);
    //debug Serial.print("command =  ");
    //debug Serial.println(COMMAND);
    //PIN = COMMAND.toInt();
    /*for(int i=0; i<PIN + 1; i++)
    {
      digitalWrite(PINS_FOR_FLICKER[3],1);
      delay(100);
      digitalWrite(PINS_FOR_FLICKER[3],0);
      delay(400);
    }*/
    //debug Serial.print("integer value of ASCII = ");
    //debug Serial.print(PIN);
    //debug Serial.println();
    //debug Serial.println();
    //analogWrite(PINS_FOR_FLICKER[PIN],1); 
    //Serial.print("PIN =  ");
    //Serial.println(COMMAND);
    //Serial.println(PIN);   
    //play_PacMan_intro_song(3);
    /*TOGGLE = TOGGLE + 1;
    if(TOGGLE ==2)
    {
      PIN = PIN + 1;
      if(PIN==3){play_MsPacMan_intro_song(3);}
      if(PIN==5){play_PacMan_intro_song(3);}
      if(PIN>5){PIN=0;}
      report = "";
      data_count = 0;
      digitalWrite(PINS_FOR_FLICKER[5],1);
      delay(200);
      //digitalWrite(13,1);
      //digitalWrite(13,0);
      digitalWrite(PINS_FOR_FLICKER[0],0);
      digitalWrite(PINS_FOR_FLICKER[1],0);
      digitalWrite(PINS_FOR_FLICKER[2],0);
      digitalWrite(PINS_FOR_FLICKER[3],0);
      digitalWrite(PINS_FOR_FLICKER[4],0);   
      digitalWrite(PINS_FOR_FLICKER[5],0);    
      digitalWrite(PINS_FOR_FLICKER[PIN],1);   
      delay(1000);
      if(PIN==4){}
      
      if(PIN<4)
      {
        digitalWrite(PINS_FOR_FLICKER[0],1);
        digitalWrite(PINS_FOR_FLICKER[1],1);
        digitalWrite(PINS_FOR_FLICKER[2],1);
        digitalWrite(PINS_FOR_FLICKER[3],1);
        digitalWrite(PINS_FOR_FLICKER[4],1);   
        digitalWrite(PINS_FOR_FLICKER[5],1);
      }
      else
      {
        digitalWrite(PINS_FOR_FLICKER[0],0);
        digitalWrite(PINS_FOR_FLICKER[1],0);
        digitalWrite(PINS_FOR_FLICKER[2],0);
        digitalWrite(PINS_FOR_FLICKER[3],0);
        digitalWrite(PINS_FOR_FLICKER[4],0);   
        digitalWrite(PINS_FOR_FLICKER[5],0);
      }    
      ESP_SPOKE = false; 
      TOGGLE = 0;
    }
    else{}
  }
}
  delay(10);

  /*
  //..........process message from ESP8266 or Phone if present and respond with data if that is the plan..............
  
  //..........measure room brightness.................................................................................
  //..........check for/react to motion at all sensors................................................................
  */
  
  //..........process message from ESP8266 or Phone if present and respond with data if that is the plan.......................
  //--- READ/STORE WANTED DATA FROM ESP8266 IF IT EXISTS ---//
  /*long lastTimeSerialAvailable = millis();
  while (millis() - lastTimeSerialAvailable < SERIAL_TIMEOUT)
  {
      while (mySerial.available())
      {
          //digitalWrite(LED_orange_pin, HIGH);
          data_count += 1;
          lastTimeSerialAvailable = millis();
          data_line [data_count] = mySerial.read();
      }
  }


  //--- end READ/STORE WANTED DATA FROM ESP8266 IF IT EXISTS
    
  //----- RESPOND TO ESP8266 -----//
  mySerial.print("THANKS FROM PACMAN"); // ESP8266 will request next node.js string of data if it sees anything come in on RX pin.  "T" is for 'thanks' but it could be anything.
  //-----end RESPOND TO ESP8266

  //..........measure room brightness.................................................................................
  ROOM_LIGHT = analogRead(PHOTO_RESISTOR);
  if(ROOM_LIGHT < DARK_TRIGGER){ROOM_DARK = 1;}else{ROOM_DARK = 0;}
 
  //..........check for motion at all sensors................................................................
  MOTION_DETECTED = digitalRead(MOTION_PIN);
  if(MOTION_DETECTED == 1){
    MOTION_DETECTED_HOLD = 1;
    MOTION_OFF_TIME = millis() + MOTION_LIGHT_DURATION;
  }
  if(MOTION_OFF_TIME < millis()){analogWrite(PINS_FOR_FLICKER[5],0);}
  
  if(MASTER_ON == 1)
  {
    if(ROOM_DARK == 0){analogWrite(PINS_FOR_FLICKER[3],255);}else{analogWrite(PINS_FOR_FLICKER[3],0);}
    if(MOTION_DETECTED == 1){analogWrite(PINS_FOR_FLICKER[5],255);}
  }
  
  delay(40);
*/



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
    tone(13, i*3);
    delay(15);
    noTone(13);
    if((random_LED == 0) || (random_LED == 5)){digitalWrite(PINS_FOR_FLICKER[random_LED],state);}
  }  
  digitalWrite(PINS_FOR_FLICKER[random_LED],0);
}

void play_PacMan_intro_song(byte added_divider) {
    //---PLAY PACMAN INTRO SONG---//
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
    tone(13, melody[thisNote], noteDuration * 0.9);
    digitalWrite(PINS_FOR_FLICKER[notelights[thisNote]],HIGH);
    // Wait for the specified duration before playing the next note.
    delay(noteDuration/2);
    if((thisNote > 42) && thisNote < 2 * notes - 2){digitalWrite(PINS_FOR_FLICKER[notelights[thisNote]],LOW);}
    // stop the waveform generation before the next note.
    delay(noteDuration/2);
    noTone(13);
    digitalWrite(PINS_FOR_FLICKER[notelights[thisNote]],LOW);
  //---end PLAY PACMAN INTRO SONG
  }
}

void play_MsPacMan_intro_song(byte added_divider) {
  //---PLAY MsPACMAN INTRO SONG---//
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
    tone(13, melody2[thisNote], noteDuration * 0.9);
    if(notelights2[thisNote] < 6){digitalWrite(PINS_FOR_FLICKER[notelights2[thisNote]],HIGH);} else
    {
      if((thisNote == 42) || (thisNote == 46)) 
      {
        digitalWrite(PINS_FOR_FLICKER[0], HIGH);
        digitalWrite(PINS_FOR_FLICKER[1], HIGH);
        digitalWrite(PINS_FOR_FLICKER[2], HIGH);
        digitalWrite(PINS_FOR_FLICKER[4], HIGH);
        digitalWrite(PINS_FOR_FLICKER[5], HIGH);
      }        
      if(thisNote > 42){digitalWrite(PINS_FOR_FLICKER[3], HIGH);}  
    }    
    // Wait for the specified duration before playing the next note.
    delay(noteDuration);
    noTone(13);
    for(int i=0; i<6; i+=1){digitalWrite(PINS_FOR_FLICKER[i],LOW);}
  //---end PLAY MsPACMAN INTRO SONG
  }
}

void Startup_Sequence()
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