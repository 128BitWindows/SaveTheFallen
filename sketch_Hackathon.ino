#include <IRremote.h>
#include <LiquidCrystal.h>

IRrecv rcvr(13);
decode_results results;

int Contrast = 60;
LiquidCrystal lcd(12, 11, 8, 4, 3, 2);

byte alarmemote[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b11111
};

byte exclaim[8] = {
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b00100,
  0b00000,
  0b01110,
  0b01110
};


void setup() {
  // put your setup code here, to run once:
  lcd.createChar(1, alarmemote);
  lcd.createChar(2, exclaim);

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  rcvr.enableIRIn();
  analogWrite(10, Contrast);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool toggle;
  static int message;

  //IR sensor stuff
  if(rcvr.decode(&results))
  {
    Serial.println(results.value, DEC);
    rcvr.resume();
  }
  Serial.println(analogRead(A5));
  delay(100);
  long val = results.value;
  long sound = analogRead(A5);

  switch (val)
  {
    case 16738455:
      message = 0;
      break;
    case 16724175:
      message = 1;
      break;
    case 16718055:
      message = 2;
      break;
    case 16743045:
      message = 3;
      break;
    case 16750695: //EQ Button
      message = 99;
      break;
    case 16756815: //ST Button
      message = 98;
      break;      
  }

  switch (message)
  {
    case 0:
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" CURRENT  STATE ");
      lcd.setCursor(5, 1);
      lcd.print("MANUAL");

      toggle = false;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write((byte)2);
      lcd.write((byte)2);
      lcd.print(" HELP ALERT ");
      lcd.write((byte)2);
      lcd.write((byte)2);

      lcd.setCursor(0, 1);
      lcd.print(" FALLEN  PERSON ");

      toggle = true;

      //delay(1000);
      //lcd.clear();
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NEED");
      lcd.setCursor(0, 1);
      lcd.print("WATER");
      break;
    case 99:
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" CURRENT  STATE ");
      lcd.setCursor(3, 1);
      lcd.print("ON STANDBY");

      toggle = false;

      if(sound > 100) message = 1;
      break;     
  }

  if(toggle) //alert lights
  {
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    delay(300);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    delay(300);
  }
}
