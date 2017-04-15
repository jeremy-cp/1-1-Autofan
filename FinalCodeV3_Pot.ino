/* PWR          FF629D
  CH            FFE21D
  |<<           FF22DD
  >|            FFC23D
  >>|           FF02FD
  -             FFE01F
  Plus          FFA857
  EQ            FF906F
  0             FF6897
  100           FF9867
  200           FFB04F
  1             FF30CF
  2             FF18E7
  3             FF7A85
  4             FF10EF
  5             FF38C7
  6             FF5AA5
  7             FF42BD
  8             FF48B5
  9             FF52AD */
#include <dht.h>
#define dht_dpin A0 //ANALOG 0
dht DHT;

#include <IRremote.h>
int RECV_PIN = 7; //IR PIN
IRrecv irrecv(RECV_PIN);
decode_results results;

int motor = 11; //PWM/DIGITAL 11
boolean autoMan = false;
boolean motorOn = true;
boolean power = false;
int autoMotorSpeed = 0;
int manMotorSpeed = 0;
int motorMan = 0;

int button = 13; //DIGITAL 11
int led = 9;
int t = 0;

boolean track;

int potentiometer = 5;
boolean pot;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Group 1-1 Auto-adjusting Fan\n");
  delay(50);
  Serial.print("<IR receiver: ");
  delay(50);
  Serial.println("READY>");
  delay(50);
  Serial.print("<Temp. sensor: ");
  delay(50);
  Serial.println("READY>");
  delay(50);
  Serial.print("<Fan: ");
  delay(50);
  Serial.println("READY>\n\n");
}

void autoManIR() {
  switch (results.value) {
    case 0xFF38C7:
      autoMan = !autoMan;
      Serial.print("Button; 5 |   AUTO/MAN: ");
      Serial.println(autoMan);
      irrecv.resume();
      break;
    case 0x488F3CBB:
      autoMan = !autoMan;
      Serial.print("Button; 5 |   AUTO/MAN: ");
      Serial.println(autoMan);
      irrecv.resume();
      break;
    case 0xFFA25D:
    Serial.println("Button: CH-   |   Motor: OFF");
      t = 0;
      autoMan = false;
      motorOn = false;
      irrecv.resume();
      break;
    case 0xE318261B:
    Serial.println("Button: CH-   |   Motor: OFF");
      t = 0;
      autoMan = false;
      motorOn = false;
      irrecv.resume();
      break;


    case 0xFFE21D:
      Serial.println("Button: CH+   |   Motor: ON  ");
      motorOn = true;
      irrecv.resume();
      break;
    case 0xEE886D7F:
      Serial.println("Button: CH+   |   Motor: ON  ");
      motorOn = true;
      irrecv.resume();
      break;
  }
}
void translateIR() {
  switch (results.value) {
    case 0xFF30CF:
      Serial.println("Button: 1   |   Motor: HIGH  ");
      t = 1;
      track = true;
      irrecv.resume();
      break;
    case 0x9716BE3F:
      Serial.println("Button: 1   |   Motor: HIGH  ");
      t = 1;
      track = true;
      irrecv.resume();
      break;


    case 0xFF18E7:
      Serial.println("Button: 2   |   Motor: MED  ");
      t = 2;
      irrecv.resume();
      break;
    case 0x3D9AE3F7:
      Serial.println("Button: 2   |   Motor: MED  ");
      t = 2;
      irrecv.resume();


    case 0xFF7A85:
      Serial.println("Button: 3   |   Motor: LOW  ");
      t = 3;
      irrecv.resume();
      break;
    case 0x6182021B:
      Serial.println("Button: 3   |   Motor: LOW  ");
      t = 3;
      irrecv.resume();
      break;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  int potRead = analogRead(potentiometer);
  potRead = map(potRead, 0, 1023, 0, 255);
  int potTrack = constrain(potRead, 0, 255);

  DHT.read11(dht_dpin);
  double tempFahr = round(DHT.temperature * 1.8) + 32;
  int temp = map(tempFahr, 72, 80, 80, 255);
  temp = constrain(temp, 0, 255);

  if (irrecv.decode(&results))
  {
    autoManIR();

    if (autoMan == false && pot == false)
    {
      translateIR();
      //Serial.println(results.value, HEX);
    }
    irrecv.resume(); // Receive the next value
  }
  if (potTrack > 51)
      {
        autoMan = false;
      }
  {
    if (pot == false)
    {
      if (t == 0)
      {
        analogWrite(motor, 0);
        analogWrite(led, 0);
      }
      if (t == 1)
      {
        analogWrite(motor, 255);
        analogWrite(led, 255);
      }
      if (t == 2)
      {
        analogWrite(motor, 255);
        delay(400);
        analogWrite(motor, 0);
        delay(10);
        analogWrite(led, 160);
      }
      if (t == 3)
      {
        analogWrite(motor, 255);
        delay(255);
        analogWrite(motor, 0);
        delay(55);
        analogWrite(led, 90);
      }
    }

    if (autoMan == true && motorOn == true)
    {
      if (temp < 80)    {
        digitalWrite(motor, LOW);
        digitalWrite(led, LOW);
      }
      else if (temp >= 80 && temp < 115)
      {
        analogWrite(motor, 255);
        delay(170);
        analogWrite(motor, 0);
        delay(130);
      }
      else if (temp >= 115 && temp < 150)
      {
        analogWrite(motor, 255);
        delay(255);
        analogWrite(motor, 0);
        delay(55);
      }
      else if (temp >= 150 && temp < 185)
      {
        analogWrite(motor, 255);
        delay(400);
        analogWrite(motor, 0);
        delay(10);
      }
      else if (temp >= 185 && temp <= 255)
      {
        analogWrite(motor, 255);
      }

      digitalWrite(led, HIGH);
      delay(10);
      digitalWrite(led, LOW);

      Serial.print("temperature = ");
      Serial.print(tempFahr);
      Serial.print("F or ");
      Serial.print(DHT.temperature);
      Serial.print("C   |  ");
      Serial.print("analog value = ");
      Serial.println(temp);
      delay(400);
    }
    else if (autoMan == false && motorOn == true)
    {
      if (potTrack >= 0 && potTrack <= 51)
      {
        pot = false;
        //Serial.println(potRead);
      }
      else if (potTrack > 51 && potTrack <= 102)
      {
        autoMan = false;
        t = 0;
        pot = true;
        analogWrite(motor, 255);
        delay(170);
        analogWrite(motor, 0);
        delay(130);
        analogWrite(led, 102);
        //Serial.println(potRead);
      }
      else if (potTrack > 102 && potTrack <= 153)
      {
        autoMan = false;
        t = 0;
        pot = true;
        analogWrite(motor, 255);
        delay(255);
        analogWrite(motor, 0);
        delay(55);
        analogWrite(led, 153);
        //Serial.println(potRead);
      }
      else if (potTrack > 153 && potTrack <= 204)
      {
        autoMan = false;
        t = 0;
        pot = true;
        analogWrite(motor, 255);
        delay(400);
        analogWrite(motor, 0);
        delay(2);
        analogWrite(led, 204);
        // Serial.println(potRead);
      }
      else if (potTrack > 204 && potTrack <= 255)
      {
        autoMan = false;
        t = 0;
        pot = true;
        analogWrite(motor, 255);
        analogWrite(led, 255);
        //Serial.println(potRead);
      }
    }
  }
}
