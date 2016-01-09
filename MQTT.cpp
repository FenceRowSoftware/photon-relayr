// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_DHT/Adafruit_DHT.h"
#include "SparkJson/SparkJson.h"
#include "MQTT/MQTT.h"

//define the model of temp sensor used for the DHT library:
#define DHTTYPE DHT11

#define DEVICE_ID ""
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_SERVER "mqtt.relayr.io"
#define MQTT_CLIENTID ""
#define DHTPIN 1 //pin DHT sensor is connected to

const int led = D7;
int ledState = LOW;
char message_buff[100];
unsigned long lastPublishTime = 0;
unsigned long lastBlinkTime = 0;
int publishingPeriod = 10000; // Set here the time in milliseconds between publications
// server will not allow values under 200 ms

void callback(char* topic, byte* payload, unsigned int length);

DHT dht(DHTPIN, DHTTYPE);
MQTT client(MQTT_SERVER, 1883, callback);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  debug("topic: " + String(topic), 0);
  debug("payload: " + String(p), 0);
  handlePayload(p);
}

void handlePayload(char* payload) {
  StaticJsonBuffer<200> jsonBuffer;
  //convert payload to json
  JsonObject& json = jsonBuffer.parseObject(payload);
  if (!json.success()) {
    debug("json parsing failed", 0);
    return;
  }
}

void mqtt_connect() {
  debug("Connecting to mqtt server", 0);

  if (client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASSWORD)) {
    debug("Connection success, subscribing to topic", 0);
    client.subscribe("/v1/"DEVICE_ID"/cmd");
  }
  else {
    debug("Connection failed, check your credentials or wifi", 0);
  }
}

void setup() {
  RGB.control(true);
  Serial.begin(9600);
  //setup our LED pin and connect to mqtt broker
  pinMode(led, OUTPUT);
  //set 200ms as minimum publishing period
  publishingPeriod = publishingPeriod > 200 ? publishingPeriod : 200;
  dht.begin();
  mqtt_connect();
}

void loop() {
  if (client.isConnected()) {
    client.loop();
    if (millis() - lastPublishTime > publishingPeriod) {
      lastPublishTime = millis();
      publish();
    }
    blink(publishingPeriod / 2);
  }
  else {
    debug("retrying..", 0 //connection lost
    mqtt_connect();
  }
}

void publish() {
  publishHumidity();
  publishTemperature();
}

void publishHumidity() {
  int h = dht.getHumidity();

  StaticJsonBuffer<100> pubJsonBuffer;
  JsonObject& pubJson = pubJsonBuffer.createObject();
  pubJson["meaning"] = "moisture";
  pubJson["value"] = h;
  char message_buff[100];
  pubJson.printTo(message_buff, sizeof(message_buff));
  client.publish("/v1/"DEVICE_ID"/data", message_buff);
  debug("Publishing " + String(message_buff), 0);
}

void publishTemperature() {
  float f = dht.getTempCelcius();

  StaticJsonBuffer<100> pubJsonBuffer;
  JsonObject& pubJson = pubJsonBuffer.createObject();
  pubJson["meaning"] = "temperature";
  pubJson["value"] = (f*(9/5)+32);
  char message_buff[100];
  pubJson.printTo(message_buff, sizeof(message_buff));
  client.publish("/v1/"DEVICE_ID"/data", message_buff);
  debug("Publishing " + String(message_buff), 0);
}

void blink(int interval) {
  if (millis() - lastBlinkTime > interval) {
    lastBlinkTime = millis();
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(led, ledState);
  }
}

void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Spark.publish("DEBUG", msg);
}
