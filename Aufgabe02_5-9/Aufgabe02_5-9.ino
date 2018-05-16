#include <SoftwareSerial.h>

#define tonePin 12
boolean led = true;
int prescalers[7] = {1, 8, 32, 64, 128, 256, 1024};
volatile uint32_t tCount = 0;
unsigned long f = 1000;

void setup(){
  Serial.begin(115200);
  pinMode(tonePin, OUTPUT);
  every1ms();
}


// Excercise 5
void tone100Hz() {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 154;
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS22);
  TIMSK2 |= (1 << OCIE1A);  
  interrupts();             
}


// Excercise 6
void toneAdjustableFreq(int freq) {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  TIMSK2 |= (1 << OCIE1A);
  for (int i = 0; i < 7; i++) {
    int result = round( (8000000 / (prescalers[i] * freq)) - 1);
    if (result < 256) {
      OCR2A = result;
      switch (prescalers[i]) {
        case (1):
          TCCR2B |= (1 << CS20);
          break;
        case (8):
          TCCR2B |= (1 << CS21);
          break; 
        case (32):
          TCCR2B |= (1 << CS20);
          TCCR2B |= (1 << CS21);
          break; 
        case (64):
          TCCR2B |= (1 << CS22);
          break; 
        case (128):
          TCCR2B |= (1 << CS20);
          TCCR2B |= (1 << CS22);
          break; 
        case (256):
          TCCR2B |= (1 << CS21);
          TCCR2B |= (1 << CS22);
          break; 
        case (1024):
          TCCR2B |= (1 << CS20);
          TCCR2B |= (1 << CS21);
          TCCR2B |= (1 << CS22);
          break; 
        }
      break;
      }
    }
    interrupts();
  }


// Excercise 7
void every1ms() {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 30;
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS22);
  TIMSK2 |= (1 << OCIE1A);  
  interrupts();  
  }


// Interrupt
ISR(TIMER2_COMPA_vect) {
  tCount += 1;
  TCNT2 = 0;  
  led = not led;
  digitalWrite(tonePin, led);
  }


void loop() {
  if (millis() > f) {
    f += 1000;
    Serial.println(tCount);
  }
}
