#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const uint8_t ledPin = D1;
const char* ssid = "rPi3AP";
const char* wifi_password = "password";
const char* mqtt_server = "10.0.0.1";
const char* mqtt_topic = "Test";
const char* clientID = "ESP8266";
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if((char)payload[0] == 'y'){
    digitalWrite(ledPin, HIGH);
  }
  if((char)payload[0] == 'n'){
    digitalWrite(ledPin, LOW);
  }
  Serial.println();
  Serial.println("----------------------------");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi");
    //while(true){
    //  delay(500);
    //  Serial.print(".");
    //}
  }
  Serial.println("Connected to the WiFi network");
  //client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(clientID)) {
      Serial.println("MQTT server connected");  
    } else {
      Serial.print("failed with state: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(mqtt_topic);
}

void loop() {
  // put your main code here, to run repeatedly:
client.loop();
}
