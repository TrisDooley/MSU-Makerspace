//https://www.uni-weimar.de/projekte/costumes-and-environment/2017/05/19/tutorial-connecting-adafruit-feather-huzzah-esp8266-wireless-in-real-time-to-processing-3-3computer/

//import needed things
import java.util.*;
import processing.net.*;

//Define the server
Server s;

void setup() {
  //Set the size, framerate, and orientation of the control window
  size(600, 300);
  frameRate(60); 
  orientation(LANDSCAPE);
  
  //Start a server on port 12345
  s = new Server(this, 12345);
}
//initializing variables, and defining the deadzone for the drive
byte yPos;
byte xPos;
int dz = 10;

void draw() {
  //Set the background
  background(192);
  
  //Draw the center line
  line(width/2, 0, width/2, height);
  
  //Draw the horizontal deadzone lines
  line(0, height/2-dz, width, height/2-dz);
  line(0, height/2+dz, width, height/2+dz);
  
  //Getting  data
  xPos = byte(map(mouseX,0,600, 255, 0));
  //If above the deadzone
  if(mouseY > height/2+dz) {
    
    //map the y position from above the deadzone to the top of the window to the top half of the byte range
    yPos = byte(map(max(mouseY,height/2+dz), height/2+dz, height, 128, 255));
    
    //draw a line from the top of the deadzone to the mouse position
    line(mouseX, mouseY, mouseX, height/2+dz);
    
  } 
  //If below the deadzone
  else if(mouseY < height/2-dz) { 
    
    //map the y position from below the deadzone to the bottom of the window to the bottom half of the byte range
    yPos = byte(map(min(mouseY,height/2-dz), 0, height/2-dz, 0, 127));
    
    //draw a line from the bottom half of the deadzone to hte mouse position
    line(mouseX, mouseY, mouseX, height/2-dz);
  }
  
  //Format the data so it can be loaded to the server
  byte[] data = {xPos, yPos}; 
  
  //Write the data to the server
  s.write(data);
  

}
