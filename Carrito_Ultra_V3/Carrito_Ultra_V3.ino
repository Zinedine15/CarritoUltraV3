const int motores[] = {16, 17, 18, 19}; // Pines para M1, M2, M3, M4
const int enable[] = {27, 32}; // Pines para ENA, ENB

const int EchoPin = 4;
const int TriggerPin = 5;

// Matriz de movimientos: {M1, M2, M3, M4, ENA, ENB}
const int movimientos[9][6] = {
  {HIGH, LOW, HIGH, LOW, 255, 255}, // Adelante
  {LOW, LOW, HIGH, HIGH, 0, 0},     // Detenerse
  {LOW, HIGH, LOW, HIGH, 255, 255}, // Reversa
  {LOW, LOW, HIGH, LOW, 0, 250},    // Giro Izquierda
  {HIGH, LOW, HIGH, HIGH, 255, 0},  // Giro Derecha
  {HIGH, LOW, HIGH, LOW, 100, 255}, // Curva Izquierda
  {HIGH, LOW, HIGH, LOW, 255, 100}, // Curva Derecha
  {LOW, LOW, LOW, LOW, 0, 0},       // Apagar motores
  {HIGH, HIGH, LOW, LOW, 100, 100}  // Otro movimiento personalizado
};

void setup() {
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Configuración de pines de motores y habilitadores
  for (int i = 0; i < 4; i++) pinMode(motores[i], OUTPUT);
  for (int i = 0; i < 2; i++) pinMode(enable[i], OUTPUT);

  // Inicializar todos los pines a LOW
  for (int i = 0; i < 4; i++) digitalWrite(motores[i], LOW);
  for (int i = 0; i < 2; i++) digitalWrite(enable[i], LOW);
}

void ejecutarMovimiento(int movimiento, int tiempo) {
  // Asignar las señales según el movimiento seleccionado
  for (int i = 0; i < 4; i++) digitalWrite(motores[i], movimientos[movimiento][i]);
  
  // Ajustar la velocidad mediante PWM
  for (int i = 0; i < 2; i++) analogWrite(enable[i], movimientos[movimiento][4 + i]);

  // Esperar el tiempo especificado
  delay(tiempo);
}

int ping(int TriggerPin, int EchoPin) {
unsigned short int duration;
float distanceCm;

digitalWrite(TriggerPin, LOW); //para generar un pulso limpio ponemos a LOW 4us
delayMicroseconds(4);
digitalWrite(TriggerPin, HIGH); //generamos Trigger (disparo) de 10us
delayMicroseconds(10);
digitalWrite(TriggerPin, LOW);

duration = pulseIn(EchoPin, HIGH); //medimos el tiempo entre pulsos, en microsegundos

distanceCm = float(duration)/58.2; //convertimos a distancia, en cm
return distanceCm;
}

void loop() {
  float cm = ping(TriggerPin, EchoPin);

  if (cm <= 50) {
    // Si la distancia es menor o igual a 20 cm, cambiar dirección
    Serial.println("Obstáculo detectado, cambiando dirección");

    ejecutarMovimiento(1, 500); // Detenerse momentáneamente

    // Elegir si girar a la izquierda o a la derecha
    if (random(0, 2) == 0) {
      ejecutarMovimiento(3, 1000); // Giro Izquierda
    } else {
      ejecutarMovimiento(4, 1000); // Giro Derecha
    }
  } else {
    // No hay obstáculo, avanzar hacia adelante
    ejecutarMovimiento(0, 100); // Adelante
  }
}

