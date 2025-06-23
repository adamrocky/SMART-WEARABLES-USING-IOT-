#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include "WiFi.h"
#include <PubSubClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


#define DHT_PIN 12
#define LDR_PIN 34


LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
LiquidCrystal_I2C LCD2 = LiquidCrystal_I2C(0x28, 16, 2);

DHTesp dht;

WiFiClient espClient;

PubSubClient client(espClient);

Adafruit_MPU6050 mpu;

int numberOfWifi = 0;
const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqttServer = "broker.emqx.io";
int lastMsg = 0;
float lastRotation = 0;
float steps = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  
  client.setServer(mqttServer, 1883);
  
  dht.setup(DHT_PIN, DHTesp::DHT22);

  LCD2.init();
  LCD2.backlight();
  LCD2.setCursor(2, 0);
  LCD2.print("Initializing");
  LCD2.setCursor(4, 1);
  LCD2.print("WiFi....");
  delay(1000);
  WiFi.mode(WIFI_STA);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(2, 0);
  LCD.print("Temprature");
  LCD.setCursor(4, 1);
  LCD.print("Monitor");
  delay(2000);

  LCD.clear();
  LCD.setCursor(2, 0);
  LCD.print("Setup Done!");
  delay(1000);

  LCD2.clear();
  LCD2.setCursor(2, 0);
  LCD2.print("Scanning");
  LCD2.setCursor(4, 1);
  LCD2.print("WiFi....");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  while(!mpu.begin()){
      delay(20);
      Serial.print("MPU Not connected");
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  TempAndHumidity data = dht.getTempAndHumidity();
  float temperature  = data.temperature;
  float humidity  = data.humidity;

  sensors_event_t acc, gcc, temp;
  mpu.getEvent(&acc, &gcc, &temp);

  float rotationX = gcc.gyro.x * 250/4.36;
  
  if(fabs(rotationX-lastRotation) > 90)
  {
    steps = steps + 0.5;
  }  

  Serial.print("Steps ");  
  Serial.print(steps);
  Serial.println("");
  lastRotation = rotationX;
  
  LCD.setCursor(0, 0);
  LCD.print("Temperature:");
  LCD.print(temperature);
  LCD.setCursor(0, 1);
  LCD.print("Humidity:");
  LCD.print(humidity);

  unsigned long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;
    String tempStr = String(temperature, 2);
    client.publish("/Temperature", tempStr.c_str());
    String humStr = String(humidity, 1);
    client.publish("/Humidity", humStr.c_str());
    String stepsStr = String(steps, 1);
    client.publish("/Steps", stepsStr.c_str());
  }

  delay(100);
  
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "mqttx_3c24a789";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Mqtt Connected");
    }
    else {
      delay(50);
    }
  }
}
