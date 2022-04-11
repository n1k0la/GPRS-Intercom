
/////////////////////////////////////////////////////////////////////////
// SOURCES: 
// Base du micro-code: https://github.com/n1k0la/GPRS-Intercom/tree/v0.0.1
// https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial
// https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
// Bibliothèque GPRS
// Sémentique de version: https://semver.org/lang/fr/#gestion-s%C3%A9mantique-de-version-200
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

int inComing = 0;
char gprsBuffer[140];
int i = 0;
char *s = NULL;

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
GPRS gprs;

void lcdDisplayStatus();
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

void loop() 
{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GPRS- Standby");
      Serial.println("GPRS- Standby");
  
      if(digitalRead(appel_app) == HIGH)
      {
        
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("GPRS- Call");
            delay(1000);
            digitalWrite(relais_1, HIGH);
            Serial.println("Send SMS message.");
            
            gprs.sendSMS(NUMBER,"Mister Bunny, someone at the for you.");
            gprs.callUp(NUMBER);
            Serial.println("Calling ..."); 
      }



      
 if(gprs.serialSIM800.available()) 
 {
        inComing = 1;
 }else
 {
        delay(100);
 }
        if(inComing)
        {
          gprs.readBuffer(gprsBuffer, 140, DEFAULT_TIMEOUT);
          Serial.println(gprsBuffer);
                          
               if(NULL != strstr(gprsBuffer,"NO CARRIER"))
               {
                   Serial.println(gprsBuffer);
                   gprs.hangup();
                   digitalWrite(relais_1, LOW);
                    
                   gprs.cleanBuffer(gprsBuffer,140);  
                   inComing = 0;
                }
        }
}
