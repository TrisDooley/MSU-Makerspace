//programm_specific
#include <string> //to use string
int pin_out_LED = 14; //LED

const int dataLen = 4;
byte line[dataLen]; 

//Servo stuff
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//Configure WIFI:
#include <ESP8266WiFi.h> //Wifi-li brary


//WLAN-Config
const char* ssid = "FIXME"; //Your WIFI Name?
const char* password = "FIXME"; //Your WIFI Password?

//Host&Client
WiFiClient client; //Feather as a client
const char* host = "192.168.137.1"; //My Server(processing) IP Address(Terminal:"ifconfig -a")
const int httpPort = 12345; //Servers port


void setup() {
  //Serial setup
  Serial.begin(115200); //baud rate

  //Pin setup
  pinMode(pin_out_LED, OUTPUT);

  //Servo setup
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //Connect to WIFI
  digitalWrite(pin_out_LED, HIGH);
  while (WiFi.status() != WL_CONNECTED) {

  delay(500);
  Serial.print(".");
  }
  digitalWrite(pin_out_LED, LOW); //We are connected to SSID
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (!client.connected()) {
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(500);
    }
  } else {
    if (client.available()) {
      //read next byte available
      for(int i = 0; i < dataLen; i++) {
        line[i] = client.read(); //READ from Server
      }
    }

    Serial.println(line[0]);
    if(line[1] > 255/2) {
      digitalWrite(pin_out_LED, HIGH);
    } else {
      digitalWrite(pin_out_LED, LOW);
    }
    int servoPos = map(line[0], 0, 255, SERVOMIN, SERVOMAX);
    pwm.setPWM(0, 0, servoPos);

  }
}