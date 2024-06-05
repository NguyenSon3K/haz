#define BLYNK_TEMPLATE_ID "TMPL6D5UV-4Zw"
#define BLYNK_TEMPLATE_NAME "GIAM SAT NHIET DO DO AM"
#define BLYNK_AUTH_TOKEN "YfyvAuNdf1Q07XXthTBgv1olLbJSVP3r"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DTM E-SMART";
char pass[] = "0919890938";

#include "DHTesp.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dht;
unsigned long timeShowOled=millis();
float temperature, humidity;
WidgetLED led1(V0);

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  dht.setup(16, DHTesp::DHT11);
}

void loop() {
  Blynk.run();

  if(millis()-timeShowOled>1000){
    float t = dht.getTemperature();
    float h = dht.getHumidity();
    if(dht.getStatusString()=="OK"){
      temperature=t;
      humidity=h;
      showOled(temperature, humidity);

      Blynk.virtualWrite(V1, t);
      Blynk.virtualWrite(V2, h);
      if (led1.getValue()) {
        led1.off();
        Serial.println("LED on V0: off");
      } else {
        led1.on();
        Serial.println("LED on V0: on");
      }
    }
    timeShowOled=millis();
  }

}
void showOled(float t, float h){
  display.clearDisplay();
  display.setTextSize(2);                     // Normal 1:2 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);                     // Start at top-left corner
  display.print("T: ");
  display.print(t,1);
  display.print((char)247);
  display.println("C");
  display.print("H: ");
  display.print(h,0);
  display.println("%");
  display.display();
}
