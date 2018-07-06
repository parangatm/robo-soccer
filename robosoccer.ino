/* 
Soccer Robot
Arduino 2560
*/

#include <PS2X_lib.h>
#include <Servo.h>

#define PS2_DAT        12
#define PS2_CMD        13  
#define PS2_SEL        11  
#define PS2_CLK        10  

/* Motor driver 1 has motors {enA,in1,in2} and {enC, in5, in6}
Motor driver 2 has motor {enB, in3, in4}
*/

#define enA 2   //Motor connected on driver 1
#define in1 52
#define in2 3

#define enB 6   //Motor connected on driver 2
#define in3 5
#define in4 4

#define in5 8   //Motor connected on driver 1
#define in6 9
#define enC 7


Servo myservo;

int RY, RX, LX, LY, mapped;

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

int error = 0;
byte type = 0;
byte vibrate = 0;
int flag = 0;

void setup()
{

  Serial.begin(57600);
  myservo.attach(24); 
  delay(300);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0)
  {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  
    if (pressures)
      Serial.println("true ");

    else
        Serial.println("false");
        Serial.print("rumble = ");

        if (rumble)
          Serial.println("true)");

        else
          Serial.println("false");
          Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
          Serial.println("holding L1 or R1 will print out the analog stick values.");
          Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  
  switch(type) 
  {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
      
    case 1:
      Serial.print("DualShock Controller found ");
      break;
      
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
      
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
 
}

void moveForward()
{
  mapped = 255 - (2*LY);     
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, mapped);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, mapped);
}

void moveBackward()
{
  mapped = (2*LY) - 255;    
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, mapped);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, mapped);
}

void turnright()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 255);
  analogWrite(enC, 255);
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
}

void turnleft()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 255);
  analogWrite(enC, 255);
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
}

void halt()
{
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  analogWrite(enC, 0);
}

void moveleft()
{
  mapped = 255 - (2*LX);
  mapped = mapped*4  /5;
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in5, LOW); 
  digitalWrite(in6, HIGH); 
  analogWrite(enC, mapped);
}


void moveright()
{
  mapped = (2*LX) - 255;
  mapped = mapped*4/5; 
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in5, HIGH); 
  digitalWrite(in6, LOW); 
  analogWrite(enC, mapped);
}

void loop()
{
  
  if(error == 1) //skip loop if no controller found
    return; 
    
  else  //DualShock Controller 
  { 
    
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  
    LX = ps2x.Analog(PSS_LX);
    LY = ps2x.Analog(PSS_LY);

    if(LY>127)
      {
        moveBackward();
        delay(100);
      }

    if(LY<127)
      {
        moveForward();
        delay(100);
      }

    if(LX<127)
      {
        moveleft();
        delay(30);
      }
    
    if(LX>127)        
    {
      moveright();
      delay(30);
    }
    
    
    if(ps2x.Button(PSB_R1))   
    {
      halt();
    }
    
    if(ps2x.ButtonPressed(PSB_L2))      //turning left
    {  
      turnleft();
      delay(40);
      
      if(ps2x.ButtonReleased(PSB_L2))
        halt();
    }

    if(ps2x.ButtonPressed(PSB_R2))     //turning right
    {
      turnright();
      delay(40);
      
      if(ps2x.ButtonReleased(PSB_R2))
      halt();
     }

     if(ps2x.ButtonPressed(PSB_CROSS))    //Servo flap in one direction
     {
      myservo.write(180);
      delay(10);
      }

      if(ps2x.ButtonPressed(PSB_TRIANGLE))    //Servo flap in other direction
      {
        myservo.write(530);
        delay(10);
      }
 
  }
  
  delay(30);
  
}
