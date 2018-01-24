/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include "DHT.h";
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h> 

int servoPin = 5;
Servo servo;  
int angle = 0;   // servo position in degrees 
 
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Constants
#define DHTPIN 6     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)



//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
const int pResistor1 = 4; // Photoresistor at Arduino analog pin A0
const int pResistor2 = 5; // Photoresistor at Arduino analog pin A0
const int buttonPin = 2;  

DHT dht(DHTPIN, DHTTYPE);

int pResist1;
int pResist2;
 
int moistureSensor1 = 0;
int moistureSensor2 = 1;
int moistureSensor3 = 2;
int moistureSensor4 = 3;
int PumpREL = A15;

int moistVal1;
int moistVal2;
int moistVal3;
int moistVal4;
int ledPin = 3;

int dryVal;
int wetVal;
long wateringTime = 0;
long previousTimeWater = 0;
float waterPumped = 0;
const float flowRate = 0.0016666;
boolean beingWatered1 = false;
boolean beingWatered2 = false;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(PumpREL, OUTPUT);
//  pinMode(LightREL1, OUTPUT);
//  pinMode(LightREL2, OUTPUT);
  pinMode(pResistor1, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(pResistor2, INPUT);// Set pResistor - A0 pin as an input (optional)
  lcd.begin (16,2); // for 16 x 2 LCD module
  delay(8000);
//  digitalWrite(LightREL1, HIGH);
//  digitalWrite(LightREL2, HIGH);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), resetVals, FALLING); 
  servo.attach(servoPin); 
}

void loop()
{
  unsigned long currentMillis = millis();
  moistVal1 = analogRead(moistureSensor1); //Bed 1
  moistVal2 = analogRead(moistureSensor2); //Bed 1
  moistVal3 = analogRead(moistureSensor3); //Bed 2
  moistVal4 = analogRead(moistureSensor4); //Bed 2

  lcd.home (); // set cursor to 0,0
  lcd.print("Moisture 1: "+String(moistVal1)+"   "); 
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print("Moisture 2: "+String(moistVal2)+"   ");  
  delay(3000);

//  wetVal = (moistVal1 + moistVal3)/2; //Take average of the moisture sensors in the wet and dry areas
//  dryVal = (moistVal2 + moistVal4)/2;
//  Serial.println(dryVal);
//  Serial.println(wetVal);

  
  lcd.home (); // set cursor to 0,0
  lcd.print("Moisture 3: "+String(moistVal3)+"   "); 
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print("Moisture 4: "+String(moistVal4)+"   ");  
  delay(2000);

  if(waterPumped <2000){
    //If the dry is too dry and the wet is too dry, turn on relay   
    if(moistVal2 >= 1000 && moistVal1 >= 950 && !beingWatered2){ //if water that is pumped is less than 2 L   
      beingWatered1 = true;
      servo.write(45);
      wateringTime = currentMillis-previousTimeWater;
      Serial.println("H2O time: "+String(wateringTime));
      waterPumped = flowRate*wateringTime; //pump rate 100 ml/min / 60 sec / 1000 ms* water watering
      Serial.println("Water pumped: "+String(waterPumped));
      lcd.home (); // set cursor to 0,0
      lcd.print("Side 1 watering "); 
      lcd.setCursor (0,1);        // go to start of 2nd line
      lcd.print("Tank Level: "+String(waterPumped)); 
      digitalWrite(PumpREL, HIGH);
      Serial.println("Relay On");
      delay(5000);
    }
    else if(moistVal4 >= 1000 && moistVal3 >= 950 && !beingWatered1){ //if water that is pumped is less than 2 L   
      beingWatered2 = true;
      servo.write(135);
      wateringTime = currentMillis-previousTimeWater;
      Serial.println("H2O time: "+String(wateringTime));
      waterPumped = flowRate*wateringTime; //pump rate 100 ml/min / 60 sec / 1000 ms* water watering
      Serial.println("Water pumped: "+String(waterPumped));
      lcd.home (); // set cursor to 0,0
      lcd.print("Side 2 watering "); 
      lcd.setCursor (0,1);        // go to start of 2nd line
      lcd.print("Tank Level: "+String(waterPumped)); 
      digitalWrite(PumpREL, HIGH);
      Serial.println("Relay On");
      delay(5000);
    }
    else {
      beingWatered1 = false;
      beingWatered2 = false;
      previousTimeWater = currentMillis;
      digitalWrite(PumpREL, LOW);
      Serial.println("Relay Off");
      }
  }
    else{
      digitalWrite(PumpREL, LOW);
      digitalWrite(ledPin, HIGH);
      Serial.println("Tank empty");
      lcd.home (); // set cursor to 0,0
      lcd.print("Pump off no H20 "); 
      lcd.setCursor (0,1);        // go to start of 2nd line
      lcd.print("Please fill tank");
      resetVals();  
      delay(4000);
    }


  pResist1 = analogRead(pResistor1);
  pResist2 = analogRead(pResistor2);

  Serial.println(pResist1);
  Serial.println(pResist2);

  lcd.home (); // set cursor to 0,0
  lcd.print("Curr Light1: "+String(pResist1)+"   "); 
  lcd.setCursor (0,1);        // go to start of 2nd line;
  lcd.print("Curr Light2: "+String(pResist2)+"   "); 
  delay(3000);

//
//  if (daylight == true)
//  {
//    //Has the plant had enough light? If yes, set daylight to false and turn off relay
//    //Take Reading for each sensor and turn on relay if it is too dark
//    delay(50); 
//  }
//  else
//  {
//    //Take a reading and if it is bright enough, set daylight to true and start a timer
//
//    delay(50);
//  }

  

  
//  if (pResist1 >= 525)
//  {
//    digitalWrite(LightREL1, LOW);
//    Serial.println("Light 1 Relay On");
//  }
//  else 
//  {
//    digitalWrite(LightREL1, HIGH);
//    Serial.println("Light 1 Relay Off");
//  }
//
//    if (pResist2 >= 525)
//  {
//    digitalWrite(LightREL2, LOW);
//    Serial.println("Light 2 Relay On");
//  }
//  else 
//  {
//    digitalWrite(LightREL2, HIGH);
//    Serial.println("Light 2 Relay Off");
//  }


  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature(true);
  //Print temp and humidity values to serial monitor
  lcd.home (); // set cursor to 0,0
  lcd.print("Humidity "+String(hum)+"   "); 
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print("Temperature: "+String(temp)+"   "); 
  
  delay(5000); //Delay 2 sec.
}

void resetVals(){
  wateringTime = 0;
  waterPumped = 0;
  beingWatered1 = false;
  beingWatered2 = false;

}


