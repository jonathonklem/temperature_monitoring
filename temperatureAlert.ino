/*
  Temperature Reporter
 
 This was a combination and alteration of a couple different example sketches.  Namely the web client sketch 
 and the USK circuit #7
 
 */

#include <SPI.h>
#include <Ethernet.h>

// edit this to the analog pin that your temperature transistor is connected to
const int temperaturePin = 3;

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your website's domain
char server[] = "www.jonathonklem.com";    

// Set the static IP address to use if the DHCP fails to assign
// be sure to set this to a valid ip in your subnet
IPAddress ip(192,168,0,177);

// This will be used to interact with ethernet shield
EthernetClient client;

// This keeps track of whether or not we have sent out a GET request or are waiting for a response
boolean sentRequest = false;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

void loop()
{
  if (!sentRequest) { 
    httpRequest(getTemperature(temperaturePin));
    sentRequest = true;
  } else {
    // if there are incoming bytes available 
    // from the server, read them and print them:
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      
      delay(60000); // repeat once every minute
      sentRequest = false;
    }
  }
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
    client.println("Host: www.jonathonklem.com");
    client.println("Connection: close");
    client.println();
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

float getTemperature(int pin) 
{
  float voltage, degreesC, degreesF;

  voltage = getVoltage(pin); 
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  return degreesF;
}
