#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Simpletimer.h>
#include <SimpleTimer.h>
 
Simpletimer timer;
//SimpleTimer timer;
 
float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
bool led_ini;
float ph_act;
float ph;
int Temperature;

// for the OLED display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE); 
  pinMode(10, OUTPUT);  //AZUL
  pinMode(11, OUTPUT);  //VERMELHO
  pinMode(12, OUTPUT);  //VERDE
}
void loop() {
  timer.run(1000L); // Initiates SimpleTimer


    digitalWrite(10, LOW);  //AZUL
    delay(100);
    digitalWrite(10, HIGH); //AZUL
    delay(100);
    digitalWrite(10, LOW);  //AZUL


 for(int i=0;i<10;i++) 
 { 
  buffer_arr[i]=analogRead(A0);
  delay(30);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
    if(buffer_arr[i]>buffer_arr[j])
    {
      temp=buffer_arr[i];
      buffer_arr[i]=buffer_arr[j];
      buffer_arr[j]=temp;
    }
  }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
  ph = ph_act;
  Temperature = 22;

  if(ph>=7.00 && 8.00>ph){
    digitalWrite(10, LOW);  //AZUL
    digitalWrite(11, LOW);  //VERMELHO
    digitalWrite(12, HIGH); //VERDE
    delay(100);
  }
  if(ph<7.00 || ph>7.00){
    digitalWrite(10, LOW);  //AZUL
    digitalWrite(11, HIGH); //VERMELHO
    digitalWrite(12, LOW);  //VERDE
    delay(100);
  } 
  if(!ph){
    digitalWrite(10, HIGH); //AZUL
    digitalWrite(11, LOW);  //VERMELHO
    digitalWrite(12, LOW);  //VERDE
    delay(100);
  }
 
 Serial.print("pH Val: ");
 Serial.println(ph);  //monitor e esp8266
 Serial.print("Temp Val: ");
 Serial.println(Temperature); //monitor e esp8266
 display_pHValue();
 delay(1000);
}
 
void display_pHValue()
{
  // Oled display
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0); // column row
  display.print("pH:");
 
  display.setTextSize(2);
  display.setCursor(55, 0);
  display.print(ph);
 

  display.setTextSize(2);
  display.setCursor(0,30);
  display.print("Temp:");
 
  display.setTextSize(2);
  display.setCursor(70, 30);
  display.print(22);
  display.setCursor(95, 50);

 display.display();
}


/*
Conexões no arduino

---=        Display       =---
VCC > 5V
GND > GND1
SLC > A5
SDA > A4

---=       Sensor PH / Módulo PH-4502C      =---
V+ > 5V
G1 > GND1
Po > A0


---=       Sensor temp / Módulo PH-4502C      =---
V+ > 5V
G1 > GND1
Po > A1

*/