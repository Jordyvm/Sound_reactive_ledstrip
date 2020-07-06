#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <StreamString.h>

#define API_KEY "66a4ab11-8d37-42e4-a03c-bec4d843b0a7" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define SSID_NAME "Dollard_2.4GHz" // TODO: Change to your Wifi network SSID
#define WIFI_PASSWORD "ik54j3s94rgq" // TODO: Change to your Wifi network password
#define SERVER_URL "iot.sinric.com"
#define SERVER_PORT 80

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void setPowerStateOnServer(String deviceId, String value);
void setTargetTemperatureOnServer(String deviceId, String value, String scale);
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

void setup() {
  Serial.begin(115200);

  WiFiMulti.addAP(SSID_NAME, WIFI_PASSWORD);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(SSID_NAME);

  // Waiting for Wifi connect
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin(SERVER_URL, SERVER_PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", API_KEY);

  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {
  webSocket.loop();

  if (isConnected) {
    uint64_t now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
      heartbeatTimestamp = now;
      webSocket.sendTXT("H");
    }
  }
}

void turnOn(String deviceId) {
  if (deviceId == "5e3ab8e4168ca72bf761574e") // Device ID of first device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
  }
}

void turnOff(String deviceId) {
  if (deviceId == "5e3ab8e4168ca72bf761574e") // Device ID of first device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
  }
}

void setColor(String deviceId, int hue, int saturation, int value) {
  if (deviceId == "5e3ab8e4168ca72bf761574e") // Device ID of first device
  {
    Serial.print("Set color Device ID: ");
    Serial.println(deviceId);
  }
}

void setBright(String deviceId, int brightness) {
  if (deviceId == "5e3ab8e4168ca72bf761574e") // Device ID of first device
  {
    Serial.print("Set brightness Device ID: ");
    Serial.println(deviceId);
  }
}

void setTemp(String deviceId, int temp) {
  if (deviceId == "5e3ab8e4168ca72bf761574e") // Device ID of first device
  {
    Serial.print("Set temp Device ID: ");
    Serial.println(deviceId);
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      isConnected = false;
      Serial.printf("[webSocketEvent] Webservice disconnected from server!\n");
      break;
    case WStype_CONNECTED: {
        isConnected = true;
        Serial.printf("[webSocketEvent] Service connected to server at url: %s\n", payload);
        Serial.printf("[webSocketEvent] Waiting for commands from server ...\n");
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[webSocketEvent] get text: %s\n", payload);
#if ARDUINOJSON_VERSION_MAJOR == 5
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload);
#endif
#if ARDUINOJSON_VERSION_MAJOR == 6
        DynamicJsonDocument json(1024);
        deserializeJson(json, (char*) payload);
#endif
        String deviceId = json ["deviceId"];
        String action = json ["action"];

        if (action == "setPowerState") {
          // alexa, turn on tv ==> {"deviceId":"xx","action":"setPowerState","value":"ON"}
          String value = json ["value"];
          if (value == "ON") {
            turnOn(deviceId);
          } else {
            turnOff(deviceId);
          }
        }
        else if (action == "AdjustBrightness") {
          // alexa, dim lights  ==>{"deviceId":"xxx","action":"AdjustBrightness","value":-25}
        }
        else if (action == "AdjustBrightness") {
          // alexa, dim lights  ==>{"deviceId":"xx","action":"AdjustBrightness","value":-25}
        }
        else if (action == "SetBrightness") {
          //alexa, set the lights to 50% ==> {"deviceId":"xx","action":"SetBrightness","value":50}
          int brightness = json ["value"];
          setBright(deviceId, brightness);
        }
        else if (action == "SetColor") {
          //alexa, set the lights to red ==> {"deviceId":"xx","action":"SetColor","value":{"hue":0,"saturation":1,"brightness":1}}
          int hue = json ["hue"];
          int saturation = json ["saturation"];
          int value = json ["brightness"];
          setColor(deviceId, hue, saturation, value);
        }
        else if (action == "IncreaseColorTemperature") {
          //alexa, set the lights softer ==> {"deviceId":"xxx","action":"IncreaseColorTemperature"}
        }
        else if (action == "IncreaseColorTemperature") {
          //alexa, set the lights softer ==> {"deviceId":"xxx","action":"IncreaseColorTemperature"}
        }
        else if (action == "SetColorTemperature") {
          //alexa, set the lights softer ==> {"deviceId":"xxx","action":"SetColorTemperature","value":2200}
          int temp = json ["value"];
          setTemp(deviceId, temp);
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[webSocketEvent] get binary length: %u\n", length);
      break;
    default: break;
  }
}


// If you are going to use a push button to on/off the switch manually, use this function to update the status on the server
// so it will reflect on Alexa app.
// eg: setPowerStateOnServer("deviceid", "ON")

// Call ONLY If status changed. DO NOT CALL THIS IN loop() and overload the server.
void setPowerStateOnServer(String deviceId, String value) {
#if ARDUINOJSON_VERSION_MAJOR == 5
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
#endif
#if ARDUINOJSON_VERSION_MAJOR == 6
  DynamicJsonDocument root(1024);
#endif
  root["deviceId"] = deviceId;
  root["action"] = "setPowerState";
  root["value"] = value;
  StreamString databuf;
#if ARDUINOJSON_VERSION_MAJOR == 5
  root.printTo(databuf);
#endif
#if ARDUINOJSON_VERSION_MAJOR == 6
  serializeJson(root, databuf);
#endif

  webSocket.sendTXT(databuf);
}
