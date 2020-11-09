/* Here ESP32 will keep 2 roles: 
1/ read data from DHT11/DHT22 sensor
2/ control led on-off
So it willpublish temperature topic and scribe topic bulb on/off
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

/* change it with your ssid-password */
const char* ssid = "embebidos";
const char* password = "11111111";
/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "192.168.0.100";

/* define DHT pins */
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temperature = 0;

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/*LED GPIO pin*/
const char led = 2;

/* topics */
#define TEMP_TOPIC    "casa/hab1"
#define LED_TOPIC     "casa/hab2" /* 1=on, 0=off */

long lastMsg = 0;
char msg[20];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == '1') {
    digitalWrite(led, HIGH); 
    Serial.println("Entro al uno--encendido");
    
  } else {
    /* we got '0' -> on */
    digitalWrite(led, LOW);
    Serial.println("Entro al cero--apagado");
  }

}



void setup() {
  Serial.begin(115200);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  /* set led as output to control led on-off */
  pinMode(led, OUTPUT);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);

  
}
void loop() {
  /* if client was disconnected then try to reconnect again */
  
  //client.loop();
 

 temperature = random(0,100);
 snprintf (msg, 20, "%lf", temperature);
  /* publish the message */
      
 client.publish(TEMP_TOPIC, msg);
 delay(5000);

}
