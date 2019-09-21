#include <OneWire.h>
#include <DallasTemperature.h>
#include <CapacitiveSensor.h>

int button=5;
int motor=11;
int ledPin=10;
int gerkon1=4;
// 7-6 capsense
// 2 - termometr
int closE=A2;//червоний діод
int opeN=A1;//зелений діод
int signalization1=A3;//синій
int signalization2=A4;//червоний
int dvir=9;
int col1=A0 ;
int col2=A6 ;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(7,6);

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1 = { 
0x28, 0x9A, 0x57, 0x57, 0x07, 0x00, 0x00, 0x80 };
//28 9A 57 57 07 00 00 80

DeviceAddress Thermometer2 = { 
0x28, 0x7B, 0x3C, 0x56, 0x07, 0x00, 0x00, 0x70 }; 
//28 7B 3C 56 07 00 00 70 

boolean lastButton = LOW;
boolean currentButton = LOW;
int ledLevel = 0;

void setup() {
 Serial.begin(9600);
 cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 
 
 pinMode(ledPin,OUTPUT);
 pinMode(button,INPUT);
 pinMode(signalization1,OUTPUT);
 pinMode(signalization2,OUTPUT);
 pinMode(gerkon1,INPUT);//dveri
 pinMode(dvir,OUTPUT);
 pinMode(closE,OUTPUT);
 pinMode(opeN,OUTPUT);
 pinMode(col1,OUTPUT);
 pinMode(col2,OUTPUT);
 
sensors.begin();
sensors.setResolution(Thermometer1, 2);
sensors.setResolution(Thermometer2, 2);
}
boolean debounce (boolean last){
  boolean current = digitalRead(button);
  if(last=!current){
    //delay(5);
    current = digitalRead(button);
  }
  return (current);
}

void ohorona(){
  digitalWrite(signalization1,HIGH);
  digitalWrite(signalization2,LOW);
  delay(150);
  digitalWrite(signalization1,LOW);
  digitalWrite(signalization2,HIGH);
  delay(50);
  }
  
  
void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.println(tempC);}

void loop() {
  long start = millis();
  long total1 =  cs_4_2.capacitiveSensor(30);
  delay (50);
    
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH){
     ledLevel = ledLevel + 51 ;  }
     lastButton = currentButton;
     if (ledLevel>255) ledLevel = 0;
     analogWrite (ledPin,ledLevel);

sensors.requestTemperatures();
Serial.print("Sensor1  ");
printTemperature(Thermometer1);
Serial.print("Sensor2  ");
printTemperature(Thermometer2);
float tempC1 = sensors.getTempC(Thermometer1);

int val1 = map ( tempC1 ,24 ,40 ,95 ,255);
analogWrite(motor,val1);

float tempC2 = sensors.getTempC(Thermometer2);
constrain (tempC2,25,32);
int val2 = map(tempC2,25,32,0,250);
analogWrite(dvir,val2);

// system of safety
if (digitalRead(gerkon1) == LOW){
  digitalWrite(closE,HIGH);
  digitalWrite(opeN,LOW);
}
if (digitalRead(gerkon1) == HIGH){
  digitalWrite(closE,LOW);
  digitalWrite(opeN,HIGH);
}


// system perumetr guardens
if (total1>6000 && digitalRead(gerkon1)==0)ohorona();
else {
  digitalWrite(A3,LOW);
  digitalWrite(A4,LOW);
}

}

  

