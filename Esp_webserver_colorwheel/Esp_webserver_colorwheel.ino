// Load Wi-Fi library
#include <ESP8266WiFi.h>


#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#define NUM_LEDS 26
#define DATA_PIN D2
#define BRIGHTNESS  255


CRGB leds[NUM_LEDS];

// Replace with your network credentials
const char* ssid     = "Dollard_2.4GHz";
const char* password = "ik54j3s94rgq";

// Set web server port number to 80
WiFiServer server(60);

// Decode HTTP GET value
String reactString = "0";
String hueString = "0";
String saturationString = "0";
String valueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

int soundReactive = 0;
int hue = 0;
int saturation = 0;
int value = 0;

// Variable to store the HTTP req  uest
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  for (int i = 0; i < NUM_LEDS ; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();


}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
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
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
            client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>ESP Color Picker</h1></div>");
            client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"sound\" role=\"button\">Sound</a> ");
            client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");
            client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"hsv\"></div>");
            client.println("<script>document.getElementById(\"sound\").href=\"?r\" + 1 + \"h\" + 0 + \"s\" +  0 + \"v\" + 0 + \"&\";");
            client.println("function update(picker) {document.getElementById('hsv').innerHTML = Math.round(picker.hsv[0]) + ', ' +  Math.round(picker.hsv[1]) + ', ' + Math.round(picker.hsv[2]);");
            client.println("document.getElementById(\"change_color\").href=\"?r\" + 0 + \"h\" + Math.round(picker.hsv[0]) + \"s\" +  Math.round(picker.hsv[1]) + \"v\" + Math.round(picker.hsv[2]) + \"&\";}</script></body></html>");
            // The HTTP response ends with another blank line
            client.println();

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if (header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('h');
              pos3 = header.indexOf('s');
              pos4 = header.indexOf('v');
              pos5 = header.indexOf('&');
              reactString = header.substring(pos1 + 1, pos2);
              hueString = header.substring(pos2 + 1, pos3);
              saturationString = header.substring(pos3 + 1, pos4);
              valueString = header.substring(pos4 + 1, pos5);

              soundReactive = reactString.toInt();
              hue = map(hueString.toInt(), 0, 360, 0, 255);
              saturation = map(saturationString.toInt(), 0, 100, 0, 255);
              value = map(valueString.toInt(), 0, 100, 0, 255);
            }

            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, saturation, value);
  }
  FastLED.show();
}
