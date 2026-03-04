
#include "trombone_notes.h"
#include "expFilter.h"
#include <CapacitiveSensor.h>

CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2);

const int toneReadPin = A1;
float toneRead;

int notes[] = {
  NOTE_AS2, NOTE_AS3, NOTE_F4, NOTE_AS4, NOTE_D5, NOTE_F5
};
int oldLipReading;

void setup(){
  Serial.begin(9600);
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

void loop(){

  toneRead = analogRead(toneReadPin);
  long start = millis();
  long lipReading = cs_4_2.capacitiveSensor(30);
  lipReading = expFilter(lipReading, oldLipReading, 1024, 0.005);

  Serial.print(millis() - start);
  Serial.print("\t");
  Serial.print(lipReading);
  Serial.print("\t");
  Serial.println(toneRead);

   if (lipReading>800){
    tone(9, notes[5], 100);
  }
  else if (lipReading>700){
    tone(9, notes[4], 100);
  }
  else if (lipReading>600){
    tone(9, notes[3], 100);
  }
    else if (lipReading>500){
    tone(9, notes[2], 100);
  }
    else if (lipReading>400){
    tone(9, notes[1], 100);
  }
    else if (lipReading>300){
    tone(9, notes[0], 100);
  }

  oldLipReading = lipReading;
  delay(10);
}