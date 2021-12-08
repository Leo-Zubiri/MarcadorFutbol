
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

/*
const int btn_left = 2;
const int btn_up = 3;
const int btn_down = 4;
const int btn_right = 5;
const int extra = 6;
*/

enum boton{arriba = 3, abajo = 4, izquierda = 2, derecha = 5, controlBtn = 6};
int btns[] = {izquierda, arriba, abajo, derecha, controlBtn};
enum estados {editandoEquipos, equiposEditados, primerT, segundoT, Gol, setTExtra,setTExtra2, primerTE, segundoTE, finalizado};

// Joshua<3 <--------8
short edoAnt; //
int i; 
short len_btnsDir = 5;
short lastPressed;
short posLetra = 0; 
char letra;
short timeForEachTime = 3;
unsigned long milisegundos = 0;
short segundos = 0;
short minutos = 0;
bool isRunning = true;

short marcador[][3] = {
  {4,  2, 0},
  {15, 2, 0}
};

short coord[][4] = {  
  {9,  0, 2, 0}, // TIME
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

short gol1=0,gol2=0;
bool canClick = true;
short state = editandoEquipos;

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


void loop() { 
  if (state == editandoEquipos or state == equiposEditados or state == Gol){
    //Edicion Equipos
     parpadeoCursor();
     //parpadeo(true); 
  }
  Serial.println(String("estado: "+state));
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

  // 6, 0, "2"    Para el segundo tiempo extra
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
  if(
    state == primerT  ||
    state == segundoT ||
    //state == primerTE ||
    state == segundoTE 
  ){
    edoAnt = state;
  }
  
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
  case primerT: case segundoT: case segundoTE:
    if(btn==controlBtn){      
      state = Gol;
      lcd.setCursor(6,3);
      lcd.print("Gooool!!");
    }
  break;
  case Gol:
    lcd.cursor();
    //printLcd(marcador[0][0], marcador[0][1], "2");
    lcd.setCursor(marcador[0][0], marcador[0][1]);
    AddGol(btn);
    break;
  case setTExtra:
    tExtraController(btn);
  break;
  case primerTE:
    if(btn==controlBtn){
      segundoTiempo();
    }
  break;

  case setTExtra2:
    tExtraController(btn);
  break;

  // case segundoTE:
  //   if(btn==controlBtn){
  //     state=Gol;
  //     AddGol(-1);
  //   }
  // break;


  //estados {editandoEquipos, equiposEditados, primerT, segundoT, Gol, setTExtra, setTExtra2, primerTE, segundoTE, finalizado};
  default:
    break;
  }
}

short tExtra = 0;
void tExtraController(short dir){
  switch (dir)
  {
  case arriba: //Arriba
    tExtra++;
    printLcd(9, 3, "+");
    printLcd(10, 3, String(tExtra));
    break;
  
  case abajo: //Abajo
    tExtra--;
    if(tExtra < 0) tExtra = 0;
    printLcd(9, 3, "+");
    printLcd(10, 3, String(tExtra));
    break;
  case controlBtn:
    printLcd(8, 3, "       ");    
    lcd.noCursor();
    isRunning = true;
    if(state==setTExtra )primerTExtra();
    else if(state==setTExtra2 )segundoTExtra();
    //if(state == segundoT) segundoTExtra();   
  break;
  }
  
}

bool eq1 = true;
void AddGol(short dir){
  switch (dir)
  {
  case izquierda: //Izquierda
    lcd.cursor();
    eq1 = !eq1;
    if(eq1){
      lcd.setCursor(marcador[0][0], marcador[0][1]);
    } else {
      lcd.setCursor(marcador[1][0], marcador[1][1]);
    }
    break;

  case arriba: //Arriba
    if(eq1) {
      gol1++;
      printLcd(marcador[0][0], marcador[0][1],String(gol1));
    } else {
      gol2++;
      printLcd(marcador[1][0], marcador[1][1],String(gol2));
    }
    
    break;
  
  case abajo: //Abajo
    if(eq1){
      gol1--; if(gol1<0) gol1=0;
      printLcd(marcador[0][0], marcador[0][1],String(gol1));
    }  else {
      gol2--;
      if(gol2<0) gol2=0;
      printLcd(marcador[1][0], marcador[1][1],String(gol2));
    } 
    break;

  case derecha:
    lcd.cursor();
    eq1 = !eq1;
    if(eq1){
      lcd.setCursor(marcador[0][0], marcador[0][1]);
    } else {
      lcd.setCursor(marcador[1][0], marcador[1][1]);
    }
  break;
  
  case controlBtn:
    state = edoAnt; //Se termina el estado de edicion
    isRunning=true;
    //state = primerT;
    lcd.noCursor(); 
    lcd.setCursor(6,3);
    lcd.print("         ");
    break;
  }

  
  //
  //printLcd(marcador[1][0], marcador[1][1], "0");
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
  state=primerT;
  lcd.noCursor();
  timer(0,timeForEachTime);
}

void segundoTiempo(){
  state = segundoT;
  printLcd(6, 0, "2T");
  printLcd(2, 3, "                ");
  lcd.noCursor();
  isRunning=true;
  timer(minutos - tExtra,timeForEachTime);
  //timer(timeForEachTime, timeForEachTime );
  tExtra=0;  
}

void primerTExtra(){    
  timer(minutos,tExtra);  
  printLcd(5, 3, "Iniciar 2T");
  lcd.cursor();
  state = primerTE;
}

void segundoTExtra(){  
  state = segundoTE;
  timer(minutos,tExtra);
  //printLcd(6, 0, "2T");
  finPartido();
}



void timer(short comienzo , short mints){
  minutos=comienzo;
  mints+=comienzo;

  marcando:
    click();

  inicio:   
    while(isRunning){
      if(state==Gol){
        lcd.cursor();
        goto marcando;
      }
      milisegundos = millis();
    if (milisegundos % 100 == 0)  // 1000
    {
    segundos++;
        if (segundos == 6){   // 60
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

          checkTimes();

          // if(state==primerTE){
          //   state=segundoT;
          // }else if(state==primerT){
          //   state = setTExtra;          
          //   tExtraController(abajo);
          // } else if (state==segundoT) {
          //   state = setTExtra2;          
          //   tExtraController(abajo);
          // } 
          
          //goto inicio;
        }
        //estados {editandoEquipos, equiposEditados, primerT, segundoT, Gol, setTExtra, setTExtra2, primerTE, segundoTE, finalizado};
      }
      click();
      //lcd.setCursor(marcador[0][0], marcador[0][1]);
    }
}


void checkTimes(){
  switch(state){
    case primerT:
      state = setTExtra;
      tExtraController(abajo);
    break;
    
    case primerTE:
      state=segundoT;
    break;

    case segundoT:
      state = setTExtra2;
      tExtraController(abajo);
    break;
    
    // case segundoTE:      
    //   finPartido();
    // break;
  }
}

void finPartido(){
  state = finalizado;
  String equipo1 = getStr(equipos[0][2]) + getStr(equipos[1][2]) + getStr(equipos[2][2]);
  String equipo2 = getStr(equipos[3][2]) + getStr(equipos[4][2]) + getStr(equipos[5][2]);
  String mensajeFinal;
  if(gol1 > gol2){
    mensajeFinal = equipo1 + " GANO";
    printLcd(6, 3, mensajeFinal);
  }
  else if(gol2 > gol1){
    mensajeFinal = equipo2 + " GANO";
    printLcd(6, 3, mensajeFinal);
  }
  else{
    mensajeFinal = "EMPATE";
    printLcd(7, 3, mensajeFinal);
  }  
}

String getStr(short a){
  return String(char(a));
}
