#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "programmer5"
#define AIO_KEY         "aio_MtnZ77wM6TssNxDH34PJ3uT0hayn"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe pushButton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/buttonState");
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);  
  delay(10);
    WiFiManager wifiManager;
    wifiManager.autoConnect("mcu");
    Serial.println("");  
    Serial.print("IP address:");  
    Serial.println(WiFi.localIP());
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&pushButton);
}

uint32_t x=0;

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &pushButton) {
     if(strcmp((char*)pushButton.lastread,"1")==0){
      digitalWrite(D1,!digitalRead(D1));
     }
    }
  }
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
