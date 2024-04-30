#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET 4

Adafruit_SSD1306 Display1(OLED_RESET);
Adafruit_SSD1306 Display2(OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define numOfValsRec 4
#define digitsPerValRec 1

int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;
int counter = 0;
bool counterStart = false;
String receivedString;

#define greenLed 3
#define redLed 2
#define solenoid1Pin 7
#define solenoid2Pin 8

int trigPin=13; 
int echoPin=11;
int buzzPin=6;

float pingTime; 
float targetDistance;
float speedofSound=343; 

void setup()
{
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(solenoid1Pin,OUTPUT);
  pinMode(solenoid2Pin,OUTPUT);
  
  Serial.begin(9600);

  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display2.clearDisplay();
  Display2.display();
  
  if(!Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  if(!Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT); 
  pinMode(buzzPin,OUTPUT);
   
}
//8===========================D

void loop()
{ 
  receiveData();
  unlock_solenoid(); 
  oled(); 
}

//8===========================D

void receiveData() {
  
   while (Serial.available()) {
    
    char c = Serial.read();
    if (c == '$') {
      counterStart = true;
    }
    if (counterStart) {
      if (counter < stringLength) {
        receivedString = String(receivedString + c);
        counter++;
      }
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++)
        {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }
  }
}

//8===========================D

void unlock_solenoid() {
    digitalWrite(solenoid1Pin, valsRec[0]);
    digitalWrite(solenoid2Pin, valsRec[1]);
    digitalWrite(greenLed, valsRec[2]);
    digitalWrite(redLed, valsRec[3]);
  }

//8===========================D

void oled() {
    digitalWrite(trigPin,LOW); 
    delayMicroseconds(2000); 
    digitalWrite(trigPin,HIGH); 
    delayMicroseconds(15); 
    digitalWrite(trigPin,LOW); 
  
    pingTime = pulseIn(echoPin,HIGH); 
    pingTime = pingTime/1000000.;
  
    targetDistance = speedofSound * pingTime;
    targetDistance = targetDistance/2; 
    targetDistance = targetDistance*100;
  
    //Serial.print("The distance to the target is: ");
    //Serial.print(targetDistance);
    //Serial.println(" centimeters");
  
    if (targetDistance <= 50){
      Display1.clearDisplay();
  
      Display1.setTextSize(2);
      Display1.setTextColor(WHITE);
      Display1.setCursor(0, 0);
      Display1.println("   Please,      the");
      Display1.display();
      Display1.clearDisplay();
      
      Display2.setTextSize(2);
      Display2.setTextColor(WHITE);
      Display2.setCursor(0, 0);
      Display2.println("Look at   Camera");
      Display2.display();
      Display2.clearDisplay();
      
      if (valsRec[2]==1){
        
        digitalWrite(buzzPin,HIGH);
        delay(500);
        digitalWrite(buzzPin,LOW);
        Display1.clearDisplay();
    
        Display1.setTextSize(2);
        Display1.setTextColor(WHITE);
        Display1.setCursor(0, 0);
        Display1.println("     Face ");
        Display1.display();
        Display1.clearDisplay();
        
        Display2.setTextSize(2);
        Display2.setTextColor(WHITE);
        Display2.setCursor(0, 0);
        Display2.println("Recognized Welcome!");
        Display2.display();
        Display2.clearDisplay();
        delay(3000);
        Display1.clearDisplay();
        Display2.clearDisplay();
      }

      if (valsRec[3]==1){
        
        digitalWrite(buzzPin,HIGH);
        delay(100);
        digitalWrite(buzzPin,LOW);
        delay(100);
        digitalWrite(buzzPin,HIGH);
        delay(100);
        digitalWrite(buzzPin,LOW);
        Display1.clearDisplay();
    
        Display1.setTextSize(2);
        Display1.setTextColor(WHITE);
        Display1.setCursor(1, 1);
        Display1.println("   Access ");
        Display1.display();
        Display1.clearDisplay();
        
        Display2.setTextSize(2);
        Display2.setTextColor(WHITE);
        Display2.setCursor(0, 0);
        Display2.println("Denied!");
        Display2.display();
        Display2.clearDisplay();
        delay(3000);
        Display1.clearDisplay();
        Display2.clearDisplay();
      }
    }
    
    if (targetDistance >= 50){
        
      Display1.clearDisplay();
  
      Display1.setTextSize(2);
      Display1.setTextColor(WHITE);
      Display1.setCursor(0, 0);
      Display1.println("    Facial    System");
      Display1.display();
      Display1.clearDisplay();
      
      Display2.setTextSize(2);
      Display2.setTextColor(WHITE);
      Display2.setCursor(0, 0);
      Display2.println("Recogntion");
      Display2.display();
      Display2.clearDisplay();
      
    }

}
