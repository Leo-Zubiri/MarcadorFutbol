#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //
int minutos;
int segundos;
String tiempo;

void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("Hola Mundo");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  //lcd.print(millis()/1000);
  //lcd.print(" Segundos");
  minutos = millis()/60000;
  segundos = millis()%60000/1000;
  //lcd.print(minutos);
  //lcd.print(segundos);
  tiempo = String(minutos+":"+segundos);
  lcd.print(tiempo);
}
