//programm_specific
#include <string> //to use string

//For motor and servo featherwing
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//defining the number of bytes recieving of the server
//This MUST be greater  than the number of bytes on the server
const int dataLen = 4;
byte line[dataLen]; 

//Servo stuff
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//true min is 100, true max is 500
#define SERVOMIN  120 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  480 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//Configure WIFI:
#include <ESP8266WiFi.h> //Wifi-li brary

//For motor featherwing
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

//Define a motor on the motor shield port 1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


//WLAN-Config
const char* ssid = "FIXME"; //Your WIFI Name?
const char* password = "FIXME"; //Your WIFI Password?

//Host&Client
WiFiClient client; //setting up the feather to act as a wifi client
const char* host = "192.168.137.1"; //My Server(processing) IP Address(Terminal:"ifconfig -a")
const int httpPort = 12345; //Servers port that was set in processing


void setup() {
  //Serial setup
  Serial.begin(115200); //baud rate

  //Start(begin) the servo control shield
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  Serial.println("Servo Shield Found");

  //Start(begin) the motor shield with the default frequency of 1.6KHz
  if (!AFMS.begin()) {         
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found");

  //Connecting to WIFI
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {
  //Connecting to the server
  if (!client.connected()) {
    
    //If the server disconnects stop the motor
    myMotor->setSpeed(0);

    //Try to reconnect to the server
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(500);
    }

  } else {
    //If there is data available to be read on the server
    if (client.available()) {

      //Read the number of bytes defined in dataLen
      for(int i = 0; i < dataLen; i++) {
        line[i] = client.read(); //READ from Server
      }
    }

    /* MOSFET code
    if(line[1] > 255/2) {
      digitalWrite(pin_out_LED, HIGH);
    } else {
      digitalWrite(pin_out_LED, LOW);
    }
    */

    //Motor featherwing speed code
    if(line[1] > 128) {
      myMotor->setSpeed((line[1]-128)*2); 
      myMotor->run(BACKWARD);
    } else if(line[1] < 128) {
      myMotor->setSpeed(abs(line[1]-128)*2);
      myMotor->run(FORWARD);
    }

    //Servo steering code
    int servoPos = map(line[0], 0, 255, SERVOMIN, SERVOMAX);
    pwm.setPWM(0, 0, servoPos);

  }
}