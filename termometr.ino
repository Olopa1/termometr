#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //deklarowanie na jakich pinach działa wyświetlacz

const int temp_pin = A0;
const float common_temp = 26.0; // stałe takie jak pin czujnika temperatura do której się odnosze oraz interwał czasu 
const long interval = 1000;

unsigned long prev_milis = 0;
String data = "";               //temperatura pobieranie informacji z programu python 
float actual_temp = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 8; i < 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);                //ustalenie wyjść na pinach 
    digitalWrite(i, LOW);
  }

  lcd.begin(16, 2);
  lcd.print("Pokojowy ");
  lcd.setCursor(0, 1);      //"konfiguracja" wyświetlacza oraz tekst początkowy
  lcd.print("Termometr");
  delay(2000);
  lcd.clear();
}


void loop() {
  unsigned long current_milis = millis();

  if (current_milis - prev_milis >= interval) {  //if który sprawdza interwał czasu
    prev_milis = current_milis;

    if (Serial.available() > 0) {
      while (Serial.available() > 0 ) {
        data = Serial.readString();     //odbieranie danych z programu pythona
      } 
    }
    
    int sensor_value = analogRead(temp_pin);
    float voltage = (sensor_value / 1024.0)  * 5.0 ; //badanie temperatury na sensorze
    actual_temp = (voltage - .5) * 100 ;

    if (actual_temp < actual_temp) {
      digitalWrite(8, HIGH);        //zależnie od tego czy temp jest mniejsza czy większa od zadeklarowanej na początku będzoe palić się konkretna dioda
      digitalWrite(9, LOW);
    }
    else {
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
    }

    float temp_avg ;

    temp_avg = (temp_avg+actual_temp)/60;
    
    /*String st_half = data.substring(0,8) ;
    String nd_half = data.substring(8,22) ;*/
    
    /*Serial.print("Wartosc sensora: ");
    Serial.print(sensorValue);
    Serial.print("  Napiecie: ");
    Serial.print(voltage);  */  
    
    Serial.print("Temperatura C: ");    //dane zapisywnae w monitorze portu szeregowego
    Serial.print(actual_temp);
    Serial.print(" Godzina: ");
    Serial.print(data);
    Serial.print("Godzina inna");
    /*Serial.print(st_half);
    Serial.print(" | ");
    Serial.println(nd_half);*/
    
    
    
    lcd.setCursor(0, 0);
    lcd.print("Godz:");
    lcd.print(data);        //dane wyświetlane na wyświetlaczy
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(actual_temp);
    lcd.print("*C");

  }
}
