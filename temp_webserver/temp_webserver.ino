#include <dht11.h>
#include <SPI.h>
#include <Ethernet.h>

// Temperature and Humidity Module
dht11 DHT11;


// Set MAC and IP Address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,5,177);

//server on port 80
EthernetServer server(80);

void setup(){

  Serial.begin(9600);

   // Init ethernet conection
  Ethernet.begin(mac, ip);

  //Start server
  server.begin();

  //Log serial
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


}

void loop(){


  // listen
  EthernetClient client = server.available();


  if (client) {
    //Log serial
    Serial.println("new conection");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);


        if (c == '\n' && currentLineIsBlank) {
          //reply for client
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connnection: close");
          client.println();

          //set pin sensor
          int pin = 2;
            // Sensor read
            int chk = DHT11.read(pin);
            switch (chk)
            {
               case 0:
                 client.print("{ \"humidity\": ");
                 client.print((float)DHT11.humidity, DEC);
                 client.print(", \"temperature\": ");
                 client.print((float)DHT11.temperature, DEC);
                 client.print("}");
                 break;
              case -1:
                 client.print("{error: Checksum error}");
                 break;
              case -2:
                 client.print("{error: Time out error}");
                 break;
              default:
                client.print("{error: Unknown error}");
            break;
          }

          client.println("");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");

  }

}
