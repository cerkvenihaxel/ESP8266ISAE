
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *NOMBRE_RED = "ISAE2021",
           *CLAVE_RED = "00000000"; 

const int PUERTO = 80,
          LED_D1 = 2;

// Tener un estado del LED
bool estadoDelLED = false;

IPAddress ip(192, 168, 4, 22);
IPAddress puertaDeEnlace(192, 168, 4, 9);
IPAddress mascaraDeRed(255, 255, 255, 0);

ESP8266WebServer servidor(PUERTO);

String obtenerInterfaz()
{
  String HtmlRespuesta = "<!DOCTYPE html>"
                         "<html lang='es'>"
                         "<head>"
                         "<meta charset='UTF-8'>"
                         "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                         "<title>LEDS ESP8266</title>"
                         "</head>"
                         "<body>";
  HtmlRespuesta += "<h1>Encender y apagar LED</h1>";
  HtmlRespuesta += "<p>El LED est&aacute; actualmente ";
  if (estadoDelLED)
  {
    HtmlRespuesta += "encendido</p>";
    HtmlRespuesta += "<a href='/apagar'>Apagar</a>";
  }
  else
  {
    HtmlRespuesta += "apagado</p>";
    HtmlRespuesta += "<a href='/encender'>Encender</a>";
  }
  HtmlRespuesta += "<br><p>ISAE 2021</p>";
  HtmlRespuesta += "</body>"
                   "</html>";
  return HtmlRespuesta;
}

void mostrarInterfazHTML()
{
  servidor.send(200, "text/html", obtenerInterfaz());
}

void rutaEncenderLed()
{
  estadoDelLED = true;
  digitalWrite(LED_D1, HIGH);
  mostrarInterfazHTML();
}

void rutaApagarLed()
{
  estadoDelLED = false;
  digitalWrite(LED_D1, LOW);
  mostrarInterfazHTML();
}

void rutaRaiz()
{
  mostrarInterfazHTML();
}

void rutaNoEncontrada()
{
  servidor.send(404, "text/plain", "404");
}

void setup()
{
  // Configuración del LED
  pinMode(LED_D1, OUTPUT);

  // Configurar como un access point
  WiFi.softAP(NOMBRE_RED, CLAVE_RED);
  WiFi.softAPConfig(ip, puertaDeEnlace, mascaraDeRed);
  delay(100);

  // Configuramos la ruta y la función que responderá a la solicitud de dicha ruta
  servidor.on("/", rutaRaiz);
  servidor.on("/encender", rutaEncenderLed);
  servidor.on("/apagar", rutaApagarLed);
  servidor.onNotFound(rutaNoEncontrada);
  // Empezar a escuchar
  servidor.begin();
}

// En el loop manejamos al cliente conectado y eso es todo ;)
void loop()
{
  servidor.handleClient();
}
