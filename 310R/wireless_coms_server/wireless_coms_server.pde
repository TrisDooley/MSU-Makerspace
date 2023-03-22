//https://www.uni-weimar.de/projekte/costumes-and-environment/2017/05/19/tutorial-connecting-adafruit-feather-huzzah-esp8266-wireless-in-real-time-to-processing-3-3computer/

import java.util.*;
//#####################Server_stuff:####################################
import processing.net.*;
Server s; 
Client c;
String input;
//######################################################################

//Bilder:
ArrayList<PImage> bilder_a;
ArrayList<PImage> bilder_b;

int phase; //Aktuelle Bild-Nr
int direc; //An oder Aus?
int mode; //Focus
int pic_anzahl; //Anzahl an Bildern

void setup() {
 //Video:
 //fullScreen(); 
 size(600, 300);
 frameRate(25); 
 orientation(LANDSCAPE); //Bug?!

 //Setting_UP:
 phase=0; //Wasser Bild 0
 mode=0; //Focus auf Hebel
 direc=-1; //Wasser auschalten

 //#####################Server_stuff:####################################
 s = new Server(this, 12345); // Start a simple server on a port
 //######################################################################
}

void draw() {
  background(192);
  line(width/2, 0, width/2, height);
  byte xPos = byte(map(mouseX*(255.0/width),0,255,155,100));
  byte yPos = byte(mouseY*(255.0/height));
  byte[] data = {xPos, yPos}; 
  println(mouseY*(255.0/height));
  s.write(data);
  line(mouseX, mouseY, mouseX, 80);
}

void mousePressed(){
  if (mouseX>width/2) {
    if (phase==0) {
       phase=1;
       //#####################Server_stuff:####################################
       byte land = 118;
       byte sea = 42;
       byte[] coast = {land, sea};
       
       s.write(coast);
     } else {
       phase = 0;
       byte land = 98;
       byte sea = 56;
       byte[] coast = {land, sea};
       
       s.write(coast);
     }
  }
}
