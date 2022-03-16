//Now the connections are ready. 
//Please refer to the description of the video
//for the code and live project simulator link!

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(57600);
  lcd.begin(16,2,0x27);
  lcd.backlight();

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) { //tenta começar o rtc
    Serial.println("Couldn't find RTC");
    lcd.print("Não foi encontrado o LCD."); // se vires isto no LCD, verifica as ligações do LCD e tenta novamente.
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) { //verifica se este esta a correr corretamente
    Serial.println("RTC is NOT running, let's set the time!");
    lcd.print("RTC is NOT running!"); // se vires isto no LCD, verifica as ligações do RTC e tenta novamente.

    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
    DateTime now = rtc.now(); // tempo de agora obtido através do RTC
    DateTime limit = DateTime(2029, 7, 21, 1, 46, 0); //data limite
    TimeSpan future =limit-now; //diferença de tempos

    lcd.setCursor(0, 0);
    Serial.print("Current Date: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);

    Serial.print("Limit Date: ");
    Serial.print(limit.year(), DEC);
    Serial.print('/');
    Serial.print(limit.month(), DEC);
    Serial.print('/');
    Serial.print(limit.day(), DEC);
    Serial.print(" ");
    Serial.print(limit.hour(), DEC);
    Serial.print(':');
    Serial.print(limit.minute(), DEC);
    Serial.print(':');
    Serial.print(limit.second(), DEC);
    Serial.println();


    //escrever anos 
    lcd.print(future.days()/365, DEC);
    lcd.print("Y ");
    //escrever os dias (modulo de todos os dias por 365 (um ano tem 365 dias))
    lcd.print(future.days()%365, DEC);
    lcd.print(" DAYS");

    lcd.setCursor(0, 1); //ir para a linha de baixo
    //escrever as horas restantes neste formato "HORAS:MINUTOS:SEGUNDOS"
    lcd.print(future.hours(), DEC); 
    lcd.print(':');
    lcd.print(future.minutes(), DEC);
    lcd.print(':');
    lcd.print(future.seconds(), DEC);
    delay(1000); //espera 1 segundo até voltar a atualizar!
}
