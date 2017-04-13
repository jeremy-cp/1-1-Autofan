/* PWR       FF629D
  CH          FFE21D
  |<<          FF22DD
  >|            FFC23D
  >>|          FF02FD
  -              FFE01F
  Plus        FFA857
  EQ          FF906F
  0             FF6897
  100         FF9867
  200         FFB04F
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
boolean motorOn = false;
boolean power = false;
int autoMotorSpeed = 0;
int manMotorSpeed = 0;
int motorMan = 0;

int button = 13; //DIGITAL 11
int led = 9;
int t = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("<IR receiver: ENABLED>");
  Serial.println("<Temp. sensor: ENABLED>");
  Serial.println("<Fan: READY>\n\n");
}

void autoManIR() {
  switch (results.value) {
    case 0xFFA25D:
      autoMan = !autoMan;
      motorOn = autoMan;
      Serial.print("Button; CH- |   AUTO/MAN: ");
      Serial.println(autoMan);
      irrecv.resume();
      break;
    case 0xE318261B:
      autoMan = !autoMan;
      motorOn = autoMan;
      Serial.print("Button; CH- |   AUTO/MAN: ");
      Serial.println(autoMan);
      irrecv.resume();
      break;


    case 0xFFE21D:
      power = !power;
      Serial.print("Button: CH+ |   Motor: ");
      Serial.println(power);
      irrecv.resume();
      break;
    case 0xEE886D7F:
      power = !power;
      Serial.print("Button: CH+ |   Motor: ");
      Serial.println(power);
      irrecv.resume();
      break;
  }
}
void translateIR() {
  switch (results.value) {
    case 0xFF30CF:
      Serial.println("Button: 1   |   Motor: HIGH  ");
      t = 1;
      irrecv.resume();
      break;
    case 0x9716BE3F:
      Serial.println("Button: 1   |   Motor: HIGH  ");
      t = 1;
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

  DHT.read11(dht_dpin);
  double tempFahr = round(DHT.temperature * 1.8) + 32;
  int temp = map(tempFahr, 70, 75, 80, 255);
  temp = constrain(temp, 0, 255);

  if (irrecv.decode(&results))
  {
    autoManIR();

    if (autoMan == false)
    {
      translateIR();
      //Serial.println(results.value, HEX);
    }
    irrecv.resume(); // Receive the next value
  }
  if (power == false)
  {
    digitalWrite(motor, LOW);
    digitalWrite(led, LOW);
  }
  else
  {
    /*if (t == 0)
      {
      digitalWrite(motor, LOW);
      digitalWrite(led, LOW);
      }*/
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

    if (autoMan == true)
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
        delay(2);
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
  }
}


