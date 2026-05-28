#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//NOTE L'interrupt pulseIn ne fonctionne pas

// Initialisation de l'écran LCD 20x2 (utilise les broches A4 et A5)
  // LiquidCrystal_I2C lcd(0x27, 20, 2);

// Caractère personnalisé : carré blanc (oeil)
  // byte carreBlanc[8] = {
  //   B11111,
  //   B11111,
  //   B11111,
  //   B11111,
  //   B11111,
  //   B11111,
  //   B11111,
  //   B11111
  // };

//int count=0; // Variable compteur de cycle qui permet de mesurer du temps sans mettre le programme en pause

const int sensorPins[5] = {A0, A1, A2, A3, A6}; // Tableau des broches du capteur
int sensorWeight[5] = {6, 4, 0, -4, -6}; // Tableau des pondértions des capteurs pour le calcul d'erreur
int sensorValue[5], sensorArray[5]; // Tableaux contenant les valeurs exactes lues et l'activation du seuil de détection de ligne
int lsensorArray[5]; // Tableau contenant les derniers capteurs actifs

const int motorG = 11; //broche ENA pour déclencher ou non le moteur de gauche
const int avantG = 3; //lmf
const int arriereG = 2; //lmb
//HIGH sur lmf et LOW sur lmb fait tourner le moteur vers l'avant et l'inverse le fait tourner vers l'arrière

const int motorD = 10; // broche ENA pour déclencher ou non le moteur de droite
const int avantD = 5; //rmf
const int arriereD = 4; //rmb
//HIGH sur rmf et LOW sur rmb fait tourner le moteur vers l'avant et l'inverse le fait tourner vers l'arrière

// const int color = 12; //broche out du capteur de couleur
// const int s0=6,s1=7,s2=8,s3=9; //broches de filtre et de fréquence du capteur de couleur
// int colorcount = 0; //variable utile dans l'interupt pulseIn

// //Interrupt pulseIn
// volatile unsigned long pulseInTimeBegin = micros(); //mesure du temps au déclenchement de la pulsation
// volatile unsigned long pulseInTimeEnd = micros(); //mesure du temps à la fin de la pulsation
// volatile bool newPulseDurationAvailable = false; //booléen pour déterminer s'il faut lancer une mesure de pulsation
// //Interrupt pulseIn

const int thresholdN = 500; //seuil noir
// const int thresholdB = 1; //seuil bleu
// const int thresholdR = 2; //seuil rouge

double error; //errreur dans le PID
int active; //compte le nombre de capteurs de réflexitivité déclenchés

int P, D, I, lp, correc; //Valeurs utilisées pour le PID
const float Kp = 2; //Coefficient de correction proportionnelle => compromis entre bonne réactivité et suivi dans les virages serrés
const float Kd = 20; //Coefficient de correction par variation => très élevé pour que le robot s'arrête parfaitement sur la ligne lorsqu'il tourne
const float Ki = 0; //On n'utilise pas la correction par calcul intégral


int vg, vd; //vitesse moteur de gauche / droite
int v = 255; //vitesse par défaut
int vmax=255; //vitesse maximale
int vmin=-255; //vitesse minimale (négative => rotation vers l'arrière)

int onLine = 1; //booléen actif si un des capteurs est déclenché, faux sinon
// int timer=0; //timer pour la fonctionnalité de demi-tour

void setup() {
  pinMode(avantG, OUTPUT);
  pinMode(arriereG, OUTPUT);
  pinMode(avantD, OUTPUT);
  pinMode(arriereD, OUTPUT);
  
  pinMode(motorD,OUTPUT);
  pinMode(motorG,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);


// Configuration du capteur TCS3200
  // pinMode(color,INPUT);
  // pinMode(s0, OUTPUT);
  // pinMode(s1, OUTPUT);
  // pinMode(s2, OUTPUT);
  // pinMode(s3, OUTPUT);

  // digitalWrite(s0, HIGH); // fréquence 100%
  // digitalWrite(s1, HIGH);

  // digitalWrite(s2, LOW);  // filtre rouge
  // digitalWrite(s3, LOW);

//Interrupt pulseIn
  // attachInterrupt(digitalPinToInterrupt(color),colorPinInterrupt,CHANGE);
  // Timer1.attachInterrupt(colorPinInterrupt);
  // attachInterrupt(0, colorCount, RISING); 

//Initialisation LCD et génération d'un caractère personnalisé (carré blanc)
  // lcd.init();
  // lcd.backlight();
  // lcd.createChar(0, carreBlanc); // Carré blanc = slot 0

  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  for (int i = 0; i < 5; i++) { // on effectue une première lecture des capteurs pour initialiser lsensorArray
    sensorValue[i] = constrain(analogRead(sensorPins[i]), 0, 1000);
    lsensorArray[i] = noir(sensorValue[i]);
  
  }
  //delay(5000);
}

// void colorCount(){
//   colorcount++;
// }



void loop() {
  // //Interruption pulseIn
  // if (newPulseDurationAvailable) {
  //   newPulseDurationAvailable = false;
  //   unsigned long pulseDuration = pulseInTimeEnd - pulseInTimeBegin;
  //   Serial.println(pulseDuration);
  // }
  // //Interruption pulseIn

  //face(); //fonction qui gère l'affichage des différentes expressions pour le LCD

  readLine(); //fonction qui effectue les lectures du module de suivi de ligne

  //Ce programme résulte de multiples changement effectués dans la panique suite à un dysfonctionnement du robot
  //qui était en fait lié à des soucis matériels

  if (onLine == 1) {  //PID suivi de ligne si on est sur la ligne
    linefollow();
    digitalWrite(LED_BUILTIN,HIGH);
    // timer=0; //on réinitialise le timer de demi-tour
  }
  else  if(lsensorArray[0] || lsensorArray[4]){ //si le dernier capteur est un des capteurs d'extrémité alors on effectue un virage serré
    digitalWrite(LED_BUILTIN,LOW);
    if (error > 0) { // l'erreur nous permet de déterminer de quel côté il faut tourner
      ctrl_motor(-130,v);
    } else {
      ctrl_motor(v,-130);
    }
  }
  // else if(timer==60){ fonction de demi-tour
  //   ctrl_motor(-v,v);
  //   delay(520);
  //   ctrl_motor(245,255);
  //   error=0;
  //   timer=0;
  //   while(!onLine){readLine();}
  // }
  // else{
  //  timer++;
  }
}

void linefollow() {
  error = 0;
  active = 0;
  for (int i = 0; i < 5; i++) { //calcul de l'erreur comme la moyenne pondérée des valeurs des capteurs
    error += sensorWeight[i] * sensorArray[i] * sensorValue[i];
    active += sensorArray[i];
  }
  error = error / active;

  P = error;
  I = I + error;
  D = error - lp;
  lp = P;

  correc = (Kp * P) + (Ki * I) + (Kd * D); // calcul de la correction

  calc_turn(); //lancement de la rotation
}

void calc_turn()
{
  vd = v + correc;
  vg = v - correc; //moteur G tourne plus vite, il me semble qu'on avait une manière de le compenser en utilisant une valeur fixe de +-10 mais elle a été enlevée dans la panique
  if (vd > vmax) 
    vd = vmax;
  if (vg > vmax) 
    vg = vmax;
  if (vd < vmin) 
    vd = vmin;
  if (vg < vmin) 
    vg = vmin;
 ctrl_motor(vg,vd);  
}

void readLine() { //on lit la valeur de chaque capteur et on met à jour les tableaux sensorValue et sensorArray. On définit si on est sur la ligne et on augmente la correction si ce n'est pas le cas
  onLine = 0;
  for (int i = 0; i < 5; i++) {
    sensorValue[i] = constrain(analogRead(sensorPins[i]), 0, 1000);
    sensorArray[i] = noir(sensorValue[i]);
    if (sensorArray[i]){
      onLine = 1;
      for(int j = 0;j<5; j++){
        lsensorArray[j]=noir(constrain(analogRead(sensorPins[j]), 0, 1000));
      }
    }
  }
  if (!onLine){
    if (correc>0 && correc<=120) correc++;
    else if(correc<0 && correc>=-120) correc--;
  }
}

void motor(int motor, int pwr){ //fonction de controle moteur en utilisant un couple(moteur,puissance) la puissance est comprise entre 0 et 255 dans le positif et dans le négatif (sens de rotation inverse)
  analogWrite(motor, 255);
  if(pwr<0){
    if(motor==motorG){
      digitalWrite(avantG, LOW);
      analogWrite(arriereG, abs(pwr));
    }
    else{
      digitalWrite(avantD, LOW);
      analogWrite(arriereD, abs(pwr));
    }
  }
  else{
    if(motor==motorG){
      analogWrite(avantG, pwr);
      digitalWrite(arriereG, LOW);
    }
    else{
      analogWrite(avantD, pwr);
      digitalWrite(arriereD, LOW);
    }
  }
}

void ctrl_motor(int G,int D){ //fonction de controle des deux moteurs en une seule ligne
  motor(motorG,G);
  motor(motorD,D);
}

bool noir(int capteur){ //booléen de détection de noir (appliqué sur une lecture analogue de capteur)
  return capteur<thresholdN; // Nos capteurs renvoient environ 900 lorsqu'il n'ya pas de ligne est environ 20 lorsqu'il y en a une
}



void face(){ // fonction de gestion des expressions du LCD en utlisant la variable count
  if(count==0) gauche();
  else if(count==150) haut();
  else if(count==300) gauche();
  else if(count==450) droit();
  else if(count==600) content();
  else if(count==750) droit();
  else if (count==900) count=0;
  count++;
}

// --- Visage 1 : regarde à gauche ---
void gauche() {
  lcd.clear();
  // Œil gauche
  lcd.setCursor(1, 0); 
  lcd.print("/   |");
  lcd.setCursor(1, 1);
  lcd.print("| ");
  lcd.write(byte(0)); 
  lcd.print(" /");
  // Œil droit
  lcd.setCursor(8, 0); 
  lcd.print("/   |");
  lcd.setCursor(8, 1);
  lcd.print("| ");
  lcd.write(byte(0)); 
  lcd.print(" /");
}

// --- Visage 2 : regarde droit ---
void droit() {
  lcd.clear();
  // Œil gauche
  lcd.setCursor(3, 0);
  lcd.print("|   |");
  lcd.setCursor(3, 1);
  lcd.print("| ");
  lcd.write(byte(0)); 
  lcd.print(" |");
  // Œil droit
  lcd.setCursor(10, 0);
  lcd.print("|   |");
  lcd.setCursor(10, 1);
  lcd.print("| ");
  lcd.write(byte(0)); 
  lcd.print(" |");
}

// --- Visage 3 : dort ---
void dort() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("_____");  // paupière œil gauche
  lcd.setCursor(10, 1);
  lcd.print("_____"); // paupière œil droit
}

// --- Visage 4 : regarde en haut ---
void haut() {
  lcd.clear();
  // Œil gauche
  lcd.setCursor(1, 0); 
  lcd.print("/ ");
  lcd.write(byte(0));
  lcd.print(" |");
  lcd.setCursor(1, 1);
  lcd.print("|   /");
  // Œil droit
  lcd.setCursor(8, 0);
  lcd.print("/ ");
  lcd.write(byte(0));
  lcd.print(" |");
  lcd.setCursor(8, 1);
  lcd.print("|   /");
}

void content(){
  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("|");
  lcd.setCursor(4, 0);
  lcd.print("___"); 
  lcd.setCursor(7,1);
  lcd.print("|"); 
  lcd.setCursor(10,1);
  lcd.print("|");
  lcd.setCursor(11, 0);
  lcd.print("___");  // paupière œil gauche
  lcd.setCursor(14,1);
  lcd.print("|"); // paupière œil droit
}

void face(){
  if(count==0) gauche();
  else if(count==150) haut();
  else if(count==300) gauche();
  else if(count==450) droit();
  else if(count==600) content();
  else if(count==750) droit();
  else if (count==900) count=0;
  count++;
}

// bool bleu(int capteur){
//   return capteur<thresholdB;
// }

// bool rouge(int capteur){
//   return capteur<thresholdR;
// }

// //Interrupt pulseIn fonction
// void colorPinInterrupt()
// {
//   colorcount=0;
//   Timer1.setPeriod(1000000);
//   Serial.println(colorcount);
// }
// //Interrupt pulseIn fonction
