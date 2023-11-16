#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "Robotardo";
const char *password = "12345678";

const int pinLed = 2;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("Punto de acceso configurado correctamente");
  Serial.print("Dirección IP del punto de acceso: ");
  Serial.println(WiFi.softAPIP());

  pinMode(pinLed, OUTPUT);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Configura la ruta para cargar el archivo HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Abre el archivo HTML desde SPIFFS
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo HTML");
      return;
    }

    // Lee el contenido del archivo HTML y lo envía como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "text/html", content);
  });

  // Configura la ruta para cargar el archivo JavaScript
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    // Abre el archivo JavaScript desde SPIFFS
    File file = SPIFFS.open("/script.js", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo JavaScript");
      return;
    }

    // Lee el contenido del archivo JavaScript y lo envía como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "application/javascript", content);
  });

  // Configura la ruta para cargar el archivo CSS
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // Abre el archivo CSS desde SPIFFS
    File file = SPIFFS.open("/style.css", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo CSS");
      return;
    }

    // Lee el contenido del archivo CSS y lo envía como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "text/css", content);
  });

  // Configura la ruta para controlar el LED
  server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request){
    // Lee el contenido de la solicitud
    String body = request->arg("plain");

    // Enciende o apaga el LED según el contenido de la solicitud
    if (body == "encender") {
      digitalWrite(pinLed, HIGH);
      request->send(200, "text/plain", "LED encendido");
    } else if (body == "apagar") {
      digitalWrite(pinLed, LOW);
      request->send(200, "text/plain", "LED apagado");
    } else {
      request->send(400, "text/plain", "Comando no reconocido");
    }
  });

  // Inicia el servidor
  server.begin();
}

void loop() {

}
