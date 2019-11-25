#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <math.h>


// MAC Address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Server IP
IPAddress mqtt_server(192, 168, 15, 2);

// Topic we're working with
const char* topicName = "temperature";

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;
EthernetClient ethClient;
PubSubClient client(ethClient);

void setup()
{
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  if (!client.connected()) {
    Serial.print("Connecting ...\n");
    client.connect("Arduino Client");
  }
  else {
    // Reading tmperature from pin
    int a = analogRead(pinTempSensor);
    float R = 1023.0/a-1.0;
    R = R0*R;

    float temp = 1.0/(log(R/R0)/B+1/298.15)-273.15;
    char buffer[10];
    dtostrf(temp,0, 0, buffer);
    client.publish(topicName, buffer);
  }
  // Time interval
  delay(5000);
}
