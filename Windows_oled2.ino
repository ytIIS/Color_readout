// ten kod dziala z wemosem OLED i TCs34725
// led podlaczamy pod D4 - bo to jest pin 2 dla proca w wemos !!
// i2c d1/d2 w wemos
// D0 w wemos to ptzycisk pomiaru - jak masa to mierzy
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TCS34725.h>
#define OLED_RESET 0  // GPIO0
const int ledPin = 2;
int button = 16; // push button is connected
int temp = 0;    // temporary variable for reading the button pin statu
int wygas= 0;   //zmienna wygaszacza
word czas=0;    // zmienna wylaczajaca wygaszacz
Adafruit_SSD1306 OLED(OLED_RESET);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

 
void setup()   {
  Serial.begin(115200);
  if (tcs.begin()) {
    Serial.println("OK sensorr");
  } else {
    Serial.println("No sensor found ... check device");
  while (1);
  }
  
  OLED.begin();
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);

} 
 
void loop() {
uint16_t r, g, b, c, colorTemp, lux;
temp = digitalRead(button);   // kiepskie ale sprawdzenie czy przycisk wycisnieto - uwaga brak redukcji dgran styku !
if (temp == 0) {
    wygas=0;
    czas=0;
    OLED.setTextWrap(false);
    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.stopscroll();
    analogWrite(ledPin, 270);
    delay(3); 
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);
    analogWrite(ledPin, 0);
    // wyslij na rs informacje
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    Serial.print("ColorTemp; "); Serial.print(colorTemp, DEC); Serial.print(";K;");
    Serial.print(";Lux; "); Serial.print(lux, DEC); Serial.print(";-;");
    Serial.print(";R; "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print(";G; "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print(";B; "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print(";C; "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(";");
    OLED.clearDisplay();
    delay(85);
    OLED.setCursor(0,0);
    // ############################################33
    // wyslij do oleda informacje
  //OLED.println(" OLED");
    OLED.print("R ");
    OLED.print(r, DEC);
    OLED.print(" G ");
    OLED.print(g, DEC);
    OLED.print(" B ");
    OLED.print(b, DEC);
    OLED.setCursor(0,10);
    OLED.print("C ");
    OLED.print(c, DEC);
    OLED.print(" Lx ");
    OLED.print(lux, DEC);
    OLED.print(" Ct ");
    OLED.print(colorTemp, DEC);  
    OLED.display(); //output 'display buffer' to screen  
    delay(1500);
}

// jezeli nie aktywny to wyswietla waiting 
//
if (temp == 1) {
   if (wygas==0 && temp ==1 ) {
      OLED.setTextWrap(false);
      OLED.setTextSize(2);
      OLED.setTextColor(WHITE);
      OLED.clearDisplay();
      delay(85);
      OLED.setCursor(0,0);
      OLED.println("Waitng");
      OLED.display(); //output 'display buffer' to screen  
      OLED.startscrolldiagright(0x00, 0x0F);//startscrollleft(0x00, 0x0F);
      wygas=1;
      //Serial.println(wygas,DEC);
   }
   czas=czas+1;
   delay(10);
   // ponizszy if wylacza ekran calkowicie po okolo 6 sekundach
   //
   if (wygas==1 && czas>2255) {
    OLED.clearDisplay();
    OLED.display();
   }
}

}
