#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pinul analogic la care este conectat LM35
const int tempPin = A0;

// Variabile pentru timp
volatile int ore = 18;
volatile int minute = 1;
volatile int secunde = 55;
volatile bool updateDisplay = true;

void setup() {
  lcd.begin(16, 2);
  
  //Configurare timer1
  cli();

  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1  = 0; 

  // Setează registrul de comparare pentru a obține o întrerupere la 1Hz
  OCR1A = 15624;

  // Pornește modul CTC
  TCCR1B |= (1 << WGM12);

  // Setează biții CS12 și CS10 pentru prescaler de 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Activează întreruperea la potrivirea cu OCR1A
  TIMSK1 |= (1 << OCIE1A);

  sei();
}

ISR(TIMER1_COMPA_vect) {
  secunde++;
  if (secunde >= 60) {
    secunde = 0;
    minute++;
    if (minute >= 60) {
      minute = 0;
      ore++;
      if (ore >= 24) {
        ore = 0;
      }
    }
  }
  updateDisplay = true;
}

void loop() {
  if (updateDisplay) {
    updateDisplay = false;
    
    //Citire temperatura ADC
    int rawADC = analogRead(tempPin);
    
    // Convertire în grade Celsius
    float tempC = (rawADC * 500.0) / 1024.0;

    //Afisare pe LCD
    lcd.setCursor(0, 0);
    lcd.print("Temp = ");
    lcd.print(tempC, 0);
    lcd.print("'C  ");

    lcd.setCursor(0, 1);
    lcd.print("Ora ");
    
    if (ore < 10) lcd.print("0");
    lcd.print(ore);
    lcd.print(":");
    
    if (minute < 10) lcd.print("0");
    lcd.print(minute);
    lcd.print(":");
    
    if (secunde < 10) lcd.print("0");
    lcd.print(secunde);
  }
}