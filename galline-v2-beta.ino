#include "Servo.h"; //libreria per utilizzare servomotori

//Variabile per gestione del servomotore

Servo motore; //variabile gestione servomotore

/*
 Variabili per la definizione dei pin per interagire con i vari dispositivi elettrici
 */
 
int pin_switch = 3; // Pin 3: modalità automatica/manuale
int pin_manuale = 4;  // Pin 4: gestione apertura/chiusura manuale
int pin_timer = 5;   // Pin 5: gestione timer automatico
int pin_led=13;  // Pin 6: gestione led

//Flag per la gestione degli stati del programma
boolean stato=false; //Indica la modalità del programma => true->cambia stato, false->no change
boolean stato_manuale=false; //Indica lo stato del pulsante manuale
boolean stato_timer=false; //Indica lo stato del timer
boolean aut=true;// true->automatico ; false->manuale
boolean stato_porta=false; //false->chiusa; true->aperta

int pos=240;//posizione motore
const int velocita_step_motore=50;  //velocità del motore degli step del motore
int gradi_motore_chiuso=240;             //posizione motore a porta chiusa
int gradi_motore_aperto=80;           //posizione motore a porta aperta


//Test Mode
boolean test=false;


void setup() {
  //Start dei pin da utilizzare nel programma
  motore.attach(9);
  pinMode(pin_manuale,INPUT);
  pinMode(pin_timer,INPUT);
  pinMode(pin_switch,INPUT);
  pinMode(pin_led,OUTPUT);


//test mode
  if(test==true){
    Serial.begin(9600);
    Serial.print("Test Mode");
    Serial.print("\n");
  }
  chiudi_porta();
}

void loop() {

//Lettura dello stato dello switch
stato=digitalRead(pin_switch);
Controllo_stato_switch(stato);
if(test==true){  
Serial.print("Modalita':");
Serial.print(aut);
Serial.print("\n");
}
if(aut==true)
{
  
stato_timer=digitalRead(pin_timer);
Gestione_timer_automatico();
}else
{
stato_manuale= digitalRead(pin_manuale);
if(stato_manuale==true)
{
  Gestione_cancello_manuale();
}
}
 stato=false;
 delay(1000);
}

//Funzione per controllare l'evento di passaggio tra modalità automatica e manuale

void Controllo_stato_switch(boolean state)
{
  if(state==true)//se è stata cambiata la modalità
  {
   if(aut==false)// se era in manuale passa in automatico
      {
        if(test==true)// stampa in seriale lo stato
        {
        Serial.print("Modalita' automatica");
        Serial.print("\n");
        }
      aut=true;
      digitalWrite(pin_led,LOW);
      }
      else// se era in automatico passa in manuale
      {
      aut=false;
      digitalWrite(pin_led,HIGH);
        Serial.print("Modalita' manuale");
        Serial.print("\n");
      }
  }
}

//Funzione per gestione dell'apertura e chiusura automatica del timer
void Gestione_timer_automatico()
{
  if(test==true)
  {
Serial.print("Stato timer:");
Serial.print(stato_timer);
Serial.print("\n");

Serial.print("Stato porta:");
Serial.print(stato_porta);
Serial.print("\n");
  }
 if(stato_timer==false&&stato_porta==true)
 {
  if(test==true)// stampa in seriale lo stato
        {
        Serial.print("Chiusura automatica x timer");
        Serial.print("\n");
        }
  chiudi_porta();
  stato_porta=false;
 }
 if(test==true){
  Serial.print("Stato timer:");
  Serial.print(stato_timer);
  Serial.print("\n");

  Serial.print("Stato porta:");
  Serial.print(stato_porta);
  Serial.print("\n");
  }
 if(stato_timer==true&&stato_porta==false)
 {
  if(test==true)// stampa in seriale lo stato
        {
        Serial.print("Apertura automatica x timer");
        Serial.print("\n");
        }
  apri_porta();
  stato_porta=true;
 }  
}


//Funzione per apertura e chiusura manuale del cancello
void Gestione_cancello_manuale()
{
    if(stato_porta==false)
  {
    if(test==true)// stampa in seriale lo stato
        {
        Serial.print("Apertura manuale");
        Serial.print("\n");
        }
    apri_porta();
    stato_porta=true;
  }else
  {
    if(test==true)// stampa in seriale lo stato
        {
        Serial.print("Chiusura manuale");
        Serial.print("\n");
        }
    chiudi_porta();
    stato_porta=false;
  }
}
// questa funzione porta il motore a 180 gradi e apre la porta (i gradi sono in una variabile)
void apri_porta() 
{

  for (int i=pos; i > gradi_motore_aperto; i--) {
    pos=i;
    motore.write(pos);
    delay(velocita_step_motore);
  }
}

// questa funzione porta il motore a 0 gradi e chiude la porta (i gradi sono in una variabile)
void chiudi_porta()  
{
 
  for (int i = pos; i <= gradi_motore_chiuso; i++) {
    pos=i;
    motore.write(i);
    delay(velocita_step_motore);
  }
}
