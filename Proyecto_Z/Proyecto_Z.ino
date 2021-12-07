
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

/*
const int btn_left = 2;
const int btn_up = 3;
const int btn_down = 4;
const int btn_right = 5;
const int extra = 6;
*/
enum boton{arriba=3,abajo=4,izquierda=2,derecha=5,controlBtn=6};
int btns[] = {izquierda, arriba, abajo, derecha, controlBtn};
enum estados {editandoEquipos,equiposEditados,primerT};

int i; //
short len_btnsDir = 5;
short lastPressed;
short posLetra = 0; 
char letra;
byte timeMatch = 90;
byte middleTime = 45;
unsigned long milisegundos = 0;
short segundos=0, minutos=0;

short marcador[][3] = {
  {4,  2, 0},
  {15, 2, 0}
};

short coord[][4] = {  
  {9, 0, 2, 0}, // TIME
  {3,  1, 0, 0}, // 1st Team
  {4,  1, 0, 0},
  {5,  1, 0, 0},
  {14, 1, 0, 0}, // 2nd Team
  {15, 1, 0, 0},
  {16, 1, 0, 0}  
};

// Columna, Fila, Valor 
// Primera A → COLUMNA [0][0]   FILA[0][1]
// Segunda A → COLUMNA [1][0]   FILA[1][1]
// Tercera A → COLUMNA [2][0]   FILA[2][1]
// ...
short equipos[][3]={
  {3,  1, 'A'}, // 1st Team 0,0
  {4,  1, 'A'}, //0,1
  {5,  1, 'A'},
  {14, 1, 'A'}, // 2nd Team
  {15, 1, 'A'},
  {16, 1, 'A'}  
};



void setup() {
  Serial.begin(9600);
  for(i = 0; i < len_btnsDir; i++){
      pinMode(btns[i], INPUT);
  }
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();     
  reset();  
}

bool canClick = true;
short state = editandoEquipos;


/*
  States 
  0: Editando nombre equipos
*/

void loop() {
  if (state==0){
    //Edicion Equipos
     parpadeoCursor();
     //parpadeo(true); 
  } else if(state==1){
    //Empezar timer 1er tiempo
    parpadeoCursor();
  }
  click();
}

//Metodos de comportamiento ----------------------------------
void reset(){
  lcd.clear();
  printLcd(6, 0, "1T 00:00");
  printLcd(coord[1][0], coord[1][1], "AAA");
  printLcd(coord[4][0], coord[4][1], "AAA");
  printLcd(marcador[0][0], marcador[0][1], "0");
  printLcd(marcador[1][0], marcador[1][1], "0");
  lcd.setCursor(coord[1][0], coord[1][1]);
}

void printLcd(short x, short y, String msg){
  lcd.setCursor(x, y);
  lcd.print(msg);
  lcd.setCursor(x, y);
}

void click(){
  for(i = 0; i < len_btnsDir; i++){
     if(digitalRead(btns[i]) && canClick){
        Serial.println(i);
        lastPressed = i;
        canClick = false;
        controller(btns[i]);
     }     
  }

  if(digitalRead(btns[lastPressed])== 0) canClick = true;
}

void controller(short btn){
  switch (state)
  {
  case editandoEquipos:
    editTeamName(btn);
    break;
  case equiposEditados:
    if(btn==controlBtn) {
      lcd.setCursor(6,3);
      lcd.print("        ");
      primerTiempo();
    }
    break;
  
  default:
    break;
  }
}
unsigned long Contador = 0;
void delayMillis(int valor)
{
  Contador = millis() + valor;
  do {
    click();
  } while (Contador>=millis());
}

void parpadeoCursor(){
  lcd.cursor();
  delayMillis(500);
  lcd.noCursor();
  delayMillis(500);
}

void editTeamName(short dir){
  switch (dir)
  {
  case izquierda: //Izquierda
    posLetra--;
    if(posLetra<0) posLetra=5;
    lcd.setCursor(equipos[posLetra][0], equipos[posLetra][1]);
    break;

  case arriba: //Arriba
    equipos[posLetra][2]++;  
    if(equipos[posLetra][2]>'Z') equipos[posLetra][2] = 'A';    
    printLcd(equipos[posLetra][0], equipos[posLetra][1], String(char(equipos[posLetra][2])));
    break;
  
  case abajo: //Abajo
    equipos[posLetra][2]--;  
    if(equipos[posLetra][2]<'A') equipos[posLetra][2] = 'Z';
    printLcd(equipos[posLetra][0], equipos[posLetra][1], String(char(equipos[posLetra][2])));
    break;

  case derecha: //Derecha
    posLetra++;
    if(posLetra>5) posLetra=0;
    lcd.setCursor(equipos[posLetra][0], equipos[posLetra][1]);
    break;
  
  case controlBtn:
    state = equiposEditados; //Se termina el estado de edicion
    lcd.noCursor(); 
    lcd.setCursor(6,3);
    lcd.print("Iniciar");
    break;
  }
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

void primerTiempo(){
  timer(1);
}

bool isRunning = true;
void timer(short mints){
  inicio:   
    while(isRunning){
      milisegundos = millis();
    if (milisegundos % 1000 == 0) 
    {
    segundos++;
        if (segundos == 60){
          segundos = 0;
          minutos++;           
        }        
        lcd.setCursor(9, 0);
        if (minutos < 10)      {
          lcd.print("0");      
        }
        lcd.print(minutos);
        lcd.print(":");
        //lcd.setCursor(6, 1);
        if (segundos < 10)     {
        lcd.print("0");        }
        lcd.print(segundos);
        
        if (minutos == mints)      {
          delay(200);
          //if(digitalRead()) 
          isRunning = false;
          //goto inicio;
        }
      }
    }
}
