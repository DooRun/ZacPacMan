#include <WiFi.h>

char ssid[] = "007";    // your network SSID (name)
char pass[] = "skyfall1";       // your network password
int keyIndex = 0;               // your network key Index number (needed only for WEP)
//const char* ssid     = "DaveCave";
//const char* password = "ladytheresabasketballinmypudding";
//int keyIndex = 0;               // your network key Index number (needed only for WEP)

//DELETE   State the analog pin that you want to read
//DELETE   int analogChannel = PB3;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

/* Command reference guide:
  1  11:#,Everything Enable,
  2  12:#,Sound Enable,
  3  13:#,Light Enable,
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
 
String MESSAGE_TO_ATMEGA = ""; 
int message_length;
int COMMAND_CAT;
bool currentLineIsBlank;
int MESSAGE_PART_LENGTH[99];

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 4000;

String M_EN_STATE = "MASTER ON ";     // Master enable
String S_EN_STATE = "SOUND ON ";      // Light enable
String L_EN_STATE = "LIGHT ON ";      // Sound enable
String MO_EN_STATE = "MOTION ON ";    // Motion enable
String CL_EN_STATE = "CLOCK ON ";     // Clock enable
String PIN_EN_STATE = "PINKY ON ";    // Light Pinky  PIN
String CLY_EN_STATE = "CLYDE ON ";    // Light Clyde  CLY
String CHE_EN_STATE = "CHERRY ON ";   // Light Cherry CHE
String PAC_EN_STATE = "PACMAN ON ";   // Light PacMan PAC
String BLI_EN_STATE = "BLINKY ON ";   // Light Blinky BLI
String INK_EN_STATE = "INKY ON ";     // Light Inky   INK
String LS_EN_STATE = "LIGHT SENSOR ON ";    // LIGHT SENSOR enable


void setup() 
{
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, HIGH);
  delay(300);
  digitalWrite(LED_G, LOW);
  delay(300);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
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

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
    
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_G, HIGH);
  delay(1000);
  digitalWrite(LED_G, LOW);

  server.begin();
  // you're connected now, so print out the status:
  // printWifiStatus();
}


void loop() 
{
  
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    //Debug Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available())  // if there's bytes to read from the client,
      {
        char c = client.read(); // read a byte
        if(c == 'G')
        {
          c = client.read();    // read a byte         
          if(c == 'E')
          {
            c = client.read();    // read a byte         
            if(c == 'T') //GET is found
            {
              c = client.read();  // read a byte to get past a space
              c = client.read();  // read another byte to get past the '/'
              c = client.read();  // read the first byte of interest, i.e., the first byte of the two digit command category.
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT = (c - 48)*10;

              c = client.read();  // read a byte
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT += c - 48;
              message_length = MESSAGE_PART_LENGTH[COMMAND_CAT];  // this determines the length of the command's value and set limit to # of reads below.

              //header = 'GET'
              for (int i = 0; i<=message_length-4; i=i+1)
              {
                char c = client.read();
                Serial.write(c);
                MESSAGE_TO_ATMEGA += c;
              }
              Serial.write('.');
              Serial.write('\n');
            }
          }          
        }
        if (c == '\n') 
        {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
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
            
            // Web Page Heading
            client.println("<body><h1>ZACPACMAN</h1>");

            if (MESSAGE_TO_ATMEGA =="11:0"){M_EN_STATE = "MASTER OFF";}
            if (MESSAGE_TO_ATMEGA =="11:1"){M_EN_STATE = "MASTER ON ";}
            if (MESSAGE_TO_ATMEGA =="12:0"){L_EN_STATE = "LIGHT OFF";}
            if (MESSAGE_TO_ATMEGA =="12:1"){L_EN_STATE = "LIGHT ON ";}
            if (MESSAGE_TO_ATMEGA =="13:0"){S_EN_STATE = "SOUND OFF";}
            if (MESSAGE_TO_ATMEGA =="13:1"){S_EN_STATE = "SOUND ON ";}
            if (MESSAGE_TO_ATMEGA =="14:0"){MO_EN_STATE = "MOTION OFF";}
            if (MESSAGE_TO_ATMEGA =="14:1"){MO_EN_STATE = "MOTION ON ";}
            if (MESSAGE_TO_ATMEGA =="15:0"){CL_EN_STATE = "CLOCK OFF";}
            if (MESSAGE_TO_ATMEGA =="15:1"){CL_EN_STATE = "CLOCK ON ";}
            if (MESSAGE_TO_ATMEGA =="16:0"){PIN_EN_STATE = "PINKY OFF";}
            if (MESSAGE_TO_ATMEGA =="16:1"){PIN_EN_STATE = "PINKY ON ";}
            if (MESSAGE_TO_ATMEGA =="17:0"){CLY_EN_STATE = "CLYDE OFF";}
            if (MESSAGE_TO_ATMEGA =="17:1"){CLY_EN_STATE = "CLYDE ON ";}
            if (MESSAGE_TO_ATMEGA =="18:0"){CHE_EN_STATE = "CHERRY OFF";}
            if (MESSAGE_TO_ATMEGA =="18:1"){CHE_EN_STATE = "CHERRY ON ";}
            if (MESSAGE_TO_ATMEGA =="19:0"){PAC_EN_STATE = "PACMAN OFF";}
            if (MESSAGE_TO_ATMEGA =="19:1"){PAC_EN_STATE = "PACMAN ON ";}
            if (MESSAGE_TO_ATMEGA =="20:0"){BLI_EN_STATE = "BLINKY OFF";}
            if (MESSAGE_TO_ATMEGA =="20:1"){BLI_EN_STATE = "BLINKY ON ";}
            if (MESSAGE_TO_ATMEGA =="21:0"){INK_EN_STATE = "INKY OFF";}
            if (MESSAGE_TO_ATMEGA =="21:1"){INK_EN_STATE = "INKY ON ";}
            if (MESSAGE_TO_ATMEGA =="22:0"){LS_EN_STATE = "LIGHT SENSOR OFF";}
            if (MESSAGE_TO_ATMEGA =="22:1"){LS_EN_STATE = "LIGHT SENSOR ON ";}
   
            if (M_EN_STATE == "MASTER OFF"){client.println("<p><a href=\"11:1,\"><button class=\"button\">MASTER OFF</button></a></p>");}
            if (M_EN_STATE == "MASTER ON "){client.println("<p><a href=\"11:0,\"><button class=\"button button2\">MASTER ON </button></a></p>");}
                  
            if (L_EN_STATE == "LIGHT OFF"){client.println("<p><a href=\"12:1,\"><button class=\"button\">LIGHT OFF</button></a></p>");}
            if (L_EN_STATE == "LIGHT ON "){client.println("<p><a href=\"12:0,\"><button class=\"button button2\">LIGHT ON </button></a></p>");}
           
            if (S_EN_STATE == "SOUND OFF"){client.println("<p><a href=\"13:1,\"><button class=\"button\">SOUND OFF</button></a></p>");}
            if (S_EN_STATE == "SOUND ON "){client.println("<p><a href=\"13:0,\"><button class=\"button button2\">SOUND ON </button></a></p>");}
            
            if (MO_EN_STATE == "MOTION OFF"){client.println("<p><a href=\"14:1,\"><button class=\"button\">MOTION OFF</button></a></p>");}
            if (MO_EN_STATE == "MOTION ON "){client.println("<p><a href=\"14:0,\"><button class=\"button button2\">MOTION ON </button></a></p>");}
          
            if (CL_EN_STATE == "CLOCK OFF"){client.println("<p><a href=\"15:1,\"><button class=\"button\">CLOCK OFF</button></a></p>");}
            if (CL_EN_STATE == "CLOCK ON "){client.println("<p><a href=\"15:0,\"><button class=\"button button2\">CLOCK ON </button></a></p>");}
          
            if (PIN_EN_STATE == "PINKY OFF"){client.println("<p><a href=\"16:1,\"><button class=\"button\">PINKY OFF</button></a></p>");}
            if (PIN_EN_STATE == "PINKY ON "){client.println("<p><a href=\"16:0,\"><button class=\"button button2\">PINKY ON </button></a></p>");}
          
            if (CLY_EN_STATE == "CLYDE OFF"){client.println("<p><a href=\"17:1,\"><button class=\"button\">CLYDE OFF</button></a></p>");}
            if (CLY_EN_STATE == "CLYDE ON "){client.println("<p><a href=\"17:0,\"><button class=\"button button2\">CLYDE ON </button></a></p>");}
          
            if (CHE_EN_STATE == "CHERRY OFF"){client.println("<p><a href=\"18:1,\"><button class=\"button\">CHERRY OFF</button></a></p>");}
            if (CHE_EN_STATE == "CHERRY ON "){client.println("<p><a href=\"18:0,\"><button class=\"button button2\">CHERRY ON </button></a></p>");}
          
            if (PAC_EN_STATE == "PACMAN OFF"){client.println("<p><a href=\"19:1,\"><button class=\"button\">PACMAN OFF</button></a></p>");}
            if (PAC_EN_STATE == "PACMAN ON "){client.println("<p><a href=\"19:0,\"><button class=\"button button2\">PACMAN ON </button></a></p>");}
          
            if (BLI_EN_STATE == "BLINKY OFF"){client.println("<p><a href=\"20:1,\"><button class=\"button\">BLINKY OFF</button></a></p>");}
            if (BLI_EN_STATE == "BLINKY ON "){client.println("<p><a href=\"20:0,\"><button class=\"button button2\">BLINKY ON </button></a></p>");}
          
            if (INK_EN_STATE == "INKY OFF"){client.println("<p><a href=\"21:1,\"><button class=\"button\">INKY OFF</button></a></p>");}
            if (INK_EN_STATE == "INKY ON "){client.println("<p><a href=\"21:0,\"><button class=\"button button2\">INKY ON </button></a></p>");}
            
            MESSAGE_TO_ATMEGA = "";
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    //header = "";  // Clear the header variable
    client.stop(); // Close the connection
    //Debug Serial.println("Client disconnected.");
    //Debug Serial.println("");
  }
}
/*
  
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, HIGH);
    bool currentLineIsBlank = true;
    //delay(10);
    //Debug  Serial.println("new client"); // print a message out in the serial port
    //String currentLine = "";                // make a String to hold incoming data from the client
    //currentTime = millis();
    //previousTime = currentTime;
    while (client.connected()) //&& currentTime - previousTime <= timeoutTime) { // loop while the client's connected
    {
      //currentTime = millis();   
      if (client.available())  // if there's bytes to read from the client,
      {
        char c = client.read(); // read a byte
        if(c == 'G')
        {
          c = client.read();    // read a byte         
          if(c == 'E')
          {
            c = client.read();    // read a byte         
            if(c == 'T') //GET is found
            {
              c = client.read();  // read a byte to get past a space
              c = client.read();  // read another byte to get past the '/'
              c = client.read();  // read the first byte of interest, i.e., the first byte of the two digit command category.
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT = (c - 48)*10;

              c = client.read();  // read a byte
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT += c - 48;
              message_length = MESSAGE_PART_LENGTH[COMMAND_CAT];  // this determines the length of the command's value and set limit to # of reads below.

              //header = 'GET'
              for (int i = 0; i<=message_length-4; i=i+1)
              {
                char c = client.read();
                Serial.write(c);
                MESSAGE_TO_ATMEGA += c;
              }
              Serial.write('.');
              Serial.write('\n');
            }
          }          
        }      

        if (c == '\n' && currentLineIsBlank) 
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //DELETE??  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          // tamper starts
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
            // Web Page Heading
            client.println("<body><h1>ZACPACMAN</h1>");
            

            if (MESSAGE_TO_ATMEGA =="11:0"){M_EN_STATE = "MASTER OFF";}
            if (MESSAGE_TO_ATMEGA =="11:1"){M_EN_STATE = "MASTER ON ";}
            if (MESSAGE_TO_ATMEGA =="12:0"){L_EN_STATE = "LIGHT OFF";}
            if (MESSAGE_TO_ATMEGA =="12:1"){L_EN_STATE = "LIGHT ON ";}
            if (MESSAGE_TO_ATMEGA =="13:0"){S_EN_STATE = "SOUND OFF";}
            if (MESSAGE_TO_ATMEGA =="13:1"){S_EN_STATE = "SOUND ON ";}
            if (MESSAGE_TO_ATMEGA =="14:0"){MO_EN_STATE = "MOTION OFF";}
            if (MESSAGE_TO_ATMEGA =="14:1"){MO_EN_STATE = "MOTION ON ";}
            if (MESSAGE_TO_ATMEGA =="15:0"){CL_EN_STATE = "CLOCK OFF";}
            if (MESSAGE_TO_ATMEGA =="15:1"){CL_EN_STATE = "CLOCK ON ";}
            if (MESSAGE_TO_ATMEGA =="16:0"){PIN_EN_STATE = "PINKY OFF";}
            if (MESSAGE_TO_ATMEGA =="16:1"){PIN_EN_STATE = "PINKY ON ";}
            if (MESSAGE_TO_ATMEGA =="17:0"){CLY_EN_STATE = "CLYDE OFF";}
            if (MESSAGE_TO_ATMEGA =="17:1"){CLY_EN_STATE = "CLYDE ON ";}
            if (MESSAGE_TO_ATMEGA =="18:0"){CHE_EN_STATE = "CHERRY OFF";}
            if (MESSAGE_TO_ATMEGA =="18:1"){CHE_EN_STATE = "CHERRY ON ";}
            if (MESSAGE_TO_ATMEGA =="19:0"){PAC_EN_STATE = "PACMAN OFF";}
            if (MESSAGE_TO_ATMEGA =="19:1"){PAC_EN_STATE = "PACMAN ON ";}
            if (MESSAGE_TO_ATMEGA =="20:0"){BLI_EN_STATE = "BLINKY OFF";}
            if (MESSAGE_TO_ATMEGA =="20:1"){BLI_EN_STATE = "BLINKY ON ";}
            if (MESSAGE_TO_ATMEGA =="21:0"){INK_EN_STATE = "INKY OFF";}
            if (MESSAGE_TO_ATMEGA =="21:1"){INK_EN_STATE = "INKY ON ";}
            if (MESSAGE_TO_ATMEGA =="22:0"){LS_EN_STATE = "LIGHT SENSOR OFF";}
            if (MESSAGE_TO_ATMEGA =="22:1"){LS_EN_STATE = "LIGHT SENSOR ON ";}
   
            if (M_EN_STATE == "MASTER OFF"){client.println("<p><a href=\"11:1,\"><button class=\"button\">MASTER OFF</button></a></p>");}
            if (M_EN_STATE == "MASTER ON "){client.println("<p><a href=\"11:0,\"><button class=\"button button2\">MASTER ON </button></a></p>");}
                  
            if (L_EN_STATE == "LIGHT OFF"){client.println("<p><a href=\"12:1,\"><button class=\"button\">LIGHT OFF</button></a></p>");}
            if (L_EN_STATE == "LIGHT ON "){client.println("<p><a href=\"12:0,\"><button class=\"button button2\">LIGHT ON </button></a></p>");}
           
            if (S_EN_STATE == "SOUND OFF"){client.println("<p><a href=\"13:1,\"><button class=\"button\">SOUND OFF</button></a></p>");}
            if (S_EN_STATE == "SOUND ON "){client.println("<p><a href=\"13:0,\"><button class=\"button button2\">SOUND ON </button></a></p>");}
            
            if (MO_EN_STATE == "MOTION OFF"){client.println("<p><a href=\"14:1,\"><button class=\"button\">MOTION OFF</button></a></p>");}
            if (MO_EN_STATE == "MOTION ON "){client.println("<p><a href=\"14:0,\"><button class=\"button button2\">MOTION ON </button></a></p>");}
          
            if (CL_EN_STATE == "CLOCK OFF"){client.println("<p><a href=\"15:1,\"><button class=\"button\">CLOCK OFF</button></a></p>");}
            if (CL_EN_STATE == "CLOCK ON "){client.println("<p><a href=\"15:0,\"><button class=\"button button2\">CLOCK ON </button></a></p>");}
          
            if (PIN_EN_STATE == "PINKY OFF"){client.println("<p><a href=\"16:1,\"><button class=\"button\">PINKY OFF</button></a></p>");}
            if (PIN_EN_STATE == "PINKY ON "){client.println("<p><a href=\"16:0,\"><button class=\"button button2\">PINKY ON </button></a></p>");}
          
            if (CLY_EN_STATE == "CLYDE OFF"){client.println("<p><a href=\"17:1,\"><button class=\"button\">CLYDE OFF</button></a></p>");}
            if (CLY_EN_STATE == "CLYDE ON "){client.println("<p><a href=\"17:0,\"><button class=\"button button2\">CLYDE ON </button></a></p>");}
          
            if (CHE_EN_STATE == "CHERRY OFF"){client.println("<p><a href=\"18:1,\"><button class=\"button\">CHERRY OFF</button></a></p>");}
            if (CHE_EN_STATE == "CHERRY ON "){client.println("<p><a href=\"18:0,\"><button class=\"button button2\">CHERRY ON </button></a></p>");}
          
            if (PAC_EN_STATE == "PACMAN OFF"){client.println("<p><a href=\"19:1,\"><button class=\"button\">PACMAN OFF</button></a></p>");}
            if (PAC_EN_STATE == "PACMAN ON "){client.println("<p><a href=\"19:0,\"><button class=\"button button2\">PACMAN ON </button></a></p>");}
          
            if (BLI_EN_STATE == "BLINKY OFF"){client.println("<p><a href=\"20:1,\"><button class=\"button\">BLINKY OFF</button></a></p>");}
            if (BLI_EN_STATE == "BLINKY ON "){client.println("<p><a href=\"20:0,\"><button class=\"button button2\">BLINKY ON </button></a></p>");}
          
            if (INK_EN_STATE == "INKY OFF"){client.println("<p><a href=\"21:1,\"><button class=\"button\">INKY OFF</button></a></p>");}
            if (INK_EN_STATE == "INKY ON "){client.println("<p><a href=\"21:0,\"><button class=\"button button2\">INKY ON </button></a></p>");}
            
            MESSAGE_TO_ATMEGA = "";
            client.println("</body></html>");
          // tamper ends

          //DELETE  int sensorReading = analogRead(analogChannel);
          //client.print("Request received.");
          //DELETE  client.print(sensorReading);
          client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    
  }
  delay(1);
}


void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
*/