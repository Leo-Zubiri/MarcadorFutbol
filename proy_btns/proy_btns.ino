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
short estado = 0;


/**
 * @param Columna → [][0]
 * @param Fila    → [][1]
 * @param Tipo    → [][2] (0 nums, 1 alpha, 2 time)
 * @param Valor   → [][3]
 */
short coord[][4] = {  
  {9,  0, 2, 0}, // TIME
  {3,  1, 0, 'A'}, // 1st Team
  {4,  1, 0, 'A'},
  {5,  1, 0, 'A'},
  {14, 1, 0, 'A'}, // 2nd Team
  {15, 1, 0, 'A'},
  {16, 1, 0, 'A'}  
};
short len_coord = 7;

/** 
 * @param 0 → IZQ       PIN 2
 * @param 1 → ARRIBA    PIN 3
 * @param 2 → ABAJO     PIN 4
 * @param 3 → DERECHA   PIN 5
 */
int btnsDir [] = {2, 3, 4, 5};
short len_btnsDir = 4;

// Valores de tiempo por el cual puede ir cambiando el usuario
String times [] = {"02", "03", "05", "10", "15", "20", "25", "30", "35", "40", "45"};
short len_times = 11;

//Columnna, fila, valor (goles)
short marcador[][3] = {
  {4,  2, 0},
  {15, 2, 0}
};

// Coordenadas deL mensaje GOOOL (centrado)
//(Columna , Fila)
short goalMsg[] = {7, 3};

// Tiempo del partido, 0 = 1er tiempo, 1 = 2do tiempo
byte timeMatch = 0;

int i, j;
void setup() {
  Serial.begin(9600); 
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();     
  reset();  

  // Establecemos los botones en modo de entrada        
  for(i = 0; i < len_btnsDir; i++){
      pinMode(btnsDir[i], INPUT);
  }
  lcd.setCursor(coord[1][0], coord[1][1]);
}


// Podrias dejar el loop VACIO, para solo mostrar el Hola mundo del setup y no se borre
void loop() {
  if(estado==0){
    parpadeo(true);
    click();
  }
}

bool canClick = true;
short lastPressed = 0;
void click(){
  for(i = 0; i < len_btnsDir; i++){
    if(digitalRead(btnsDir[i]) && canClick){
      canClick = false;
      lastPressed = i;      
      switch (estado)
      {
      case 0:
        checkDir(btnsDir[i]);
        break;

      default:
        break;
      }
      
    }
  }  
  if(digitalRead(btnsDir[lastPressed])== 0) canClick = true;
}

// Verificar que boton fue presionado
void checkDir(int check){
  switch(check){
      // Izquierda
      case 2:
          col--;
          checkCol();
      break;

      // Derecha
      case 5:
          col++;
          checkCol();
      break;

      // Arriba
      case 3:
          coord[col][3]++;
          checkValue();
      break;

      // Abajo
      case 4:
          coord[col][3]--;
          checkValue();
      break;

      default:
      break;
  }
  
}

void checkCol(){
  if(col >= len_coord) col = 0;
  else if(col < 0) col = len_coord-1;
}

void checkValue(){
  // Letra : o Alpha (?)
  if(coord[col][2] == 0){
    if(coord[col][3] > 'Z') coord[col][3] = 'A';
    else if(coord[col][3] < 'A') coord[col][3] = 'Z';
    printLcd(coord[col][0], coord[col][1], String(char(coord[col][3])));
  }

  // Tiempo
  else{
    if(coord[col][3] >= len_times) coord[col][3] = 0;
    else if(col < 0) coord[col][3] = len_coord-1;
    printLcd(coord[col][0], coord[col][1], times(coord[col][3]));
  }    
}


void reset(){
   lcd.clear();
   printLcd(6, 0, "1T 00:00");
   printLcd(coord[1][0], coord[1][1], "AAA");
   printLcd(coord[4][0], coord[4][1], "AAA");
   printLcd(marcador[0][0], marcador[0][1], "0");
   printLcd(marcador[1][0], marcador[1][1], "0");
}

/**
 * @param x → Columna ← →
 * @param y → Fila ↓ ↑
 * @param msg → Mensaje a imprimir
 */
void printLcd(short x, short y, String msg){
  lcd.setCursor(x, y);
  lcd.print(msg);
  lcd.setCursor(x, y);
}

int ms;
void parpadeo(bool prender){
  if(prender){    
    ms = millis();
    ms = ms%1000;
    
    // Turn on the cursor:
    if(ms < 499) lcd.cursor();
    // Turn off the cursor:
    else lcd.noCursor();  
  }
}

  unsigned long Contador = 0;
  void delayMillis(int valor)
  {
  Contador = millis() + valor;
  do {
          
    } while (Contador>=millis());
  }



/*
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
*/
