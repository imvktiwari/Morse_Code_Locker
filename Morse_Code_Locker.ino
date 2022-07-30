#include<LiquidCrystal.h>
#include<Servo.h>

LiquidCrystal lcd(5, 6, 7,8, 9,10);

Servo Myservo;
int pos;

unsigned long signal_len,t1,t2;   //time for which button is pressed
int inputPin = 2;                 //input pin for push button
int ledPin = 4;                   //output pin for LED
String code = "";                 //to take input of morse code
String pwd="";                    //to store final password
String crct="ISM";                //to store correct password
int flag;                         //flag variable
int error=0;                      //flag variable for error input

void setup()
{
  Myservo.write(0);
  Myservo.attach(3);
  lcd.begin(16,2);
  lcd.write("Enter Your Password");
  pinMode(inputPin, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(ledPin,OUTPUT);
  delay(1000);                     
  lcd.clear();
}

void loop()
{
NextDotDash:
  while (digitalRead(inputPin) == HIGH) {}
  t1 = millis();                            //time at button press
  digitalWrite(ledPin, HIGH);               //LED on while button pressed
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();                            //time at button release
  digitalWrite(ledPin, LOW);                //LED off on button release
  signal_len = t2 - t1;                     //time for which button is pressed
  if (signal_len > 50)                      //to account for switch debouncing
  {
    code += readio();                       //function to read dot or dash
  }
  if(pwd.length()==3)                       //Assumed password length to be 3
  {
    flag=check();
    if(flag==0)                             //Password Entered is Correct
    {
      lcd.clear();
      lcd.write("Correct Password");
      for(pos=0;pos<=180;pos++)
      {
        Myservo.write(pos);
        delay(15);
      }
    }
    else                                    //Password Entered is Incorrect
    {
      lcd.clear();
      lcd.write("Wrong Password");
    }
    exit(0);
  }
  while ((millis() - t2) < 500)           //if time between button press greater than 0.5sec, skip loop and go to next alphabet
  {     
    if (digitalRead(inputPin) == LOW)
    {
      goto NextDotDash;
    }
  }
  convertor();        //function to decipher code into alphabet
  if(error==1)
    exit(0);
}


int check()
{
  int i;
  int f=0;
  for(i=0;i<=3;i++)
  {
    if(pwd[i]!=crct[i])
      f=1;                   //f is also flag variable
  }
  return f;
}


char readio()
{
  if (signal_len < 600 && signal_len > 50)
  {
    return '.';                        //if button press less than 0.6sec, it is a dot
  }
  else if (signal_len > 600)
  {
    return '-';                        //if button press more than 0.6sec, it is a dash
  }
}


void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
                            };
  int i = 0;
  while (letters[i] != "E")  //loop for comparing input code with letters array
  {
    if (letters[i] == code)
    {
      pwd+=char('A' + i);
      lcd.write(char('A' + i));
      break;
    }
    i++;
  }
  if (letters[i] == "E")
  {
    error=1;
    lcd.clear();
    lcd.write("Error");  //if input code doesn't match any letter, error
  }
  code = "";                            //reset code to blank string
}