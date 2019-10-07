#include "ADXL345.h"

int trig = 3;
int echo = 4;
int buzzer = 2;
long dur = 0;
float distance = 0;
long Pins[] = {5, 6, 7, 8, 9, 10};
long State[] = {100000, 101000, 110000, 110100, 100100, 111000, 111100, 101100, 11000, 11100, 100010, 101010, 110010, 110110, 100110, 111010, 111110, 101110, 11010, 11110, 100011, 101011, 11101, 110011, 110111, 100111, 1101, 1000, 1011, 1110, 10, 11, 1, 10111, 11100, 100000, 101000, 110000, 110100, 100100, 111000, 111100, 101100, 11000};
int startLetters = 0;
int startDigits = 34;
int startSigns = 26;
char s[] = ".,?!'-";
int capital = 32;
char text[] = "Polihack V6.0";
int button = 11;

ADXL345 accelerometer;

void setup(){
  Serial.begin(9600);
  accelerometer.begin();
  
  Serial.println("");
  pinMode(trig, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echo, INPUT);
  for (int i = 0;i < 6; ++i){
    pinMode(Pins[i], OUTPUT);
  }
  pinMode(button, INPUT);
  pinMode(12, OUTPUT);
  accelerometer.setFreeFallThreshold(0.35);
  accelerometer.setFreeFallDuration(0.1);

  accelerometer.useInterrupt(ADXL345_INT1);
}

void loop(){
  digitalWrite(buzzer, HIGH);
  Vector norm = accelerometer.readNormalize();
  Activites activ = accelerometer.readActivites();
  
  if (activ.isFreeFall)
  {
    
    Serial.println("Free Fall Detected!");
    digitalWrite(buzzer, LOW);
    delay(5000);
    digitalWrite(buzzer, HIGH);
  }
  
  
 
  digitalWrite(trig, LOW);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  dur = pulseIn(echo, HIGH);
  distance = dur * 340 / 20000;
  Serial.println(distance);
  Serial.println(digitalRead(button));
  
  if (digitalRead(button) == HIGH){
    showMessage();
  }
  else
  if (distance <= 50){
    digitalWrite(buzzer, LOW);
    delay(distance * 10);
    digitalWrite(buzzer, HIGH);
    delay(distance * 10);
  }
}

void showMessage(){
  long aux;
  for (int k = 0; k <= strlen(text); ++k){
      if ('a' <= text[k] && 'z' >= text[k])
        aux = State[text[k] - 'a'], showPins(aux);
      else if (strchr(s, text[k]))
        aux = State[startSigns + strchr(s, text[k]) - s], showPins(aux);
      else if ('0' <= text[k] && '9' >= text[k])
        aux = State[startDigits + text[k] - '0'], showPins(aux);
      else if ('A' <= text[k] && 'Z' >= text[k]){
        aux = State[capital];
        showPins(aux);
        delayReset();
        aux = State[text[k] - 'A'];
        showPins(aux);
      }
      delayReset();
   }
}

void showPins(long state){
  
  for (int i = 5; i >= 0; --i){
    if (state % 10 != 0){
      digitalWrite(Pins[i], HIGH);
    }
    state /= 10;
  }
}

void delayReset(){
  delay(500);
  for (int i = 0; i < 6; ++i){
    digitalWrite(Pins[i], LOW);
  }
  delay(500);
}

