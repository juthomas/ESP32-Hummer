#include <WiFi.h>
#include <WiFiUdp.h>
#include "jsonParser.h"
#define MOTOR_1_A 2
#define MOTOR_1_B 15
#define MOTOR_2_A 13
#define MOTOR_2_B 12
#define FORWARD_CHANNEL 1
#define BACKWARD_CHANNEL 2
#define MOTOR_FREQUENCY 5000
#define MOTOR_RESOLUTION 8

#define FRONT_LIGHT_1 21
#define FRONT_LIGHT_2 22
#define FRONT_LIGHT_CHANNEL 3
const char *ssid = "ESP32WIFI";
const char *password = "44448888";
WiFiUDP udp;

uint8_t front_light_power = 0;


void setup()
{
  Serial.begin(115200);
  pinMode(MOTOR_1_A, OUTPUT);
  pinMode(MOTOR_1_B, OUTPUT);

  ledcSetup(FORWARD_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcSetup(BACKWARD_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcSetup(FRONT_LIGHT_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcAttachPin(MOTOR_2_B, FORWARD_CHANNEL);
  ledcAttachPin(MOTOR_2_A, BACKWARD_CHANNEL);
  ledcAttachPin(FRONT_LIGHT_1, FRONT_LIGHT_CHANNEL);
  ledcAttachPin(FRONT_LIGHT_2, FRONT_LIGHT_CHANNEL);
  // Configuration de l'ESP32 en tant que point d'accès
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Point d'accès créé, adresse IP: ");
  Serial.println(WiFi.softAPIP());

  // Démarrage du serveur UDP sur le port 1234
  udp.begin(12345);
  Serial.println("Serveur UDP démarré sur le port 12345");
}

DynamicJsonDocument doc(1024);

void loop()
{
  // Lire les données UDP disponibles
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    // Si des données sont disponibles, les lire et les afficher sur la sortie standard
    char buffer[packetSize];
    int len = udp.read(buffer, packetSize);
    if (len > 0)
    {
      buffer[len] = 0;
      Serial.print("Message UDP reçu: ");
      Serial.println(buffer);
      deserializeJson(doc, buffer);
      float j1x = doc["J1X"];
      float j1y = doc["J1Y"];
      int left = doc["Left"];
      int right = doc["Right"];
      int a = doc["A"];
      int b = doc["B"];
      float l2 = doc["L2"];
      float r2 = doc["R2"];


    ledcWrite(FORWARD_CHANNEL, r2);
    ledcWrite(BACKWARD_CHANNEL, l2);

    digitalWrite(MOTOR_1_A, left ? HIGH : LOW);
    digitalWrite(MOTOR_1_B, right ? HIGH : LOW);
    ledcWrite(FRONT_LIGHT_CHANNEL, front_light_power);

    if (a && front_light_power < 254)
    {
      front_light_power++;
    }
    if (b && front_light_power > 0)
    {
      front_light_power--;
    }



      Serial.printf("Left %f:%f:%f:%f\n", j1x, j1y, l2, r2);
    }
  }
  delay(10);
}