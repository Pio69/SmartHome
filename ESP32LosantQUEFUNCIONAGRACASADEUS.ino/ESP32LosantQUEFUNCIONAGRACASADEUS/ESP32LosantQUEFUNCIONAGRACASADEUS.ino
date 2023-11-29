/**
 * Example that connects an ESP32 based board to the Losant
 * IoT platform. This example reports state to Losant whenever a button is
 * pressed. It also listens for the "toggle" command to turn the LED on and off.
 *
 * This example assumes the following connections:
 * Button connected to pin 14.
 * LED connected to pin 12.
 *
 * Copyright (c) 2023 Losant. All rights reserved.
 * http://losant.com
 */

#include <WiFiClientSecure.h>
#include <Losant.h>

// WiFi credentials.
const char* WIFI_SSID = "GalaxyM31";
const char* WIFI_PASS = "12345678";

// Losant credentials.
const char* LOSANT_DEVICE_ID = "6563ef106c47200ba20f8289";
const char* LOSANT_ACCESS_KEY = "20902b87-88d4-4eba-8cea-ee30f0712cda";
const char* LOSANT_ACCESS_SECRET = "762e64382a846fe8f584425830ff9a311f5c276ff45f23366d81b9ce832de7f3";

const int BUTTON_PIN = 14;
const int LED_PIN = 12;

unsigned long previousMillis = 0; // Variável para armazenar o tempo da última mudança
const long interval = 30000;

bool ledState = false;

const int potPin = 34;

float potValue = 0;

const char* rootCABuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
"-----END CERTIFICATE-----\n";

// initiate the the wifi client
WiFiClientSecure wifiClient;

LosantDevice device(LOSANT_DEVICE_ID);

void toggle() {
  Serial.println("Toggling LED.");
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

// Called whenever the device receives a command from the Losant platform.
void handleCommand(LosantCommand *command) {
  Serial.print("Command received: ");
  Serial.println(command->name);

  // Optional command payload. May not be present on all commands.
  // JsonObject payload = *command->payload;

  // Perform action specific to the command received.
  if(strcmp(command->name, "toggle") == 0) {
    toggle();
    /**
    * In Losant, including a payload along with your
    * command is optional. This is an example of how
    * to parse a JSON payload from Losant and print
    * the value of a key called "temperature".
    */
    // int temperature = payload["temperature"];
    // Serial.println(temperature);
  }
}

void connect() {

  // Connect to Wifi.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  wifiClient.setCACert(rootCABuff);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to Losant.
  Serial.println();
  Serial.print("Connecting to Losant...");

  device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  while(!device.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { }
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Register the command handler to be called when a command is received
  // from the Losant platform.
  device.onCommand(&handleCommand);

  connect();
}

void buttonPressed() {
  Serial.println("Publish Data!");

  // Losant uses a JSON protocol. Construct the simple state object.
  // { "button" : true }
  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();

  float current = map(random(300, 600), 0, 1023, 300, 600) / 1000.0; // Gera um número aleatório entre 0.3 e 0.6
  float voltage = random(215, 220); // Gera um número aleatório entre 215 e 220

  potValue = (analogRead(potPin)/1000);
  Serial.println(potValue);

  root["current"] = potValue;
  root["frequency"] = 60;
  root["powerFact"] = 1;
  root["voltage"] = voltage;

  float power = potValue * voltage; // Calcula o valor da potência
  root["power"] = power;

  // Send the state to Losant.
  device.sendState(root);
}

int buttonState = 0;

void loop() {

  bool toReconnect = false;

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    toReconnect = true;
  }

  if(!device.connected()) {
    Serial.println("Disconnected from Losant");
    toReconnect = true;
  }

  if(toReconnect) {
    connect();
  }

  device.loop();

   unsigned long currentMillis = millis(); // Obtém o tempo atual

  if (currentMillis - previousMillis >= interval) {
    // Se passaram 5 minutos
    previousMillis = currentMillis; // Salva o tempo atual como a última mudança
    ledState = !ledState; // Inverte o estado do LED
    buttonPressed(); // Define o estado do LED
  }



  delay(100);
}
