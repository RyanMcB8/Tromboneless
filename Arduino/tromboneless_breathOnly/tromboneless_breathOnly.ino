
// Code for arduino with IR balloon breath sensor & one PWM out i.e. LED

// These constants won't change. They're used to give names to the pins used:
const int photodiodeInPin = A0;  // Analog input pin that the potentiometer is attached to
const int volumeOutPin = 9;  // Analog output pin that the LED is attached to

int photodiodeInValue = 0;  // value read from the photodiode
int volumeOutValue = 0;  // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(volumeOutPin, OUTPUT);  // set D9 as volume output pin
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  photodiodeInValue = analogRead(photodiodeInPin);
  // map it to the range of the analog out:
  volumeOutValue = map(photodiodeInValue, 200, 100, 0, 127);
  if(volumeOutValue<0){
    volumeOutValue=0;
  }
  else if(volumeOutValue>127){
    volumeOutValue=127;
  }
  // change the digital out value (analog write is actually pwm digital):
  analogWrite(volumeOutPin, volumeOutValue);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(photodiodeInValue);
  Serial.print("\t output = ");
  Serial.println(volumeOutValue);


  // wait 2 milliseconds before the next loop for the analog-to-digital
  delay(2);
}
