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
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0x00, 0x22, 0x19, 0xD1, 0x6A, 0xF3 };
IPAddress ip(169,254, 1, 1);
// IPAddress ip(129,65,155,215);
IPAddress gateway(169,254,255, 255);
IPAddress subnet(255, 255, 0, 0);
struct Motors             // a variable that structures selected data. used for the array. 
{
  unsigned char motorA; // variables for 4 motors for up to 255 using the PWM digital pins
  unsigned char motorB;
  unsigned char motorC;
  unsigned char motorD;
  //unsigned char motorServo;
  //unsigned char motorClaw;
}  __attribute__((__packed__)); // says that the struct field/ variable has smallest alignment. One buyte for a variable and one bit for a field

  
EthernetServer server(23); // telnet defaults to port 23
boolean alreadyConnected = false; // whether or not the client was connected previously
//******* Standard communication protocols below for serial and ethernet interface ********
void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600); // tells coms to operate at 9600 baud
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  for(int i = 0; i < 13; i++)// initializes pin outputs for the analog outputs and digital
   {
     pinMode(i,OUTPUT);
   } 
   pinMode(A0, OUTPUT);
   pinMode(A1, OUTPUT);
   pinMode(A2, OUTPUT);
   pinMode(A3, OUTPUT);
   pinMode(A5, OUTPUT);
   
  

  Serial.print("Chat server address: "); // prints out IP and server address for connection 
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available(); // the object client gets the number of bytes from the serial port(23) and returns the number of bytes available to read
  unsigned char   buffer[6]; // creates an buffer array of six spaces, used for the motors
  // when the client sends the first byte, say hello:
  if (client) { // large if that says if client is connected, do the following
    if (!alreadyConnected)  // says if not false,..... alreadyConnected is a false boolean by default
    {
      // clead out the input buffer:
       // client.flush();   
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true; // switches boolean to true so it does not repeat same command
    }
    if (client.available() > 0) // if recieving bytes are greater than 0
    {
      // read the bytes incoming from the client:
       for(int i=0 ;i<6;i++ )  
        {
           //delay(40);     
          unsigned char thisChar = client.read();
          buffer[i] = thisChar; // reads the incoming data from the client and turns it into a number from 0 to 255
        }
        
        struct Motors * a  = (struct Motors *)buffer; // a is part of the motors // fills the motors with the buffered input
       digitalWrite(A5,1); // powers on the polulu
               
          if(a->motorA<128)
         {
           digitalWrite(A0, 1); // turns on M1INA (top)
           digitalWrite(2,0); // turns off the M1INB (top)
           analogWrite(3,2*(127-a->motorA)); // defines speed of the motor
           Serial.print("Motor A : R"); // says that motor should be running in reverse
           Serial.print( 2*(127-a->motorA),DEC);
           Serial.print("\n");
         }
         else if(a->motorA >= 128)
         {
           digitalWrite(A0, 0); // turns off M1INA (top)
           digitalWrite(2,1);  // turns on M1INB (top)
           analogWrite(3,2*(a->motorA-128));
           Serial.print("Motor A : F"); // says the motor should be running forward
           Serial.print( 2*(a->motorA-128),DEC);
           Serial.print("\n");
         }
        
                // md.setM1Speed(( a->motorB-127)*400/127); // PWM is the speed command and 01 is forward and 10 is reverse
          if(a->motorB<128)
         {
         digitalWrite(4, 1); // turns on the M2INA (top)
         digitalWrite(A1,0); // turns off the M2INB (top)
         analogWrite(6,2*(127-a->motorB)); // speed for motor B
         Serial.print("Motor B : R"); // should be running forward
         Serial.print( 2*(127-a->motorB),DEC);
         Serial.print("\n");
         }
         else if(a->motorB>=128)
         {
         digitalWrite(4, 0); // turns off the M2INA (top)
          digitalWrite(A1,1); // turns off the M2INB (top)
         analogWrite(6,2*(a->motorB-128)); // speed for motor B
         Serial.print("Motor B : F");
         Serial.print( 2*(a->motorB-128),DEC);
         Serial.print("\n");
         }

//       Serial.print("Motor C  :");
//       Serial.print( a->motorC,DEC);
//       Serial.print("\n");
       if(a->motorC<128)
       {
       digitalWrite(7,1); // turns on M1INA (bottom)
       digitalWrite(8,0); // turns on M1INB (bottom)
       analogWrite(9,2*(127-a->motorC)); // speed for motor c
       Serial.print("Motor C : R");
       Serial.print( 2*(127-a->motorC),DEC);
       Serial.print("\n");
       }
       else if(a->motorC>=128)
       {
         digitalWrite(7,0); //turns off M1INA (bottom)
         digitalWrite(8,1); // turns on M1INB (bottom)
         analogWrite(9,(a->motorC-128)*2); // speed for motor c
         Serial.print("Motor C : F");
         Serial.print( 2*(a->motorC-128),DEC);
         Serial.print("\n");
       
       }
       
//       Serial.print("Motor D : F");
//       Serial.print( 2*(a->motorD-127),DEC);
//       Serial.print("\n");
       if(a->motorD<128)
         {
           digitalWrite(A2, 1); //turns on M1INB (bottom)
           digitalWrite(A3,0); //turns off M1INB (bottom)
           analogWrite(5,2*(127-a->motorD)); //speed for motor d
           Serial.print("Motor D : R");
           Serial.print( 2*(127-a->motorD),DEC);
           Serial.print("\n");
         }
         else if(a->motorD>=128)
         {
         digitalWrite(A2, 0); //turns off M1INB (bottom)
          digitalWrite(A3,1); //turns on M1INB (bottom)
         analogWrite(5,2*(a->motorD-128)); // speed for motor d
         Serial.print("Motor D : F");
         Serial.print( 2*(a->motorD-128),DEC);
         Serial.print("\n");
         }
      
//       Serial.print("Servo  :");
//       Serial.print( a->motorServo,DEC);
//       Serial.print("\n");
//      
//       Serial.print("Claw  :");
//       Serial.print( a->motorClaw,DEC);
//       Serial.print("\n");
//      
//       Serial.print("\n");
//       Serial.print("\n");
//       Serial.print("\n");
//       Serial.print("\n");
//       Serial.print("\n");
//       Serial.print("\n");


     
     
      // echo the bytes back to the client:
      //server.write(thisChar);
      // echo the bytes to the server as well:
    }
  }
}

