#include "DHT.h"
#include <Servo.h>
#include <Stepper.h> 
#include <LiquidCrystal.h>

//Pin configurations
#define STEPS 80 //360 turn
#define A 45
#define B 47
#define C 49
#define D 51
#define DHTPIN 53
#define DHTTYPE DHT11

int pos = 0;
int LED1 = 23;
int LED2 = 25;
int fan1 = 12;
int fan2 = 10;
int pump = 11;
int motor2 = 50;
int buzzer = 52;
int ldrPin = A14;
int IRSensor = A1;
int ledSensor = A15;
int LED_strip = 13;
int red_light_pin= 6;
int green_light_pin = 7;
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);
Servo myservo;

//variable configurations
const int LightLevelToSwitchAt = 50;
int i = 0;
int z = 0;
int a = 120;
int b = 240;
int c = 240;
int Counter = 0;
int mainCount = 0;
int ledSense = 0;
int syringeCount = 0;
int motorVal = 0;
int motorCount = 0;

void setup() {
  pinMode(A,OUTPUT);           //Servomotor
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(pump, OUTPUT);       //Pump
  pinMode(buzzer, OUTPUT);
  pinMode (ldrPin, INPUT);
  pinMode(motor2, OUTPUT);
  pinMode (IRSensor, INPUT);   //IRSensor
  pinMode (ledSensor, INPUT);  //LedSensor
  pinMode (LED_strip, OUTPUT); //Ledstrip
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);

  //initialising components
  noTone(buzzer); 
  RGB_color(0, 255); //Set LED as Green to signify start of system
  //ADD code for sending number to RASPI (SYSTEM ON)
  digitalWrite(pump, LOW);
  digitalWrite(LED_strip, LOW);
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  myservo.attach(43);          // attaches the servo on pin 43 to the servo object
  dht.begin(); 
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  Serial.print("status: on");
  delay(1000);
}

void write(int a,int b,int c,int d) //For motor
{
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}


void loop() {

  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature
  float volts = analogRead(IRSensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = (13*pow(volts, -1))*2;
  int ldrStatus = analogRead(ldrPin);
  int LightValue = analogRead(ledSensor);

  String myHumi = "";     // empty string
  myHumi.concat(humi);

  String myTemp = "";     // empty string
  myTemp.concat(tempC);

  Serial.print("temp:");     //Send temp over
  Serial.println(humi);
  Serial.print("humidity:"); //send humidity over
  Serial.println(tempC);
  delay(1000);


  if (distance == 8)
  {
    Counter = 1;
  }
  else 
  {
    if (mainCount == 0)
    {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please add");
    lcd.setCursor(0, 1);
    lcd.print("more water");
    delay(3000);
    }
    else 
    {
      delay(20);
    }
  }

 if (Counter == 1)
 {
  if ((mainCount == 0) && (LightValue < LightLevelToSwitchAt)) //start of operation, cover closed
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome to clean");
    lcd.setCursor(0, 1);
    lcd.print("TUBES");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please place");
    lcd.setCursor(0, 1);
    lcd.print("Syringes");
    delay(2000);
  }
  else if ((mainCount == 0) && (LightValue > LightLevelToSwitchAt)) //cover open
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Close cover");
    lcd.setCursor(0, 1);
    lcd.print("once placed");
    delay(1000);
    mainCount = 1;
  }

  else if ((mainCount == 1) && (LightValue < LightLevelToSwitchAt)) //syringes places and cover closed
  {
    Serial.print("stage: Process Starts");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Adding Water");
    lcd.setCursor(0, 1);
    lcd.print("Please wait");
    delay(2000);
    digitalWrite(pump, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(15000);
    digitalWrite(pump, LOW);
    mainCount = 2;
   }
   
   else if ((mainCount == 2) && (LightValue < LightLevelToSwitchAt))
   {
    Serial.print("stage: Washing");
    delay(1500);
    RGB_color(255, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Washing Syringes ");
    tone(buzzer, 1200); // Send 1KHz sound signal...
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(80); 
    tone(buzzer, 1200); // Send 1KHz sound signal...
    delay(100); 
    tone(buzzer, 1800);
    delay(150);
    noTone(buzzer);     // Stop sound...
    delay(2000);        // ...for 1sec
    delay(2000);
    
    while(i < 121)
    {
      int LightValue = analogRead(ledSensor);
      Serial.println(LightValue);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(a);
      lcd.print("s");
      a--;
      i++;
      delay(1000);
      if (LightValue > LightLevelToSwitchAt)
      {
        break;
      }
    } 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Clearing water");
    lcd.setCursor(0, 1);
    lcd.print("Please wait");
    delay(2000);
    digitalWrite(pump, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(15000);
    digitalWrite(pump, LOW);
    mainCount = 3;
  
  }
   else if ((mainCount == 3) && (LightValue < LightLevelToSwitchAt))
   {
    Serial.print("stage: Drying");
    delay(1500);
    stopMotor();
    int i = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Drying Syringes");
    delay(4000);

    while(i < 241)
    {
      onestep();
      int LightValue = analogRead(ledSensor);
      digitalWrite(fan1, HIGH);
      digitalWrite(fan2, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(b);
      lcd.print("s");
      b--;
      i++;
      delay(1000);
      if (LightValue > LightLevelToSwitchAt)
      {
        break;
      }
    }
    digitalWrite(fan1, LOW);
    digitalWrite(fan2, LOW);
    stopMotor();
    //ADD wait for yes from RASPI (IF loop)
    mainCount = 4;
   }
   else if ((mainCount == 4) && (LightValue < LightLevelToSwitchAt))
   {
    Serial.print("stage: Checking Syringes");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Checking clean");
    lcd.setCursor(0, 1);
    lcd.print("syringes");
    delay(2000);

    for (int i =0; i<6; i++)
    {
     while(motorVal<STEPS)
        {
          RGB_color(0, 255);
          onestep();
          motorVal++;
        }

     motorCount++;
      
     if (motorCount == 2)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataA);   // send the data
        delay(1500);


        if(data == 1)
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == 0)
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Syringes");
          lcd.setCursor(0, 1);
          lcd.print("not clean");
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

     else if (motorCount == 3)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataB);   // send the data
        delay(1500);
   

        if(data == "1")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == "0")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Syringes");
          lcd.setCursor(0, 1);
          lcd.print("not clean");
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

     else if (motorCount == 4)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataC);   // send the data
        delay(1500);


        if(data == "1")
        {
          String data = Serial.readStringUntil('\n');
         lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == "0")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

     else if (motorCount == 5)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataD);   // send the data
        delay(1500);


        if(data == "1")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == "0")
        {
          String data = Serial.readStringUntil('\n');
           lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

     else if (motorCount == 6)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataE);   // send the data
        delay(1500);

        if(data == "1")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == "0")
        {
          String data = Serial.readStringUntil('\n');
           lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

     else if (motorCount == 7)
     {
        String data = Serial.readStringUntil('\n');
        Serial.println(dataF);   // send the data
        delay(1500);

        if(data == "1")
        {
          String data = Serial.readStringUntil('\n');
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 0;
        }
        else if (data == "0")
        {
          String data = Serial.readStringUntil('\n');
           lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not Clean");
          lcd.setCursor(0, 1);
          lcd.print(data);
          motorVal = 100;
          stopMotor();
          motorCount = 8;
        }
     }

         else if (motorCount == 8)
        {
          Serial.print("stage: sterializing");
          delay(4000);
          motorCount = 9;
        }

     }
    mainCount = 5;
   }

   else if ((mainCount == 5) && (LightValue < LightLevelToSwitchAt))
   {
    Serial.print("stage: Sterializing");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sterilising");
    lcd.setCursor(0, 1);
    lcd.print("syringes");
    delay(2000);
    
    for (int i=0; i<80; i++)
    {
    digitalWrite(motor2, HIGH);
    delay(5);
    digitalWrite(motor2, LOW);
    delay(38);
    }

    if(ldrStatus <=1000)
    {
    while(z < 241)
    {
      int LightValue = analogRead(ledSensor);
      digitalWrite(LED_strip, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(c);
      lcd.print("s");
      c--;
      z++;
      delay(1000);
      if (LightValue > LightLevelToSwitchAt)
      {
        break;
      }
    }
    digitalWrite(LED_strip, LOW);
    mainCount = 6;
   }
   }
   
   else if ((mainCount == 6) && (LightValue < LightLevelToSwitchAt))
   {
    RGB_color(0, 255); // Green
    tone(buzzer, 900); // Send 1KHz sound signal...
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(80); 
    tone(buzzer, 700); // Send 1KHz sound signal...
    delay(100); 
    noTone(buzzer);     // Stop sound...
    delay(80); 
    tone(buzzer, 500);
    delay(150);
    noTone(buzzer);     // Stop sound...
    delay(2000);        // ...for 1sec
    Serial.print("stage: Process Complete");
    delay(4000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Process complete");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You can");
    lcd.setCursor(0, 1);
    lcd.print("retrieve syringes");
    delay(3000);
   }
   

   else if ((mainCount == 1) && (LightValue > LightLevelToSwitchAt))
   {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Close cover");
    lcd.setCursor(0, 1);
    lcd.print("once placed");
    Serial.println("wait");
    delay(1000);
   }
   else if ((mainCount > 1 && mainCount < 6) && (LightValue > LightLevelToSwitchAt))
   {
    Serial.print("stage: Cover Open ERROR");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cover Open");
    lcd.setCursor(0, 1);
    lcd.print("Needs to restart");
    digitalWrite(pump, LOW);
    digitalWrite(LED_strip, LOW);
    stopMotor();
    RGB_color(255, 0);
    delay(2000);
    RGB_color(0, 0);
    delay(2000);
    //add everything shut down [low]
    mainCount = 7;
    delay(1000);
   }
   
   else if ((mainCount == 6) && (LightValue > LightLevelToSwitchAt))
   {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thank you");
    lcd.setCursor(0, 1);
    lcd.print("Have a good day");
   }
 }

 else
 {
  delay(800);
 }
   
}

void RGB_color(int red_light_value, int green_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
}

void onestep(){
write(1,0,0,0);
delay(2);
write(1,1,0,0);
delay(2);
write(0,1,0,0);
delay(2);
write(0,1,1,0);
delay(2);
write(0,0,1,0);
delay(2);
write(0,0,1,1);
delay(2);
write(0,0,0,1);
delay(2);
write(1,0,0,1);
delay(2);
}

void stopMotor(){
write(0,0,0,0);
delay(2);
}
