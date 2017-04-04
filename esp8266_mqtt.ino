#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "wifi-name";
const char* password = "wifi-password";

const char* mqtt_server = "mqtt-host";
const char* mqtt_username = "mqtt-host";
const char* mqtt_password = "mqtt-host";
const char* mqtt_pub_topic = "mqtt-host";
int mqtt_pub_interval = 10000;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
 
int sensorValue = 0;  // value read from the pot
int outputValue = 0;

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str(),mqtt_username,mqtt_password))
    {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  Serial.print("Connected to mqtt");
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 512, 0, 100);    

  sprintf (msg, "%i", outputValue);  
  client.publish(mqtt_pub_topic, msg);
  delay(mqtt_pub_interval);
  }
