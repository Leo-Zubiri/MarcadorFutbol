
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,20,4);  //


int inicio = 8;   
int pausa = 9;    
int reset = 10;
   
int minutos = 0;     
int segundos = 0;  
int milisegundos=0;  

int a = 0;
int b = 0;


void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();  
  /*  CHAT
  

  */
  

  // Escribimos el Mensaje en el LCD.
  //lcd.print("Hola Mundo");
}

void loop() {
  timer(45);
}

void timer(short tiempo){
  inicio:  
  if (true)
  {
    a = 1;
    b = 0;
    while (a == 1)
    {
    contador: 
    milisegundos = millis();
    if (milisegundos % 1000 == 0) 
    {
    segundos++;
        if (segundos == 60){
          segundos = 0;
          minutos++;           
        }        
        lcd.setCursor(3, 1);
        if (minutos < 10)      {
          lcd.print("0");      
        }
        lcd.print(minutos);
        lcd.print(":");
        lcd.setCursor(6, 1);
        if (segundos < 10)     {
        lcd.print("0");        }
        lcd.print(segundos);
        
        if (digitalRead(pausa) == false)      {
        delay(200);
          a = 0;
          b = 1;
          while (b == 1)                       {
            if (digitalRead( inicio) == false) {
            goto contador;                     }
            if (digitalRead(reset) == false)   { 
              minutos = 0;
              segundos = 0;
              lcd.clear();
              delay(200);
              goto inicio;    }
          }
        }
        if (digitalRead(reset) == false) 
        { 
          a = 0;                                                 
          minutos = 0;
          segundos = 0;
          lcd.clear();
          delay(200);   
          goto inicio;  
        }
      }
    }
  }
}
