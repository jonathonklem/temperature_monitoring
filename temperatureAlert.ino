#include <SPI.h>
#include <Ethernet.h>
#include <stdlib.h>

// edit this to the analog pin that your temperature transistor is connected to
const int temperaturePin = 3;

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// fill in an available IP address on your network here,
// for manual configuration, this must be on your subnet:
IPAddress ip(192,168,0,177);

// use the google's dns
IPAddress myDns(8,8,8,8);

// initialize the library instance:
EthernetClient client;

char server[] = "www.yourwebsite.com";

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 60*1000;  // delay between updates, in milliseconds

void setup()
{
  Serial.begin(9600);

  // give the ethernet module time to boot up:
  delay(1000);

  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);

  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}


void loop()
{
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin); 
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  
  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest(degreesF);
  }
  
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
  
  delay(500); // repeat once per second (change as you wish!)
}

// this method makes a HTTP connection to the server:
void httpRequest(float temperature) {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /settemp.php?temp=");
    client.print(temperature);
    client.println(" HTTP/1.1");
    client.println("Host: www.yourwebsite.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

float getVoltage(int pin)
{
  // this function and that constant were given with the Uno's documentation
  return (analogRead(pin) * 0.004882814);
}

