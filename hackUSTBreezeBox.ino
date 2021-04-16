//credit to: https://icreateproject.info/2014/12/14/arduino-save-data-to-database/ 
#include <SPI.h>
#include <Ethernet.h>
//--------------------------------- ethernet shield ----------------------------------------------//
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
// Enter the IP address for Arduino, as mentioned we will use 192.168.0.16
// Be careful to use , insetead of . when you enter the address here
IPAddress ip(192,168,0,16);

//int dustPin = 13;  // Analog input pin on Arduino we connected the SIG pin from sensor
int dustReading;  // Here we will place our reading

char server[] = "192.168.0.11"; // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie. "www.yourwebpage.com")

// Initialize the Ethernet server library
EthernetClient client;
//---------------------------------------PM2.5 Sensor----------------------------------------------//
int dustPin=0;
float dustVal=0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;

//-----------------------------------------Motor A----------------------------------------------//
int pwmA = 9;
const int motorPin1  = 8;  // Pin 2 of L293a
const int motorPin2  = 7;  // Pin 7 of L293a
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
//motors are all off
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
//--------------------------------------------SET UP--------------------------------------------//    
void setup(){
Serial.begin(9600);
pinMode(ledPower,OUTPUT);
pinMode(dustPin, INPUT);
 pinMode(pwmA, OUTPUT);

 // Serial.begin starts the serial connection between computer and Arduino
  Serial.begin(9600);
 
  // start the Ethernet connection
  Ethernet.begin(mac, ip);
}
void loop(){
digitalWrite(ledPower,LOW);
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin);
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH);
delayMicroseconds(offTime);
delay(1000);
if (dustVal>36.455){
  Serial.println((float(dustVal/1024)-0.0356)*120000*0.035);
}
while (dustVal > 50){//if the level has gone a certain level
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(10000); //10 seconds
  dustVal=analogRead(dustPin);
}
//turn the fans off
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);


  dustReading = analogRead(dustPin); // Fill the sensorReading with the information from sensor
 
  // Connect to the server (your computer or web page)  
  if (client.connect(server, 80)) {
    client.print("GET /write_data.php?"); // This
    client.print("value="); // This
    client.print(dustReading); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.0.11"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server

  }

  else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }
 
  // Give the server some time to recieve the data and store it. I used 10 seconds here. Be advised when delaying. If u use a short delay, the server might not capture data because of Arduino transmitting new data too soon.
  delay(10000);

}
