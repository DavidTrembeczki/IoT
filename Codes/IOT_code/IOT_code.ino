#include <WiFi.h>
#include <DHT.h>

DHT dht(14, DHT11);

// WiFi halozat neve es jelszava
const char *ssid = "dave";
const char *password = "12345678";

NetworkServer server(80);

void setup() {
  Serial.begin(115200);

  delay(10);

  // WiFi visszajelzés Serial Monitorra

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  dht.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            //HTTP header: text/html tartalmat biztosit, UTF-8 karakter kodolassal
            client.println("Content-type:text/html; charset=utf-8");
            //HTTP header: 2mp-kent frissiti az oldalt
            client.println("Refresh: 2");
            client.println();

            // Homerseklet es paratartalom kiiratasa
            client.print("<h2>Hőmérséklet: </h2><h1>");
            client.print(temp);
            client.print(" °C</h1> <br>");
            client.print("<h2>Páratartalom: </h2><h1>");
            client.print(humidity);
            client.print(" % </h1><br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");

  

  }
}
