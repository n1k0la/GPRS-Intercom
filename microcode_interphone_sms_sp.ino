
/////////////////////////////////////////////////////////////////////////
// SOURCES: 
// Base du micro-code: https://github.com/n1k0la/GPRS-Intercom/tree/v0.0.1
// https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial
// https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
// Bibliothèque GPRS
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////
//  Bibliothèque : 
//
///////////////////////////////////////////
#include <gprs.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

///////////////////////////////////////////
//  Déclarations & prototypes : 
//
///////////////////////////////////////////

#define NUMBER "+32497118002"
int appel_app = 12;
int etat_appel = 0;
//int led_diag =
int relais_1 = 13;
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
GPRS gprs;

////////////////////////////////////////////
//  SETUP
//
////////////////////////////////////////////
void setup() {
  
  //////////////////////////////////////////
  // LCD DISPLAY
  lcd.init();                                                                    //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0, 0);
  lcd.print("GPRS-init ...");
  delay(1000);
  lcd.clear();

  //////////////////////////////////////////
  // I/O

  //////////////////////////////////////////
  // Port serie  
  Serial.begin(9600);
  while(!Serial);
  Serial.println("GPRS-init ...");
  gprs.preInit();
  delay(1000);
  while(0 != gprs.init()) {
      delay(1000);
      Serial.print("init error\r\n");

  //////////////////////////////////////////
  // GPRS MOD.
  }  
}

//////////////////////////////////////////
// LOOP
//
//////////////////////////////////////////

void loop() {


  
if(digitalRead(appel_app) == HIGH){
  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GPRS- Call");
      delay(2000);
      digitalWrite(relais_1, HIGH);
      //Serial.println("Start to send SMS message...");
      
      gprs.sendSMS(NUMBER,"Mister Bunny someone at the for you");
      gprs.callUp(NUMBER);
      delay(20000);
      gprs.hangup();
      lcd.clear();
  
  }
 else {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GPRS-Standby");
 }
}

///////////////////////////////////////////
//  Définitions de fonctions: 
//
///////////////////////////////////////////
