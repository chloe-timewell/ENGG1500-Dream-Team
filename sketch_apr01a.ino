#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// OLED display TWI address
#define OLED_ADDR 0x3C

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // create our screen object setting resolution to 128x64

//define varaibles used for state based BPM calculation.
#define UpperThreshold 450
#define LowerThreshold 370
 
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, INPUT);

  Wire.setClock(1000000);

  //initialize display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  //display a pixel in each corner of the screen
  display.drawPixel(0, 0, SSD1306_WHITE);
  display.drawPixel(127, 0, SSD1306_WHITE);
  display.drawPixel(0, 63, SSD1306_WHITE);
  display.drawPixel(127, 63, SSD1306_WHITE);

  //display the splash screen with text
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(10, 20);     // Start 27 pixels in and 10 pixels down
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("HeartWave");
  display.display();  // Update screen with each newly-drawn text
  delay(2000);
  display.clearDisplay();
}

//initialise global variables
int value=0;
int y=0;
int x=0;
int lastx=0;
int lasty=0;
int lastTimeBPM=0;
int thisTimeBPM;
bool BPMTiming=false;
bool beatComplete=false;
int BPM=0;
double hold1=0;
double hold2=0;
double hold3=0;
double average=0;
int lastTimeHeartWave=0;
int thisTimeHeartWave=0;
int heartWaveTiming=6;
int thisTimeDisplay=0;
int lastTimeDisplay=0;
int displayTiming=1000;

void loop() 
{
  //measure the current time
  thisTimeBPM=millis();
  thisTimeHeartWave=millis();
  thisTimeDisplay=millis();
  
  //measure the curent value of the AD8232 module for the oled display
  value=analogRead(A0);
  
  //transform the value to scale onto the OLED screen
  y=60-(value/14);
  
  // calcualate bpm if the current value is above UpperThreshold and the state is true because it has dropped below the LowerThreshold since it was last calculated
  if(value>UpperThreshold)
  {
    if(beatComplete)
    {
      BPM=thisTimeBPM-lastTimeBPM;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      beatComplete=false;
      tone(8,1000,250);
    }
    if(BPMTiming==false)
    {
      lastTimeBPM=millis();
      BPMTiming=true;
    }
  }
  if((value<LowerThreshold)&(BPMTiming))
    beatComplete=true;
    
  //update the display every 1 s with the updateDisplay method
  if((thisTimeDisplay-lastTimeDisplay)>=displayTiming)
  {
    updateDisplay();
  }

  //only measure the heart wave every 6 ms
  if((thisTimeHeartWave-lastTimeHeartWave)>=heartWaveTiming)
  {
    measureWave();
  }
}

void measureWave(){
  
    //set the last time the heart wave was measured equal to the current time
    lastTimeHeartWave=thisTimeHeartWave;
    
    //calculate the average by passing values to the hold variables then averaging them
    hold3=hold2;
    hold2=hold1;
    hold1=analogRead(A0);
    average=((hold1+hold2+hold3) / 3);
  
    //print the average to the serial monitor
    Serial.println(average);
}

void updateDisplay(){

  /*//hold the values of x and y in the lasty and lastx varialbles
  lasty=y;
  lastx=x;
  
  //when the PQRST wave reaches the end, clear the screen and start again from the left
  if(x>127)  
  {
    display.clearDisplay();
    x=0;
    lastx=x;
  }
  
  //draw the PQRST wave from left to right
  display.writeLine(lastx,lasty,x,y,WHITE);
  
  //increment the pixel count to the right for the PQRST wave
  x++;*/

  //set the last time the display was updated equal to the current time
  lastTimeDisplay=thisTimeDisplay;
  
  //draw the BPM number and text
  display.writeFillRect(0,50,128,16,BLACK);
  display.setCursor(0,50);
  display.print(BPM);
  display.print(" BPM"); 
  display.display();  
}
