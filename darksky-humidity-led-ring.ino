#include <FastLED.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// How many leds in your strip?
#define NUM_LEDS 24

// LED data line
#define LED_DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];

const char* ssid = "WIFI SSID";          // network SSID (name)
const char* pass = "WIFI PASS";          // network password
int status = WL_IDLE_STATUS;             // Wifi radio's status

const char* apiHost = "api.darksky.net";
const char* apiKey = "DARK SKY API KEY";
const char* location = "LAT,LNG";
const String apiPath = String("/forecast/") + apiKey + "/" + location + "?exclude=minutely,hourly,daily,alerts,flags" + "&units=si";

// The root CA of the host (Amazon Root CA 1)
const char* apiRootCA = \
                       "-----BEGIN CERTIFICATE-----\n" \
                       "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
                       "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
                       "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
                       "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
                       "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
                       "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
                       "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
                       "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
                       "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
                       "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
                       "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
                       "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
                       "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
                       "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
                       "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
                       "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
                       "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
                       "rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
                       "-----END CERTIFICATE-----\n";

WiFiClientSecure client;

void setup() {
  // Setup serial
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Setup LED ring
  Serial.print("Setup LED");
  FastLED.addLeds<WS2812B, LED_DATA_PIN, BRG>(leds, NUM_LEDS);

  // Setup WiFi connection
  Serial.print("WiFi connecting to SSID: ");
  Serial.println(ssid);
  while (status != WL_CONNECTED) {
    Serial.print(".");
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("");
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  // Setup WiFi client
  client.setCACert(apiRootCA);
}

void loop() {
  Serial.print("Action");
  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Red;
  leds[2] = CRGB::Green;
  FastLED.show();

  Serial.print("Requesting URL: ");
  Serial.println(apiHost + apiPath);

  Serial.println("Starting connection to server...");
  if (!client.connect(apiHost, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");

    client.println(String("GET https://") + apiHost + apiPath + " HTTP/1.0");
    client.println(String("Host: ") + apiHost);
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }

  delay(200000);
}