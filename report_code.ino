#define BLYNK_TEMPLATE_ID "TMPL3A6JNa5by"
#define BLYNK_TEMPLATE_NAME "BTP DEMO"
#define BLYNK_AUTH_TOKEN "VORLbYb3cgbNtxVSLmhNdzcqx5n2eWKp"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "bidhan";         
char pass[] = "aaaaaa123456";
//You can change the wifi name and password as your device

BlynkTimer timer;

#define Speed1 25
#define Speed2 33
#define Speed4 32

#define LED1 2
#define LED2 4
#define LED3 5
#define LED4 18
#define trig 19   
#define echo 15   

#define pump_relay 14

int relay1_state = 0;
int pump_relaystate = 0;
int relay3_state = 0;

int MaxLevel = 125;

int Level1 = (MaxLevel * 90) / 100;
int Level2 = (MaxLevel * 75) / 100;
int Level3 = (MaxLevel * 50) / 100;
int Level4 = (MaxLevel * 20) / 100;

// int Level5 = (MaxLevel * 98) /100;

#define button2_vpin    V2

BLYNK_CONNECTED() {
  Blynk.syncVirtual(button2_vpin);
}

BLYNK_WRITE(button2_vpin) {
  pump_relaystate = param.asInt();
  digitalWrite(pump_relay, pump_relaystate);
}

BLYNK_WRITE(V5)
{
  int fan_speed = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (fan_speed == 0)
  {
    //Set all realys Off - Fan Off
    
    digitalWrite(Speed1, HIGH);
    digitalWrite(Speed2, HIGH);
    digitalWrite(Speed4, HIGH);
  }

  if (fan_speed == 1)
  {
    //Speed1 Relay On - Fan at speed 1
    digitalWrite(Speed1, HIGH);
    digitalWrite(Speed2, HIGH);
    digitalWrite(Speed4, HIGH);
    delay(100);
    digitalWrite(Speed1, LOW);
  }

  if (fan_speed == 2)
  {
    //Speed2 Relay On - Fan at speed 2
    digitalWrite(Speed1, HIGH);
    digitalWrite(Speed2, HIGH);
    digitalWrite(Speed4, HIGH);
    digitalWrite(Speed2, LOW);
  }

  if (fan_speed == 3)
  {
    //Speed1 & Speed2 Relays On - Fan at speed 3
    digitalWrite(Speed1, HIGH);
    digitalWrite(Speed2, HIGH);
    digitalWrite(Speed4, HIGH);
    digitalWrite(Speed1, LOW);
    digitalWrite(Speed2, LOW);
  }

  if (fan_speed == 4)
  {
    //Speed4 Relay On - Fan at speed 4
    
    digitalWrite(Speed1, HIGH);
    digitalWrite(Speed2, HIGH);
    digitalWrite(Speed4, HIGH);
    digitalWrite(Speed4, LOW);
  }
}


void setup(){
  // Debug console
  Serial.begin(115200);

  pinMode(pump_relay, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(pump_relay, HIGH);

  pinMode(Speed1, OUTPUT);
  pinMode(Speed2, OUTPUT);
  pinMode(Speed4, OUTPUT);

  //Initially the fan will be in OFF state
  digitalWrite(Speed1, HIGH);
  digitalWrite(Speed2, HIGH);
  digitalWrite(Speed4, HIGH);

  Blynk.begin(auth, ssid, pass);
}

void loop(){
  Blynk.run();
  timer.run();
  ultrasonic();
}

void control_relay(int relay){
  if(relay == 1){
    relay1_state = !relay1_state;
    // digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    delay(50);
  }
  else if(relay == 2){
    pump_relaystate = !pump_relaystate;
    digitalWrite(pump_relay, pump_relaystate);
    delay(50);
  }
  else if(relay == 3){
    relay3_state = !relay3_state;
    // digitalWrite(relay3_pin, relay3_state);
    delay(50);
  }
}

void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH);
  int distance = t / 29 / 2;
  Serial.println(distance);

  int blynkDistance = (distance - MaxLevel) * -1;
  if (distance <= MaxLevel) {
    Blynk.virtualWrite(V3, blynkDistance);
  } else {
    Blynk.virtualWrite(V3, 0);
  }
  if (Level1 <= distance) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  } else if (Level2 <= distance && Level1 > distance) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  } else if (Level3 <= distance && Level2 > distance) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
  } else if (Level4 <= distance && Level3 > distance) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    pump_relaystate = !pump_relaystate;
    digitalWrite(pump_relay, pump_relaystate);
    delay(1300);
  }
}