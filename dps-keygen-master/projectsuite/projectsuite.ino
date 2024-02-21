#include <IRremote.h> // Inclut la bibliothèque IRremote pour utiliser la réception IR
#include <Stepper.h> // Inclut la bibliothèque Stepper pour contrôler les moteurs pas à pas

const int stepsPerRevolution = 2048; // Nombre de pas par tour du moteur pas à pas, à ajuster selon votre moteur

// Définit les broches utilisées pour connecter le moteur pas à pas à l'Arduino
#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6

// Initialise l'objet Stepper avec le nombre de pas et les broches
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

int ledrouge = 8; // Définit la broche pour la LED rouge
int ledverte = 9; // Définit la broche pour la LED verte
// Télécommande
const int RECV_PIN = 11; // Broche de réception pour le récepteur IR
IRrecv irrecv(RECV_PIN); // Crée une instance de IRrecv pour lire les signaux IR
decode_results results; // Stocke les résultats des signaux décodés

void setup() {
  Serial.begin(115200); // Initialise la communication série à 115200 bauds
  pinMode(ledrouge, OUTPUT); // Configure la broche de la LED rouge en sortie
  pinMode(ledverte, OUTPUT); // Configure la broche de la LED verte en sortie
  irrecv.enableIRIn(); // Active la réception IR pour commencer à recevoir des signaux
  
  myStepper.setSpeed(10); // Configure la vitesse du moteur pas à pas en tours par minute (RPM)
}

void loop() {
  digitalWrite(ledverte, HIGH); // Allume la LED verte au début de la boucle
  if (irrecv.decode(&results)) { // Vérifie si un signal IR a été reçu et décodé
    
    if (results.value == 0xFFA857) { // Si le signal correspond au bouton "Haut" de la télécommande
      digitalWrite(ledrouge, HIGH); // Allume la LED rouge
      digitalWrite(ledverte, LOW); // Éteint la LED verte
      myStepper.step(stepsPerRevolution); // Fait tourner le moteur d'un tour dans un sens
      Serial.println("motorUp"); // Envoie un message via la communication série

      digitalWrite(ledrouge, LOW); // Éteint la LED rouge
      digitalWrite(ledverte, HIGH); // Allume la LED verte
    } else if (results.value == 0xFFE01F) { // Si le signal correspond au bouton "Bas" de la télécommande
      digitalWrite(ledrouge, HIGH); // Allume la LED rouge
      digitalWrite(ledverte, LOW); // Éteint la LED verte
      Serial.println("motorDown");
      myStepper.step(-stepsPerRevolution); // Fait tourner le moteur d'un tour dans l'autre sens
      digitalWrite(ledrouge, LOW); // Éteint la LED rouge
      digitalWrite(ledverte, HIGH); // Allume la LED verte
    } else if (results.value == 0xFFC23D) { // Si le signal correspond à un autre bouton spécifique de la télécommande
      Serial.println("sendTelemetry"); // Envoie un message via la communication série pour indiquer une action de télémétrie
    }

    irrecv.resume(); // Prépare le récepteur IR à recevoir le prochain signal
  }
}
