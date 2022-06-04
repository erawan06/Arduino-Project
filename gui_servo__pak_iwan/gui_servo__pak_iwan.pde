import processing.serial.*;
import controlP5.*;

Serial myPort; 
ControlP5 cp5;
ControlGroup messageBox;


void setup() {
  size(500,430);
  myPort = new Serial(this, "COM3", 115200); // Change this to your port
  myPort.bufferUntil('\n');
  cp5 = new ControlP5(this);
  createMessageBox();
}

void draw() {

}

void createMessageBox() {
  PFont pfont = createFont("Arial",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,14);
  
  // create a group to store the messageBox elements
  messageBox = cp5.addGroup("messageBox",50,50,400);
  messageBox.setBackgroundHeight(330);
  messageBox.setBackgroundColor(color(0,200));
  messageBox.hideBar();

  
  // add a TextLabel to the messageBox.
  Textlabel label = cp5.addTextlabel("label","Kendalikan servo anda",45,160);
  label.setFont(font);
  label.moveTo(messageBox);
  
  Slider slider = cp5.addSlider("slider");
  slider.setLabel("");
  slider.setSize(300,30);
  slider.setPosition(50,100);
  slider.setRange(0,7);
  slider.setColorForeground(color(0,113,220));
  slider.setColorBackground(color(255,255,255));
  slider.showTickMarks(true);
  slider.setNumberOfTickMarks(5);
  slider.snapToTickMarks(false);
  slider.setValue(90);
  slider.moveTo(messageBox);
  
}

void serialEvent(Serial myPort)
{
  
}

void slider(int slider)
{
  myPort.write(slider);
}
