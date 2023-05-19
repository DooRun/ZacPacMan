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

char ssid1[] = "DaveCave";
char pass1[] = "ladytheresabasketballinmypudding";
//int keyIndex1 = 0;        // your network key Index number (needed only for WEP)
char ssid2[] = "007";       // second network SSID (name)
char pass2[] = "skyfall1";  // second network password
//int keyIndex2 = 0;        // your network key Index number (needed only for WEP)

byte status = WL_IDLE_STATUS;

WiFiServer server(80);
 
String MESSAGE_TO_ATMEGA = ""; 
byte MESSAGE_PART_LENGTH[99];

unsigned long currentTime = millis();  // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 4000; // Define timeout time in milliseconds (example: 2000ms = 2s)
unsigned char c;                   // char read by client from server http reply
//unsigned char data_line [100];     // this will be an array holding the invidiually read data points in ASCII value starting after ZPMZ is found and until x is found.
short data_count;     // counter for number of characters stored or printed.
bool break_out = 0;  // used to exit several nested loops once all important part of message is received externally from phone or website.

String M_EN_STATE = "MASTER ON ";       // Master enable
String S_EN_STATE = "SOUND ON ";        // Light enable
String L_EN_STATE = "LIGHT ON ";        // Sound enable
String MO_EN_STATE = "MOTION ON ";      // Motion enable
String CL_EN_STATE = "CLOCK ON ";       // Clock enable
String PIN_EN_STATE = "PINKY ON ";      // Light Pinky  PIN
String CLY_EN_STATE = "CLYDE ON ";      // Light Clyde  CLY
String CHE_EN_STATE = "CHERRY ON ";     // Light Cherry CHE
String PAC_EN_STATE = "PACMAN ON ";     // Light PacMan PAC
String BLI_EN_STATE = "BLINKY ON ";     // Light Blinky BLI
String INK_EN_STATE = "INKY ON ";       // Light Inky   INK
String LS_EN_STATE = "LIGHT SENSOR ON ";// LIGHT SENSOR enable

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
  for (int i = 0; i<99; i=i+1){MESSAGE_PART_LENGTH[i]=8;}

  MESSAGE_PART_LENGTH[10] = 5; // Lambda, i.e., an unassigned message to send to do nothing but refresh webpage
  MESSAGE_PART_LENGTH[11] = 5; // Master enable
  MESSAGE_PART_LENGTH[12] = 5; // Light enable
  MESSAGE_PART_LENGTH[13] = 5; // Sound enable
  MESSAGE_PART_LENGTH[14] = 5; // Motion enable
  MESSAGE_PART_LENGTH[15] = 5; // Clock enable
  MESSAGE_PART_LENGTH[16] = 5; // Light Pinky  PIN
  MESSAGE_PART_LENGTH[17] = 5; // Light Clyde  CLY
  MESSAGE_PART_LENGTH[18] = 5; // Light Cherry CHE
  MESSAGE_PART_LENGTH[19] = 5; // Light PacMan PAC
  MESSAGE_PART_LENGTH[20] = 5; // Light Blinky BLI
  MESSAGE_PART_LENGTH[21] = 5; // Light Inky   INK
  MESSAGE_PART_LENGTH[22] = 5; // Light Sensor enable
  MESSAGE_PART_LENGTH[23] = 8; // Light Sensor trigger value
  MESSAGE_PART_LENGTH[24] = 6; // Performance number
  
  //Initialize serial and wait for port to open:
  while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only
  // check for the presence of the shield:
  //if (WiFi.status() == WL_NO_SHIELD) 
  //{
  //  Serial.println("WiFi shield not present");
  //  digitalWrite(LED_R, HIGH);
  //  while (true);  // don't continue:
  //  }

  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);

  while (status != WL_CONNECTED)  // attempt to connect to Wifi network:
  {
    //DEBUG Serial.println("Attempting to connect to SSID: ");
    //DEBUG Serial.println(ssid1);
    status = WiFi.begin(ssid1, pass1);    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(5000);    // wait 5 seconds for connection:
    if(status == WL_CONNECTED)
    {
      //DEBUG Serial.println("");
      //DEBUG Serial.println(".....................successfully connected to first network.");
      break;
    }
    //DEBUG Serial.println("..................failed to connect to 1st");
    //DEBUG Serial.println("......................Attempting second network");
    //DEBUG Serial.println(ssid2);
    delay(1000);
    status = WiFi.begin(ssid2, pass2);    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(5000);    // wait 5 seconds for connection:
    if(status = WL_CONNECTED)
    {
      //DEBUG Serial.println("");
      //DEBUG Serial.println(".............................connected to second");
    }
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
                  if(c==120){break_out=1;}  // 120 is the ASCII value for lower case x.
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
    //DEBUG Serial.println("");
    //DEBUG Serial.print("MESSAGE_TO_ATMEGA =");
    //DEBUG Serial.println(MESSAGE_TO_ATMEGA);
    //DEBUG Serial.println("");

    digitalWrite(LED_G, LOW); // light indicator Tthat ZPMZ was found.
    digitalWrite(LED_B, HIGH); // light indicator connection was made.
    client.flush();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    // Display the HTML web page
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    // CSS to style the on/off buttons 
    // Feel free to change the background-color and font-size attributes to fit your preferences
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button {background-color: #77878A; border: none; color: white; padding: 8px 20px;");
    client.println("text-decoration: none; font-size: 20px; margin: 1px; cursor: pointer;}");
    client.println(".button2 {background-color: #195B6A;}</style></head>");
    

    client.println("<body><h1>ZACPACMAN</h1>");    // Web Page Heading

    if (MESSAGE_TO_ATMEGA =="11:0x"){M_EN_STATE = "MASTER OFF";}
    if (MESSAGE_TO_ATMEGA =="11:1x"){M_EN_STATE = "MASTER ON ";}
    if (MESSAGE_TO_ATMEGA =="12:0x"){L_EN_STATE = "LIGHT OFF";}
    if (MESSAGE_TO_ATMEGA =="12:1x"){L_EN_STATE = "LIGHT ON ";}
    if (MESSAGE_TO_ATMEGA =="13:0x"){S_EN_STATE = "SOUND OFF";}
    if (MESSAGE_TO_ATMEGA =="13:1x"){S_EN_STATE = "SOUND ON ";}
    if (MESSAGE_TO_ATMEGA =="14:0x"){MO_EN_STATE = "MOTION OFF";}
    if (MESSAGE_TO_ATMEGA =="14:1x"){MO_EN_STATE = "MOTION ON ";}
    if (MESSAGE_TO_ATMEGA =="15:0x"){CL_EN_STATE = "CLOCK OFF";}
    if (MESSAGE_TO_ATMEGA =="15:1x"){CL_EN_STATE = "CLOCK ON ";}
    if (MESSAGE_TO_ATMEGA =="16:0x"){PIN_EN_STATE = "PINKY OFF";}
    if (MESSAGE_TO_ATMEGA =="16:1x"){PIN_EN_STATE = "PINKY ON ";}
    if (MESSAGE_TO_ATMEGA =="17:0x"){CLY_EN_STATE = "CLYDE OFF";}
    if (MESSAGE_TO_ATMEGA =="17:1x"){CLY_EN_STATE = "CLYDE ON ";}
    if (MESSAGE_TO_ATMEGA =="18:0x"){CHE_EN_STATE = "CHERRY OFF";}
    if (MESSAGE_TO_ATMEGA =="18:1x"){CHE_EN_STATE = "CHERRY ON ";}
    if (MESSAGE_TO_ATMEGA =="19:0x"){PAC_EN_STATE = "PACMAN OFF";}
    if (MESSAGE_TO_ATMEGA =="19:1x"){PAC_EN_STATE = "PACMAN ON ";}
    if (MESSAGE_TO_ATMEGA =="20:0x"){BLI_EN_STATE = "BLINKY OFF";}
    if (MESSAGE_TO_ATMEGA =="20:1x"){BLI_EN_STATE = "BLINKY ON ";}
    if (MESSAGE_TO_ATMEGA =="21:0x"){INK_EN_STATE = "INKY OFF";}
    if (MESSAGE_TO_ATMEGA =="21:1x"){INK_EN_STATE = "INKY ON ";}
    if (MESSAGE_TO_ATMEGA =="22:0x"){LS_EN_STATE = "LIGHT SENSOR OFF";}
    if (MESSAGE_TO_ATMEGA =="22:1x"){LS_EN_STATE = "LIGHT SENSOR ON ";}

    if (M_EN_STATE == "MASTER OFF"){client.println("<p><a href=\"ZPMZ11:1x,\"><button class=\"button\">MASTER OFF</button></a></p>");}
    if (M_EN_STATE == "MASTER ON "){client.println("<p><a href=\"ZPMZ11:0x,\"><button class=\"button button2\">MASTER ON </button></a></p>");}
    if (L_EN_STATE == "LIGHT OFF"){client.println("<p><a href=\"ZPMZ12:1x,\"><button class=\"button\">LIGHT OFF</button></a></p>");}
    if (L_EN_STATE == "LIGHT ON "){client.println("<p><a href=\"ZPMZ12:0x,\"><button class=\"button button2\">LIGHT ON </button></a></p>");}
    if (S_EN_STATE == "SOUND OFF"){client.println("<p><a href=\"ZPMZ13:1x,\"><button class=\"button\">SOUND OFF</button></a></p>");}
    if (S_EN_STATE == "SOUND ON "){client.println("<p><a href=\"ZPMZ13:0x,\"><button class=\"button button2\">SOUND ON </button></a></p>");}
    if (MO_EN_STATE == "MOTION OFF"){client.println("<p><a href=\"ZPMZ14:1x,\"><button class=\"button\">MOTION OFF</button></a></p>");}
    if (MO_EN_STATE == "MOTION ON "){client.println("<p><a href=\"ZPMZ14:0x,\"><button class=\"button button2\">MOTION ON </button></a></p>");}
    if (CL_EN_STATE == "CLOCK OFF"){client.println("<p><a href=\"ZPMZ15:1x,\"><button class=\"button\">CLOCK OFF</button></a></p>");}
    if (CL_EN_STATE == "CLOCK ON "){client.println("<p><a href=\"ZPMZ15:0x,\"><button class=\"button button2\">CLOCK ON </button></a></p>");}
    if (PIN_EN_STATE == "PINKY OFF"){client.println("<p><a href=\"ZPMZ16:1x,\"><button class=\"button\">PINKY OFF</button></a></p>");}
    if (PIN_EN_STATE == "PINKY ON "){client.println("<p><a href=\"ZPMZ16:0x,\"><button class=\"button button2\">PINKY ON </button></a></p>");}
    if (CLY_EN_STATE == "CLYDE OFF"){client.println("<p><a href=\"ZPMZ17:1x,\"><button class=\"button\">CLYDE OFF</button></a></p>");}
    if (CLY_EN_STATE == "CLYDE ON "){client.println("<p><a href=\"ZPMZ17:0x,\"><button class=\"button button2\">CLYDE ON </button></a></p>");}
    if (CHE_EN_STATE == "CHERRY OFF"){client.println("<p><a href=\"ZPMZ18:1x,\"><button class=\"button\">CHERRY OFF</button></a></p>");}
    if (CHE_EN_STATE == "CHERRY ON "){client.println("<p><a href=\"ZPMZ18:0x,\"><button class=\"button button2\">CHERRY ON </button></a></p>");}
    if (PAC_EN_STATE == "PACMAN OFF"){client.println("<p><a href=\"ZPMZ19:1x,\"><button class=\"button\">PACMAN OFF</button></a></p>");}
    if (PAC_EN_STATE == "PACMAN ON "){client.println("<p><a href=\"ZPMZ19:0x,\"><button class=\"button button2\">PACMAN ON </button></a></p>");}
    if (BLI_EN_STATE == "BLINKY OFF"){client.println("<p><a href=\"ZPMZ20:1x,\"><button class=\"button\">BLINKY OFF</button></a></p>");}
    if (BLI_EN_STATE == "BLINKY ON "){client.println("<p><a href=\"ZPMZ20:0x,\"><button class=\"button button2\">BLINKY ON </button></a></p>");}
    if (INK_EN_STATE == "INKY OFF"){client.println("<p><a href=\"ZPMZ21:1x,\"><button class=\"button\">INKY OFF</button></a></p>");}
    if (INK_EN_STATE == "INKY ON "){client.println("<p><a href=\"ZPMZ21:0x,\"><button class=\"button button2\">INKY ON </button></a></p>");}

    client.println("</body></html>");
    client.println();   // The HTTP response ends with another blank line     
    MESSAGE_TO_ATMEGA = "";    
    digitalWrite(LED_B, LOW); // light indicator connection was made.
    client.println();
    client.stop(); // Close the connection
  }
}