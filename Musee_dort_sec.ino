#include <WiFi.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define TIME_TO_SLEEP   6000
#define uS_TO_mS_FACTOR 10000ULL
#define WLAN_SSID       "HUAWEI P20 lite"
#define WLAN_PASS       "Pistache1"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "PelouseCitron"
#define AIO_KEY         "aio_uCKo467FiPSBhduscau2b2idznAd"

#define BME_SCK 14  // Connect "SCL" pin on the BME680 with GPIO 14 on the ESP32
#define BME_MISO 12 // Connect "SDO" pin on the BME680 with GPIO 12 on the ESP32
#define BME_MOSI 13 // Connect "SDA" pin on the BME680 with GPIO 13 on the ESP32
#define BME_CS 15   // Connect "CS"  pin on the BME680 with GPIO 15 on the ESP32

Adafruit_BME680 bme;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'Humidity' for publishing.
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
// Setup a feed called 'Temperature' for publishing.
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
// Setup a feed called 'Luminosity' for publishing.
Adafruit_MQTT_Publish Luminosity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Luminosity");
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");
void MQTT_connect();


int temp, lum, hum = 0;

void setup() {
  Serial.begin(115200);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_mS_FACTOR);
  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);

  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  // Setup MQTT subscription for onoff feed.
  
  mqtt.subscribe(&onoffbutton);
  
  
}

void loop() {

  /*temp = bme.temperature;
  hum = bme.humidity;
  lum = analogRead(A0)/32;
*/
  
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }
  // Now we can publish stuff!
  Serial.print(F("\nSending Humidity val "));
  Serial.print(bme.humidity);
  Serial.print("...");
  if (! Humidity.publish(bme.humidity)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending Temperature val "));
  Serial.printf("temp: %f", bme.temperature);
  Serial.print("...");
  if (! Temperature.publish(bme.temperature)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("\nSending Luminosity val "));
  Serial.printf("Luminosity: %d",analogRead(A0)/32);
  Serial.print("...");
  if (! Luminosity.publish(analogRead(A0)/32)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  delay(5000);
  esp_deep_sleep_start();

  
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
  uint8_t retries = 5;
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
