#include "Arduino.h"


const int trigPin1 = 34;
const int echoPin1 = 35;
#define redled 32
#define grnled 33







long duration1;
int distance1;
void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
  Serial.begin(115200);
  pinMode(32,OUTPUT);

}
void loop()
{

  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);
  if (distance1 <= 15)
  {
    digitalWrite(32, HIGH);


    delay(1500);

  }
  else
  {

    digitalWrite(32, LOW);
  }

}
