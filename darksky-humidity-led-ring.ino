#include <FastLED.h>
#include <WiFi.h>

// How many leds in your strip?
#define NUM_LEDS 24

// LED data line
#define LED_DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];

const char* ssid = "WIFI SSID";          // network SSID (name)
const char* pass = "WIFI PASS";          // network password
int status = WL_IDLE_STATUS;             // Wifi radio's status

void setup() {
  // Setup serial
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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

  // Setup LED ring
  Serial.print("Setup LED");
  FastLED.addLeds<WS2812B, LED_DATA_PIN, BRG>(leds, NUM_LEDS);
}

void loop() {
  Serial.print("Action");
  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Red;
  leds[2] = CRGB::Green;
  FastLED.show();
  delay(5000);
}
