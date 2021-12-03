/* ASCII    | DATA
 * 48 - 57  | 0 - 9
 * 65 - 90  | A - Z
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
//LiquidCrystal_I2C lcd(0x3F,16,2); 
LiquidCrystal_I2C lcd(0x27,20,4);  //
int VRx = A1;
int VRy = A0;
int SW = 2;
int btn;

short xJoy = 0;
short yJoy = 0;
byte swJoy = 0;
short lcdX = 0; // Coordenada X (columnas) del LCD
short lcdY = 0; // Coordenada Y (filas) del LCD

short len = 9;

short coord[][3] = {
  // 0 → Es valor alphanumerico
  // 1 → Es valor numerico
  {10, 0, 1}, // TIME
  {3, 1, 0},  // 1st Team
  {4, 1, 0},
  {5, 1, 0},
  {14, 1, 0}, // 2nd Team
  {15, 1, 0},
  {16, 1, 0},
  {4, 2, 1},  // Goal 1st Team
  {15, 2, 1}  // Goal 2nd Team
};


short gol [][2] = {
  {2, 3},
  {13, 3}    
};


void setup() {
  Serial.begin(9600); 
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();   

  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  reset();
}

// Podrias dejar el loop VACIO, para solo mostrar el Hola mundo del setup y no se borre
void loop() {
  xJoy = analogRead(VRx);
  yJoy = analogRead(VRy);

  joystickX();
  joystickY();
}

void joystickX(){
  if(xJoy > 900){
    if(lcdX < 19){
      lcdX++;
    }
  }
  else if(xJoy < 300){
    if(lcdX > 0){
      lcdX--;
    }
  }  
}

void joystickY(){
  if(yJoy > 900){
    if(lcdY < 3){
      lcdY++;
    }
  }
  else if(yJoy < 300){
    if(lcdY > 0){
      lcdY--;
    }
  }  
}

void reset(){
   //printLcd(coord[0][0], coord[0][1], "00:00");
   printLcd(6, 0, "1T 00:00");
   printLcd(coord[1][0], coord[1][1], "---");
   printLcd(coord[4][0], coord[4][1], "---");
   printLcd(coord[len-2][0], coord[len-2][1], "0");
   printLcd(coord[len-1][0], coord[len-1][1], "0");
}

// x = columna    y = fila
void printLcd(short x, short y, String msg){
  lcd.setCursor(x, y);
  lcd.print(msg);
}
