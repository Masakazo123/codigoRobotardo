#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Definición de las credenciales del punto de acceso WiFi
const char *ssid = "Robotardo";
const char *password = "12345678";

// Definición del pin al que están conectados los motores
const int motores = 2;

// Creación de un objeto para el servidor web asíncrono
AsyncWebServer server(80);

void setup() {
  // Inicialización de la comunicación serial a 115200 bps
  Serial.begin(115200);

  // Configuración del punto de acceso WiFi
  WiFi.softAP(ssid, password);

  // Mensajes de información sobre la configuración del punto de acceso
  Serial.println("Punto de acceso configurado correctamente");
  Serial.print("Dirección IP del punto de acceso: ");
  Serial.println(WiFi.softAPIP());

  // Configuración del pin de los motores como salida
  pinMode(motores, OUTPUT);

  // Inicialización del sistema de archivos SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Configuración de la ruta para cargar el archivo HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Apertura del archivo HTML desde SPIFFS
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo HTML");
      return;
    }

    // Lectura del contenido del archivo HTML y envío como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "text/html", content);
  });

  // Configuración de la ruta para cargar el archivo JavaScript
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    // Apertura del archivo JavaScript desde SPIFFS
    File file = SPIFFS.open("/script.js", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo JavaScript");
      return;
    }

    // Lectura del contenido del archivo JavaScript y envío como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "application/javascript", content);
  });

  // Configuración de la ruta para cargar el archivo CSS
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // Apertura del archivo CSS desde SPIFFS
    File file = SPIFFS.open("/style.css", "r");
    if (!file) {
      request->send(404, "text/plain", "Error al abrir el archivo CSS");
      return;
    }

    // Lectura del contenido del archivo CSS y envío como respuesta al cliente
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }

    file.close();
    request->send(200, "text/css", content);
  });

  // Configuración de la ruta para controlar los motores mediante solicitudes POST
  server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request){
    // Lectura del contenido de la solicitud POST
    String body = request->arg("plain");

    // Encendido o apagado de los motores según el contenido de la solicitud
    if (body == "encender") {
      digitalWrite(motores, HIGH);
      request->send(200, "text/plain", "LED encendido");
    } else if (body == "apagar") {
      digitalWrite(motores, LOW);
      request->send(200, "text/plain", "LED apagado");
    } else {
      request->send(400, "text/plain", "Comando no reconocido");
    }
  });

  // Inicio del servidor web
  server.begin();
}

void loop() {
  // El bucle principal está vacío ya que la gestión del servidor web es asincrónica
}
