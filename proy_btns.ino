/* ASCII    | DATA
 * 48 - 57  | 0 - 9
 * 65 - 90  | A - Z
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
//LiquidCrystal_I2C lcd(0x3F,16,2); 
LiquidCrystal_I2C lcd(0x27,20,4);  //
// Columna y valor
short col = 0;
short value = 0;
// Valores ascii
short A = 65;
short Z = 90;
short cero = 48;
short nueve = 57;

short len_coord = 9;
short coord[][4] = {
  // Columna, Fila, (0 numeros, 1 alpha), Value
  {10, 0, 1, 0}, // TIME
  {3,  1, 0, 0}, // 1st Team
  {4,  1, 0, 0},
  {5,  1, 0, 0},
  {14, 1, 0, 0}, // 2nd Team
  {15, 1, 0, 0},
  {16, 1, 0, 0},
  {4,  2, 1, 0}, // Goal 1st Team
  {15, 2, 1, 0}  // Goal 2nd Team
};

// Valores de tiempo por el cual puede ir cambiando el usuario
short times [] = {2, 3, 5, 10, 15, 20, 25, 30, 35, 40, 45};

// Coordenadas de GOOOL de cada equipo
short goals [][2] = {
  {2, 3},
  {13, 3}    
};

// Areglo de botones {der, izq, arriba, abajo}
int btnsDir [] = {1, 2, 3, 4};

// Tiempo del partido, 0 = 1er tiempo, 1 = 2do tiempo
bit timeMatch = 0;

int i, j;
void setup() {
    Serial.begin(9600); 
    // Inicializar el LCD
    lcd.init();
    
    //Encender la luz de fondo.
    lcd.backlight();     
    reset();  

    // Establecemos los botones en modo de entrada        
    for(i = 0; i < 4; i++){
        pinMode(btnsDir[i], INPUT);
    }
}

// Podrias dejar el loop VACIO, para solo mostrar el Hola mundo del setup y no se borre
void loop() {

}

void click(){
    for(i = 0; i < 4; i++){
        if(digitalRead(btnsDir[i]){
            checkDir(i)
        }
    }
}

void checkDir(int i){
    switch(i){
        // Derecha
        case 0:
            col++;
            checkCol();
        break;

        // Izquierda
        case 1:
            col--;
            checkCol();
        break;

        // Arriba
        case 2:
            value++;
            checkValue();
        break;

        // Abajo
        case 3:
            value--;
            checkValue();
        break;

        default:
        break;
    }
}

void checkCol(){
    if(col >= len_coord){
        col = 0;
    }else if(col < 0){
        col = len_coord-1;
    }
}

void checkValue(){

}

/*
void joystickX(){
  if( > 900){
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
*/

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

void clock(){
    // si es primer tiempo → bitMatch = 0
    // Si es segundo tiempo → bitMatch = 1

    // si el tiempo es de 5 minutos
    // 1er tiempo
    // i = 0 * 5;       i < 5 * (0 + 1);

    // 2do tiempo
    // i = 1 * 5;       i < 5 * (1 + 1);
    // MINUTOS
    for(i = bitMatch * time; i < time*(bitMatch + 1); i++){
        // SEGUNDOS
        for(j = 0; j < 60; j++){
            // PRINT
        }
        // Minutos++
        // PRINT
    }
}
