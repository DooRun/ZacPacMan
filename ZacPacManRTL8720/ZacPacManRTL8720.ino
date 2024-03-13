//***** ZacPacManRTL8720 *****//
//  This is the code for the wireless modcule RTL8720DN_BW16
//  It acts a server to create an html page for clients to control the ZacPacMan Shelf
//  After a client loads the webpage on their device, a click on a button will generate a GET request to the ZacPacMan RTL8720DN_BW16 module
//  The RTL8720DN_BW16 module code (below) extracts the command after "GET" is found and then sens the command via serial communication to the ZacPacMan Atmega
//  
/* Command reference guide:
  11:#,Everything Enable,
  12:#,Sound Enable,
  13:#,Light Enable,
  14:#,Motion Enable,
  15:#,Clock Enable,
  16:####,Room Light Sensor Trigger Value,
  17:##,Performance number,
  18:#,Light Pinky,
  19:#,Light Clyde,
  20:#,Light Cherry,
  21:#,Light PacMan,
  22:#,Light Blinky,
  23:#,Light Inky.
*/

#include <WiFi.h>
#include <wifi_conf.h>

//---Brookings programming version below---

char ssid1[] = "007";
char pass1[] = "skyfall1";
byte status = WL_IDLE_STATUS;

WiFiServer server(80);
 
String MESSAGE_TO_ATMEGA = ""; 

unsigned long currentTime = millis();  // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 100; // Define timeout time in milliseconds (example: 2000ms = 2s)
unsigned char c;                   // char read by client from server http reply
//unsigned char data_line [100];     // this will be an array holding the invidiually read data points in ASCII value starting after ZPMZ is found and until x is found.
short data_count;     // counter for number of characters stored or printed.
bool break_out = 0;  // used to exit several nested loops once all important part of message is received externally from phone or website.

int p; // p is starting position for sbustring functions
int Master_Enable;
int Light_Enable;
int Sound_Enable;
int Motion_Enable;
int Clock_Enable;
int Light_Pinky;
int Light_Clyde;
int Light_Cherry;
int Light_PacMan;
int Light_Blinky;
int Light_Inky;
int Light_Sensor_Enable;
int Light_Sensor_trigger_value;
int Night_Light_Enable_and_mode;
int Alarm_Enable;
String Alarm_Date;
String Alarm_Time;
int Performance_number;
int Stayin_Game_Status;
int myCharNum;
String ActualName;
int BankAfterSpend;
int Attack_balance;
long Attack_or_Donate_Flag;
long Will_or_No;
long Mirror_or_No;
int Attack_or_Donate_Pinky_Amnt;
int Attack_or_Donate_Clyde_Amnt;
int Attack_or_Donate_Cherry_Amnt;
int Attack_or_Donate_PacMan_Amnt;
int Attack_or_Donate_Blinky_Amnt;
int Attack_or_Donate_Inky_Amnt;
int Mirror_Balances_Pinky_Values;
int Mirror_Balances_Clyde_Values;
int Mirror_Balances_Cherry_Values;
int Mirror_Balances_Pacman_Values;
int Mirror_Balances_Blinky_Values;
int Mirror_Balances_Inky_Values;

void setup() 
{
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, HIGH);
  delay(300);
  digitalWrite(LED_G, LOW);
  delay(300);

  Serial.begin(9600);
  
  //Initialize serial and wait for port to open:
  while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only

  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);

  while (status != WL_CONNECTED)  // attempt to connect to Wifi network:
  {
    Serial.println("Attempting to connect to SSID: ");
    Serial.println(ssid1);
    status = WiFi.begin(ssid1, pass1);    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(5000);    // wait 5 seconds for connection:
    if(status == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println(".....................successfully connected to network.");
      break;
    }
    Serial.println("..................failed to connect to network");
  }
    
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_G, HIGH);
  delay(1000);
  digitalWrite(LED_G, LOW);

  server.begin();
  // printWifiStatus();  // you're connected now, so print out the status:
}


void loop() 
{
  break_out = 0;
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    digitalWrite(LED_B, HIGH); // light indicator connection was made.
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();     
      if (client.available())  // if there are bytes to read from the client,
      {    
        char c = client.read(); // read a byte
        //DEBUG Serial.write(c);
        if(c == 90)   // Z was found
        {
          c = client.read();    // read a byte 
          //DEBUG Serial.write(c);  
          if(c == 80)    // P was found
          {
            c = client.read();    // read a byte 
            //DEBUG Serial.write(c);
            if(c == 77)   // M was found
            {
              c = client.read();    // read a byte 
              //DEBUG Serial.write(c);
              if(c == 90)   // Z was found for a total of ZPMZ
              {
                Serial.write(90);  // Z <======= Need to write ZPMZ since it was found and removed from serial buffer...
                Serial.write(80);  // P
                Serial.write(77);  // M
                Serial.write(90);  // Z
                delay(1);
                digitalWrite(LED_B, LOW); // light indicator connection was made.
                digitalWrite(LED_G, HIGH); // light indicator Tthat ZPMZ was found.
                data_count = 0;
                while (client.available())  // while there are bytes to read from the client,
                {
                  data_count += 1;
                  c = client.read(); // read a byte
                  Serial.write(c);  //<======= DO NOT comment this Serial.write statement as part of disabling DEBUG
                  MESSAGE_TO_ATMEGA += c;
                  if(c==94){break_out=1;}  // 94 is the ASCII value for ^.
                  if(break_out==1){break;}
                } 
              }
            }
          }
        }
        if(break_out==1){break;}
      }
      if(break_out==1){break;}
    }
    /*
    Serial.println();  */
    //Serial.println(MESSAGE_TO_ATMEGA);  
    /*
    //<--  //DEBUG  this statement in final code.
    Serial.println(MESSAGE_TO_ATMEGA.indexOf("12:"));
    int q = MESSAGE_TO_ATMEGA.indexOf("12:")+3;
    Serial.print("q = ");Serial.println(q);
    int r = MESSAGE_TO_ATMEGA.substring(q,q+1).toInt();
    Serial.print("r = ");Serial.println(r);
    String s = MESSAGE_TO_ATMEGA.substring(q,q+3);
    Serial.print("String s = ");Serial.println(s);

    p=MESSAGE_TO_ATMEGA.indexOf("11:")+3;if(p>-1){Master_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Master_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("12:")+3;if(p>-1){Light_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("13:")+3;if(p>-1){Sound_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Sound_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("14:")+3;if(p>-1){Motion_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Motion_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("15:")+3;if(p>-1){Clock_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Clock_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("16:")+3;if(p>-1){Light_Pinky=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Pinky);
    p=MESSAGE_TO_ATMEGA.indexOf("17:")+3;if(p>-1){Light_Clyde=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Clyde);
    p=MESSAGE_TO_ATMEGA.indexOf("18:")+3;if(p>-1){Light_Cherry=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Cherry);
    p=MESSAGE_TO_ATMEGA.indexOf("19:")+3;if(p>-1){Light_PacMan=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_PacMan);
    p=MESSAGE_TO_ATMEGA.indexOf("10:")+3;if(p>-1){Light_Blinky=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Blinky);
    p=MESSAGE_TO_ATMEGA.indexOf("21:")+3;if(p>-1){Light_Inky=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Inky);
    p=MESSAGE_TO_ATMEGA.indexOf("22:")+3;if(p>-1){Light_Sensor_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Light_Sensor_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("23:")+3;if(p>-1){Light_Sensor_trigger_value= (MESSAGE_TO_ATMEGA.substring(p,p+5).toInt());};Serial.println(Light_Sensor_trigger_value);
    p=MESSAGE_TO_ATMEGA.indexOf("24:")+3;if(p>-1){Night_Light_Enable_and_mode=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Night_Light_Enable_and_mode);

    p=MESSAGE_TO_ATMEGA.indexOf("26:")+3;if(p>-1){Alarm_Enable=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Alarm_Enable);
    p=MESSAGE_TO_ATMEGA.indexOf("27:")+3;if(p>-1){Alarm_Date=(MESSAGE_TO_ATMEGA.substring(p,p+8));};Serial.println(Alarm_Date);
    p=MESSAGE_TO_ATMEGA.indexOf("28:")+3;if(p>-1){Alarm_Time=(MESSAGE_TO_ATMEGA.substring(p,p+8));};Serial.println(Alarm_Time);
    p=MESSAGE_TO_ATMEGA.indexOf("29:")+3;if(p>-1){Performance_number=(MESSAGE_TO_ATMEGA.substring(p,p+2).toInt());};Serial.println(Performance_number);
    
    p=MESSAGE_TO_ATMEGA.indexOf("41:")+3;if(p>-1){Stayin_Game_Status=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(Stayin_Game_Status);
    p=MESSAGE_TO_ATMEGA.indexOf("42:")+3;if(p>-1){myCharNum=(MESSAGE_TO_ATMEGA.substring(p,p+1).toInt());};Serial.println(myCharNum);
    p=MESSAGE_TO_ATMEGA.indexOf("43:")+3;if(p>-1){ActualName=(MESSAGE_TO_ATMEGA.substring(p,p+7));};Serial.println(ActualName);
    p=MESSAGE_TO_ATMEGA.indexOf("44:")+3;if(p>-1){BankAfterSpend=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(BankAfterSpend);
    p=MESSAGE_TO_ATMEGA.indexOf("45:")+3;if(p>-1){Attack_balance=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_balance);
    p=MESSAGE_TO_ATMEGA.indexOf("46:")+3;if(p>-1){Attack_or_Donate_Flag=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Attack_or_Donate_Flag);
    p=MESSAGE_TO_ATMEGA.indexOf("47:")+3;if(p>-1){Will_or_No=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Will_or_No);
    p=MESSAGE_TO_ATMEGA.indexOf("48:")+3;if(p>-1){Mirror_or_No=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_or_No);
    p=MESSAGE_TO_ATMEGA.indexOf("49:")+3;if(p>-1){Attack_or_Donate_Pinky_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_Pinky_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("50:")+3;if(p>-1){Attack_or_Donate_Clyde_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_Clyde_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("51:")+3;if(p>-1){Attack_or_Donate_Cherry_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_Cherry_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("52:")+3;if(p>-1){Attack_or_Donate_PacMan_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_PacMan_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("53:")+3;if(p>-1){Attack_or_Donate_Blinky_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_Blinky_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("54:")+3;if(p>-1){Attack_or_Donate_Inky_Amnt=(MESSAGE_TO_ATMEGA.substring(p,p+4).toInt());};Serial.println(Attack_or_Donate_Inky_Amnt);
    p=MESSAGE_TO_ATMEGA.indexOf("55:")+3;if(p>-1){Mirror_Balances_Pinky_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Pinky_Values);
    p=MESSAGE_TO_ATMEGA.indexOf("56:")+3;if(p>-1){Mirror_Balances_Clyde_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Clyde_Values);
    p=MESSAGE_TO_ATMEGA.indexOf("57:")+3;if(p>-1){Mirror_Balances_Cherry_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Cherry_Values);
    p=MESSAGE_TO_ATMEGA.indexOf("58:")+3;if(p>-1){Mirror_Balances_Pacman_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Pacman_Values);
    p=MESSAGE_TO_ATMEGA.indexOf("59:")+3;if(p>-1){Mirror_Balances_Blinky_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Blinky_Values);
    p=MESSAGE_TO_ATMEGA.indexOf("60:")+3;if(p>-1){Mirror_Balances_Inky_Values=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Mirror_Balances_Inky_Values);
    //p=MESSAGE_TO_ATMEGA.indexOf("61:")+3;if(p>-1){Pinky_Rename=(MESSAGE_TO_ATMEGA.substring(p,p+7).toInt());};Serial.println(Pinky_Rename);
  

    //----- PROCESS MESSAGE FROM ANDROID -----//
    //----- first determine if in stayin-alive game mode.  All necessary stayin-alive code will reside here to save on Atmega chip memory. -----//
    Serial.println("");
    Serial.print("MESSAGE_TO_ATMEGA =");
    Serial.println(MESSAGE_TO_ATMEGA);
    Serial.println("");
    */

    digitalWrite(LED_G, LOW); // light indicator Tthat ZPMZ was found.
    digitalWrite(LED_B, HIGH); // light indicator connection was made.
    client.flush();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println(MESSAGE_TO_ATMEGA);
    client.println("Connection: close");
    client.println();
    client.println();   // The HTTP response ends with another blank line     
    MESSAGE_TO_ATMEGA = "";    
    digitalWrite(LED_B, LOW); // light indicator connection was made.
    client.println();
    client.stop(); // Close the connection
  }
}