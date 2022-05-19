#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int todaslasluces[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
int A = 22;
int marcha = 1;
int i = marcha;
int marchup = 0;
int marchdown = 0;
int rvup = 0;
int rvdown = 0;
int rev_old = 0;
int rev = 1000;
int roto = 0;
int radio = 0;
int subir = 135;
int finradio = 0;
int on = 0;

#include "pitches.h"

int melody[] = {
  NOTE_C5, NOTE_G4, NOTE_D5, NOTE_C5};

int yaesta[] = {
  NOTE_C5, NOTE_C5};

int marchupbut = 44;
int marchdownbut = 46;

int rvupbut = 50;
int rvdownbut = 48;

int embraguebut = 52;
int embraguecont = 0;

int radiobut = 53;
int duration = 100;

#include "LedControl.h"

LedControl lc=LedControl(4,2,3,1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lc.shutdown(0,true);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,0.1);
  /* and clear the display */
  lc.clearDisplay(0);

  pinMode(marchupbut, INPUT_PULLUP);
  pinMode(marchdownbut, INPUT_PULLUP);
  pinMode(rvupbut, INPUT_PULLUP);
  pinMode(rvdownbut, INPUT_PULLUP);
  pinMode(radiobut, INPUT_PULLUP);
  pinMode(embraguebut, INPUT_PULLUP);
  pinMode(6, OUTPUT); 


    for (int lights = 0; lights < 15; lights++) {
      pinMode(36-lights, LOW);
      pinMode(36-lights, OUTPUT);
    } 
    lcd.begin(16, 2);
    lcd.print("RPM: ");
    lcd.noDisplay();
}


void writeArduinoOnMatrix(int i) {
  /* here is the data for the characters */
  byte n[][8]={{0x0, 0x78, 0x44, 0x44, 0x78, 0x48, 0x44, 0x0},
                {0x0, 0x42, 0x62, 0x52, 0x4a, 0x46, 0x42, 0x0},
                {0x0, 0x8, 0x18, 0x8, 0x8, 0x8, 0x1c, 0x0},
                {0x0, 0x3c, 0x42, 0xc, 0x10, 0x20, 0x7e, 0x0},
                {0x0, 0x7e, 0x2, 0x2, 0x3e, 0x2, 0x7e, 0x0},
                {0x0, 0x44, 0x44, 0x7e, 0x4, 0x4, 0xe, 0},
                {0x0, 0x7e, 0x40, 0x7c, 0x2, 0x2, 0x7c, 0},
                {0x0, 0x38, 0x40, 0x40, 0x7c, 0x42, 0x3c, 0x0},
                {0x0, 0x7e, 0x4, 0x8, 0x3c, 0x8, 0x8, 0x0},
                {0x0, 0x3c, 0x42, 0x42, 0x3c, 0x42, 0x3c, 0x0},
                {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
                
  lc.setRow(0,0,n[i][0]);
  lc.setRow(0,1,n[i][1]);
  lc.setRow(0,2,n[i][2]);
  lc.setRow(0,3,n[i][3]);
  lc.setRow(0,4,n[i][4]);
  lc.setRow(0,5,n[i][5]);
  lc.setRow(0,6,n[i][6]);
  lc.setRow(0,7,n[i][7]);
  lc.setRow(0,8,n[i][8]);
  
}    

void embrague() {
      if (digitalRead(embraguebut) == LOW && (on == 0)) {
      // SE ENCIENDE:

      if (embraguecont == 0) {
        embraguecont = 1;
        on = 1;
        lcd.display();
        lcd.clear();
        lcd.print("RPM: ");
        Serial.print("onembrague");
        lc.shutdown(0,false);
        digitalWrite(6, HIGH); 
        digitalWrite(36, HIGH);
        roto = 0;
      }
      } else if (digitalRead(embraguebut) == LOW && (on == 1)) {
        //SE APAGA

        if (embraguecont == 0) {
        embraguecont = 1;
        marcha = 1;
        on = 0;
        lcd.noDisplay();
        lcd.clear();
        Serial.print("embragueoff");
        lc.shutdown(0,true);
        digitalWrite(6, LOW); 
        rev = 1000;
        roto = 1;
        for (int lights = 0; lights < 15; lights++) {
          digitalWrite(36-lights, LOW);
        }        
        }

    } else if (digitalRead(embraguebut) == HIGH) {
      embraguecont = 0;
    }
}


void radiocosa() {

   if (digitalRead(radiobut) == LOW  && (finradio == 0)) {
      if (radio == 0) {
        radio = 1;
        finradio = 1;
      for (int thisNote = 0; thisNote < 4; thisNote++) {
      delay(10);
      tone(13, melody[thisNote], duration);    
      delay(duration);
    }
      } 
    }else if (digitalRead(radiobut) == LOW && (finradio == 1)) {
      if (radio == 0) {
      radio = 1;
      finradio = 0;
      for (int thisNote = 0; thisNote < 2; thisNote++) {
      delay(10);
      tone(13, yaesta[thisNote], duration);    
      delay(duration);
      }
    }
    }else if (digitalRead(radiobut) == HIGH) {
      radio = 0;
    } 
    
}

void marchaup() {
  
    if (digitalRead(marchupbut) == LOW) {
      // turn LED on:
      if (marchup == 0) {
        marchup = 1;
        Serial.println("UP");
        if (marcha<9) {
          marcha++;
          rev=rev-5000;
          subir=subir-15;
        }

      } 
    } else if (digitalRead(marchupbut) == HIGH) {
      marchup = 0;
    }

}

void marchadown() {

    if (digitalRead(marchdownbut) == LOW) {
      // turn LED on:
      if (marchdown == 0) {
        marchdown = 1;
        Serial.println("DOWN");
        if (marcha>0) {
          marcha--; 
          if (marcha > 1)
          rev=rev+3000;
          subir=subir+10;
        }
      } 
    } else if (digitalRead(marchdownbut) == HIGH) {
      marchdown = 0;
    }

}

void led() {

  lcd.setCursor(5, 0);
  lcd.print(rev);
  lcd.print("      ");
  if (on == 1){
    lcd.display();
  } else if (on == 0) {
    lcd.noDisplay();
  }

}

void loop() {

embrague();

//Sonido del coche
if (rev >= 2000 && radio != 1) {
  tone(13, rev / 10 / 3, 50);  
}

if (on == 1){

radiocosa();
led();

if (roto == 0){

marchaup();
marchadown();

  if (digitalRead(rvdownbut) == LOW) {
        rev=rev-subir;
  }

  if (digitalRead(rvupbut) == LOW) {
        rev=rev+subir;
  }

}
}
// Roto
if (roto == 1) {
  if (digitalRead(rvdownbut) == LOW && digitalRead(rvupbut) == LOW) {
     marcha = 1;
  }
}

  int rev_max=13000;
  int rev_min=1000;

// Marcha = Normal
  if (marcha == 1){
    if (rev_min != rev) {
      rev=rev-200;
      delay(10);
      subir=200;
    }
    if (rev == 1000){
      roto = 0;
    }
  }
  
  if (rev < rev_min) {
    rev=rev_min;
  }
  if (rev > rev_max) {
    rev=rev_max;
    marcha = 10;
    roto = 1;
  }
  
  // Para encender las luces acorde a la revolución
  if (on == 1) {
  int rev_diff=(rev_max-rev_min)/15;
  if (rev != rev_old) 
    for(int i=0; i<15;i++) {
      if ( rev >= rev_min+rev_diff*i) {
         digitalWrite((36-i), 200); 
      } else digitalWrite((36-i), LOW); 
    }
  rev_old=rev; 
  writeArduinoOnMatrix(marcha);
  }

}
