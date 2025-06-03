#include <WiFi.h>
#include <PubSubClient.h>

// Configurações WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações MQTT
const char* mqttBroker = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopicAlerts = "emergency/device/alerts";
const char* mqttTopicStatus = "emergency/device/status";

// Pinos
#define BUTTON_PIN 5
#define LED_PIN 2

// Componentes
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Variáveis
String deviceId;
float latitude = -23.5505;   // Posição padrão (São Paulo)
float longitude = -46.6333;
bool buttonPressed = false;
int deviceCounter = 0; // Contador para gerar IDs e posições diferentes

void connectWiFi() {
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqttClient.connect(deviceId.c_str())) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Tentando novamente em 5s...");
      delay(5000);
    }
  }
}

void sendEmergencyAlert(String reason) {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  // Obter timestamp atual
  unsigned long now = millis();
  unsigned long seconds = now / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  String timestamp = String(hours % 24) + ":" + String(minutes % 60) + ":" + String(seconds % 60);

  String payload = "{";
  payload += "\"deviceId\":\"" + deviceId + "\",";
  payload += "\"reason\":\"" + reason + "\",";
  payload += "\"latitude\":" + String(latitude, 6) + ",";
  payload += "\"longitude\":" + String(longitude, 6) + ",";
  payload += "\"timestamp\":\"" + timestamp + "\"";
  payload += "}";

  mqttClient.publish(mqttTopicAlerts, payload.c_str());
  Serial.println("ALERTA ENVIADO: " + payload);

  mqttClient.publish(mqttTopicStatus, payload.c_str());
  Serial.println("STATUS ENVIADO: " + payload);

  // Feedback visual
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void generateNewDeviceData() {
  deviceCounter++;

  // Gerar um novo ID baseado no contador
  deviceId = "device-" + String(ESP.getEfuseMac(), HEX) + "-" + String(deviceCounter);

  // Coordenadas realistas dentro do Brasil:
  // Latitude: -33.70 (sul) até 5.30 (norte)
  // Longitude: -73.90 (oeste) até -34.80 (leste)
  latitude = random(-3370, 530) / 100.0;
  longitude = random(-7390, -3480) / 100.0;

  Serial.println("Novos dados do dispositivo gerados:");
  Serial.println("ID: " + deviceId);
  Serial.println("Latitude: " + String(latitude, 6));
  Serial.println("Longitude: " + String(longitude, 6));
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Inicializar gerador de números aleatórios

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Gerar dados iniciais do dispositivo
  generateNewDeviceData();

  connectWiFi();
  mqttClient.setServer(mqttBroker, mqttPort);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // Verificar botão de emergência
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    delay(50); // debounce

    // Gerar novos dados antes de enviar o alerta
    generateNewDeviceData();
    sendEmergencyAlert("Botão de emergência pressionado");
  }

  if (digitalRead(BUTTON_PIN) == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}
