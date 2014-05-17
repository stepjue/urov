/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h> // includes the internet shield library

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,101);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,0,0);
struct Motors   // motor structure
{

unsigned char motorA;
unsigned char motorB;
unsigned char motorC;
unsigned char motorD;
unsigned char motorServo;
unsigned char motorClaw;
}  __attribute__((__packed__));

// telnet defaults to port 23
EthernetServer server(23); //Create a server that listens for incoming connections on the specified port.
boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  for(int i = 0; i < 10; i++) // turns pins 1 through 10 on
   {
     pinMode(i,OUTPUT);
   } 
   
  

  Serial.print("Chat server address:");  // prints out the server address
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  digitalWrite(1, 1);
          digitalWrite(2,0);
          analogWrite(3, 255);
  EthernetClient client = server.available();
unsigned char   buffer[6];
  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
     // client.flush();   
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
   for(int i=0 ;i<6;i++ )  
      {
             unsigned char thisChar = client.read();

      buffer[i] = thisChar;
      }
    struct Motors * a  = (struct Motors *)buffer;
       Serial.print("Motor A  :");
       Serial.print( a->motorA,DEC);
       Serial.print("\n");
          digitalWrite(1, 0);
          digitalWrite(2,1);
          analogWrite(3, 127);
      Serial.print("Motor B  :");
       Serial.print( a->motorB,DEC);
       Serial.print("\n");
                // md.setM1Speed(( a->motorB-127)*400/127);
          digitalWrite(4, 0);
          digitalWrite(5,1);
          analogWrite(6,127);

       Serial.print("Motor C  :");
       Serial.print( a->motorC,DEC);
       Serial.print("\n");
      
       Serial.print("Motor D :");
       Serial.print( a->motorD,DEC);
       Serial.print("\n");
      
       Serial.print("Servo  :");
       Serial.print( a->motorServo,DEC);
       Serial.print("\n");
      
       Serial.print("Claw  :");
       Serial.print( a->motorClaw,DEC);
       Serial.print("\n");
      
       Serial.print("\n");
       Serial.print("\n");
       Serial.print("\n");
       Serial.print("\n");
       Serial.print("\n");
       Serial.print("\n");


     
     
      // echo the bytes back to the client:
      //server.write(thisChar);
      // echo the bytes to the server as well:
    }
  }
}


