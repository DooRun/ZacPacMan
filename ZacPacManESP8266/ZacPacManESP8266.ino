//  ZacPacMan8266

/*    
 * ESP8266 module supporing PacManAtmega.
 * This code will configure ESP8266 in station mode which will then connect with a local WIFI network. 
 * But if unable to connect to local WIFI network, it creates its own network.
 * Some ESP8266 generated website code taken form Rui Santos---https://randomnerdtutorials.com  
*/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

const char* ssid     = "007";
const char* password = "skyfall1";
//const char* ssid     = "DaveCave";
//const char* password = "ladytheresabasketballinmypudding";

// Set web server port number to 80
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
int MESSAGE_PART_LENGTH[99];

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 4000;

void setup() {
  Serial.begin(9600);
  MESSAGE_PART_LENGTH[11] = 5;
  MESSAGE_PART_LENGTH[12] = 5;
  MESSAGE_PART_LENGTH[13] = 5;
  // Connect to Wi-Fi network with SSID and password
  //Debug Serial.print("Connecting to ");
  //Debug Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Debug Serial.print(".");
  }
  // Print local IP address and start web server
  //Debug Serial.println("");
  //Debug Serial.println("WiFi connected.");
  //Debug Serial.println("IP address: ");
  //Debug Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
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
              c = client.read();  // read a byte
              c = client.read();  // read a byte
              c = client.read();  // read another byte to get past the '/'
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT = (c - 48)*10;

              c = client.read();  // read a byte
              MESSAGE_TO_ATMEGA += c;
              Serial.write(c);
              COMMAND_CAT += c - 48;
              message_length = MESSAGE_PART_LENGTH[COMMAND_CAT];

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
            client.println(".button { background-color: #77878A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #195B6A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ZachPACMAN</h1>");
            client.println("<p>MASTER ENABLE IS...</p>");   
            if(MESSAGE_TO_ATMEGA.substring(0,2) == "11")
            {
              if (MESSAGE_TO_ATMEGA =="11:0"){client.println("<p><a href=\"11:1,\"><button class=\"button\">OFF</button></a></p>");}
              else {client.println("<p><a href=\"11:0,\"><button class=\"button button2\">ON</button></a></p>");}
            }   
               
            client.println("<p>SOUND ENABLE IS...</p>");    
            if(MESSAGE_TO_ATMEGA.substring(0,2) == "12") 
            { 
              if (MESSAGE_TO_ATMEGA =="12:0"){client.println("<p><a href=\"12:1,\"><button class=\"button\">OFF</button></a></p>");}
              else {client.println("<p><a href=\"12:0,\"><button class=\"button button2\">ON</button></a></p>");} 
            }

            client.println("<p>LIGHT ENABLE IS...</p>");   
            if(MESSAGE_TO_ATMEGA.substring(0,2) == "13")
            {   
              if (MESSAGE_TO_ATMEGA =="13:0"){client.println("<p><a href=\"13:1,\"><button class=\"button\">OFF</button></a></p>");}
              else {client.println("<p><a href=\"13:0,\"><button class=\"button button2\">ON</button></a></p>");} 
            }
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
