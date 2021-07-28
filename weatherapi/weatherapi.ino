#include <ESP8266WiFi.h> 

const char* ssid = "Zhone_F121"; // Rellena con el nombre de tu red WiFi
const char* password = "nala2021"; // Rellena con la contraseña de tu red WiFi

const char* host = "api.wunderground.com";
const char* apiKey = "4b0406d4febb40a28406d4febb70a209"; // Puedes obtenerla en
                                                         // https://www.wunderground.com/weather/api/

const char * fingerprint = "67 8c 77 b9 8a 0a 41 1d df 3b 90 e6 46 d1 92 8a 02 eb 07 32";                                                         

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conectamos a la red WiFi

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  /* Configuramos el ESP8266 como cliente WiFi. Si no lo hacemos 
     se configurará como cliente y punto de acceso al mismo tiempo */
  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  WiFi.begin(ssid, password);

  // Esperamos a que estemos conectados a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Mostramos la IP
}

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);
 
  // Creamos el cliente
  WiFiClientSecure client;
  const int httpPort = 80; // Puerto HTTP
  if (!client.connect(host, httpPort)) {
    // ¿hay algún error al conectar?
    Serial.println("Ha fallado la conexión");
    return;
  }
 
  // Creamos la URL para la petición
  String url = "/api/";
  url += apiKey;
  url += "/conditions/lang:SP/q/autoip.json";
 
  Serial.print("URL de la petición: http://");
  Serial.print(host);
  Serial.print(":");
  Serial.print(httpPort);
  Serial.println(url);
 
  // Enviamos la petición
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" + 
         "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Superado el tiempo de espera !");
      client.stop();
      return;
    }
  }

  // Consutar la memoria libre
  // Quedan un poco más de 40 kB
  Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());
 
  // Leemos la respuesta y la enviamos al monitor serie
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("Cerrando la conexión");

  while(1){
    delay(0); // Siempre que hay un bucle que pueda durar mucho tiempo
              // hay que llamar a la función delay() para atender a los 
              // procesos de la conexión WiFi. Si no se hace el ESP8266
              // generará un error y se reiniciará a los pocos segundos
  }
}
