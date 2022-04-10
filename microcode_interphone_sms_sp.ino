/////////////////////////////////////////////////////////////////////////
// SOURCES: 
//
// https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial
// https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////
//  Bibliothèque : 
//
///////////////////////////////////////////

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <GPRS_Shield_Arduino.h>

///////////////////////////////////////////
//  Déclarations & prototypes : 
//
///////////////////////////////////////////
#define SRX 2 // broche rx. 
#define STX 3 // broche tx.
#define BAUDERATE 19200
#define NUMERO "+3232497118002"
#define RAPPEL "10:00"


int appel_app = 12;
int etat_appel = 0;
//int led_diag =
//int relais_1 = 13;
 
GPRS gprs(SRX, STX, BAUDERATE);
SoftwareSerial portserie_s8l(3, 2);                                             //SIM800L - arduino - Tx pin3 & Rx pin2.
LiquidCrystal_I2C lcd(0x27, 20, 4);                                             // I2C address 0x27, 16 column and 2 rows
void initialisationSP();
void make_phoneCall();
void status_LCD_display();


////////////////////////////////////////////
//  SETUP
//
////////////////////////////////////////////


void setup()
{
  //////////////////////////////////////////
  // LCD DISPLAY
 lcd.init();                                                                    //initialize the lcd
 lcd.backlight(); //open the backlight 
 lcd.setCursor(0, 0);
 lcd.print("Initialisation:");
 delay(1000);
 lcd.clear();

  //////////////////////////////////////////
  // I/O
 pinMode(appel_app, INPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 //pinMode(led_diag, OUTPUT);
 
  
  //////////////////////////////////////////
  // Port serie
 Serial.begin(19200); //Port série arduino / ordinateur. 
 Serial.println("Go go go");
 
  //////////////////////////////////////////
  // GPRS MOD.
  gprs.checkPowerUp();

  while(!gprs.init()){
    delay(1000);
    Serial.println("(ee) Echec de l'intitialisation !");
  }
  // initialisationSP();
  //portserie_s8l.begin(9600); // Port série Arduino / SIM800L



  
}
//////////////////////////////////////////
// LOOP
//
//////////////////////////////////////////

void loop()
{ 
  char buffer[64]; //Tableau de récuperation date et heure. 
  String dateheure = String(buffer);
  //////////////////////////////
  // Récuperation de de l'heure et de la date. 
  if(!gprs.getDateTime(buffer)){
    Serial.println("(e) Erreur lecture heure");
    // Pourrait utiliser une LED de diagnostique || methode affiche LCD display. 
  } else {
    // Appelle une methode pour signaler l'état. 
    // Affiche moniteur série.
    String dateheure =String(buffer);
    Serial.print(dateheure);
  }
  

  ////////////////////////////////////////
  // Message de service
  String heure = dateheure.substring(9,14);
  
  if(heure.equals(RAPPEL)) {
    while(!gprs.isNetworkRegistered()){
      delay(1000);
      Serial.println("(ww) Pas de connection réseau");
      // Signalisation + méthode problème.
    }
    Serial.println("(ii) Connecté au réseau");
    // Signalisation + méthode problème.

    String message = "Info intercom:"; //+ txttempsc + "...";
    
    Serial.println("(ii) Envoi du message de service.");
    if(gprs.sendSMS(NUMERO, message.c_str())){
      Serial.println("(ii) SMS envoyé.");
    } else {
      Serial.print("(ee) Echec de l'envoi du sms.");
      // Signalisation + méthode problème.
    }
    memset(buffer, 0, sizeof(buffer));
  }
  
 
  if (digitalRead(appel_app) == HIGH)
        {
            make_phoneCall();
        }
        
        else
        {

        }  

}     

///////////////////////////////////////////
//  Définitions de fonctions: 
//
///////////////////////////////////////////
void updateSerial()
{
  delay(500);

  while(portserie_s8l.available()) 
  {
    Serial.write(portserie_s8l.read());//Forward what Software Serial received to Serial Port
  }
}

void initialisationSP()
{
  Serial.println("Initialisation...");
  portserie_s8l.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
 
  portserie_s8l.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
 
  portserie_s8l.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  
  portserie_s8l.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
}

void make_phoneCall()
{

      char hangup [10]= "NO CARRIER";
  portserie_s8l.println("ATD+ +32497118002;");
  do {
    digitalWrite(LED_BUILTIN, HIGH);
    
   
    lcd.setCursor(5,1);
    lcd.print(portserie_s8l.read());
    delay(500);
    lcd.clear();


    //updateSerial();
    //hangup = portserie_s8l.read();
    
  } while (hangup != portserie_s8l.read());
 
  digitalWrite(LED_BUILTIN, LOW);
}
/*
//void status_LCD_display()
{
        lcd.setCursor(0,0);
        lcd.print("Status:");

        lcd.setCursor(0,1);
        lcd.print("Status:");
        
        lcd.setCursor(0,2);
        lcd.print("Status:");

        lcd.setCursor(0,0);
        lcd.print("Status:");
        lcd.setCursor(8,0);
        lcd.print("on air");
            
}*/
