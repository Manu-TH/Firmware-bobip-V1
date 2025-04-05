#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes; // create RoboEyes instance

unsigned long previousMillis = 0; // Almacena el tiempo de la última actualización
const long interval = 30000; // Intervalo de tiempo en milisegundos (30 segundos)

void setup() {
  Serial.begin(9600);

  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Startup robo eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate

  // Define some automated eyes behaviour
  roboEyes.setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
  roboEyes.setIdleMode(ON, 2, 2); // Start idle animation cycle (eyes looking in random directions) -> turn on/off, set interval between each eye repositioning in full seconds, set range for random time interval variation in full seconds
  
  // Define mood, curiosity and position
  roboEyes.setMood(DEFAULT); // mood expressions, can be TIRED, ANGRY, HAPPY, DEFAULT
  roboEyes.setPosition(DEFAULT); // cardinal directions, can be N, NE, E, SE, S, SW, W, NW, DEFAULT (default = horizontally and vertically centered)
  roboEyes.setCuriosity(ON); // bool on/off -> when turned on, height of the outer eyes increases when moving to the very left or very right

  // Inicializa la semilla aleatoria
  randomSeed(analogRead(0)); // Usa una entrada analógica sin conectar para generar una semilla aleatoria
} // end of setup

void cambiarHumor(int nuevoHumor) {
  switch (nuevoHumor) {
    case 0:
      roboEyes.setMood(TIRED);
      break;
    case 1:
      roboEyes.setMood(ANGRY);
      break;
    case 2:
      roboEyes.setMood(HAPPY);
      break;
    case 3:
      roboEyes.setMood(DEFAULT);
      break;
  }
} 


void imprimirHumor(int humor) {
  Serial.print("Cambiando a modo: ");
    switch (humor) {
      case 0:
        Serial.println("TIRED");
        break;
      case 1:
        Serial.println("ANGRY");
        break;
      case 2:
        Serial.println("HAPPY");
        break;
      case 3:
        Serial.println("DEFAULT");
        break;
    }

}

void loop() {
  int randomMood;
  unsigned long currentMillis = millis(); // Obtiene el tiempo actual
  
  // Verifica si ha pasado el intervalo de tiempo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Guarda el tiempo actual

    // Genera un número aleatorio entre 0 y 3
    randomMood = random(0, 4); // random(min, max) -> genera un número entre min (inclusive) y max (exclusivo)

    // Cambia el estado del humor según el número aleatorio
    cambiarHumor(randomMood);

    // Opcional: Imprime el estado de humor seleccionado en el monitor serial
    imprimirHumor(randomMood);
    
  }

  roboEyes.update(); // Actualiza los ojos
}