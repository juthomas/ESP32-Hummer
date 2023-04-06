#include <WiFi.h>
#include <WiFiUDP.h>

const char* ssid = "ESP32WIFI";
const char* password = "44448888";
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Configuration de l'ESP32 en tant que point d'accès
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32AP", "motdepasse");
  Serial.print("Point d'accès créé, adresse IP: ");
  Serial.println(WiFi.softAPIP());

  // Démarrage du serveur UDP sur le port 1234
  udp.begin(12345);
  Serial.println("Serveur UDP démarré sur le port 1234");
}

void loop() {
  // Lire les données UDP disponibles
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Si des données sont disponibles, les lire et les afficher sur la sortie standard
    char buffer[packetSize];
    int len = udp.read(buffer, packetSize);
    if (len > 0) {
      buffer[len] = 0;
      Serial.print("Message UDP reçu: ");
      Serial.println(buffer);
    }
  }
  delay(10);
}