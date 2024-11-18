// Incluyo libreria para conexion I2C
#include <Wire.h>
// Incluyo libreria para moudulo de radio
#include <TEA5767Radio.h>
// Incluyo libreria para LCD
#include <LiquidCrystal_I2C.h>

// Incializo el modulo de Radio como "radio"
TEA5767Radio radio;

// Incializo el modulo LCD como "lcd", en la direccion "0x27"
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inicializo pines del Enconder
#define CLK 5 // CLK ENCODER
#define DT 18 // DT ENCODER
#define SW  19  // SW encoder

// Inicializo las frecuencia maxima y minima de la radio FM
#define minFrequency 87.5
#define maxFrequency 108.0

// Iniciliazo pines para la activacion del modulo Bluetooth y el modulo de Radio
const int KEY_Bluetooth = 27;
const int KEY_Radio = 14;

// Incializo la frecuencia por defecto del modulo de Radio
float frequency = 99.9;

// Bandera para indicar cambio de frecuencia
volatile bool frequencyChanged = false;  

// Inicializamos bandera para activar tanto el modulo de radio o el modulo bluetooth
bool aux=true;

// Variables para el Enconder, que guardaran el estado actual del pin "CLK" y el estado anterior de "CLK"
int lastCLKState, currentCLKState;


// Funcion de interrupcion
// "IRAM_ATTR" se le pone a las interrupciones en el ESP32, con un arduino UNO no haria falta
void IRAM_ATTR adjustFrequency() {
        // Tomo el valor del pin "CLK"
        currentCLKState = digitalRead(CLK);
        // Verifico si el valor actual es diferente al anterior, por lo tanto cambio la posicion del encoder
        if (currentCLKState != lastCLKState) {
          frequencyChanged = true;  // Pongo la bandera en "True", para que luego en el loop se ajuste la frecuencia en el modulo de radio

          // En el caso de que el valor de DT sea desigual al de CLK de incrementa la frecuencia
          if (digitalRead(DT) != currentCLKState) {
              frequency += 0.1;  // Incrementar frecuencia
            
          } else { // En caso contrario, decrementa
              frequency -= 0.1;  // Decrementar frecuencia
          }
          // Decimos que el antiguo valor de CLK sea igual al actual
          lastCLKState = currentCLKState;
        } 
}

void setup() {
    // Incializamos el puerto Serial en 9600 baudios
    Serial.begin(9600);
    // Incializamos la comunicacion I2C
    Wire.begin();

    // Ponemos el pin CLK de Enconder como entrada
    pinMode(CLK, INPUT);
    // Ponemos el pin DT de Enconder como entrada
    pinMode(DT, INPUT);
    // Ponemos el pin DT de Enconder como entrada con el pull-up interno del ESP32. No me acuerdo muy bien por que, pero la datasheet del Enconder dice que hay que 
    // activar pull-up interno del ESP32 para el pin SW del Enconder
    pinMode(SW, INPUT_PULLUP);
    
    // Declaramos estos pines como salidas
    // Nos ayudaran a activar tanto el modulo bluetooth como el modulo de Radio, para que no se pongan simultaneamente
    pinMode(KEY_Bluetooth, OUTPUT);
    pinMode(KEY_Radio, OUTPUT);

    // Tomamos el valor del pin CLK del Enconder
    lastCLKState = digitalRead(CLK);

    // Seteamos una frecuecia por defecto en el modulo de Radio, la 100 (99.9)
    radio.setFrequency(frequency);

    // Inciliazamos el LCD
    lcd.init();
    lcd.backlight(); // Activar luz de fondo del LCD
    // Seteamos el cursor del LCD
    lcd.setCursor(0, 0);
    // Imprimos la frecuencia que estamos escuchando
    lcd.print("Frecuencia: ");
    lcd.setCursor(0, 1);
    lcd.print(frequency);
    lcd.print(" MHz");

    // Por defecto, se activa el modulo bluetooth. Y el modulo de Radio queda desactivado
    digitalWrite(KEY_Bluetooth,aux);
    digitalWrite(KEY_Radio,!aux);

    // Creamos una interrupcion con el pin CLK del enconder, al producirse un cambio de estado en el pin CLK, por eso CHANGE, llamda a "adjustFrequency"
    attachInterrupt(digitalPinToInterrupt(CLK), adjustFrequency, CHANGE);  // Interrupción en CLK 
}

void loop() {
    if (frequencyChanged) {  // Verificar si la frecuencia cambio, por lo tanto entro a la interrupcion y el enconder fue movido
        frequencyChanged = false;  // Resetear la bandera
        // Limitar frecuencia dentro del rango
        if (frequency < minFrequency) frequency = minFrequency;
        if (frequency > maxFrequency) frequency = maxFrequency;

        // Actualizar frecuencia en la radio
        radio.setFrequency(frequency);
        Serial.print("Frecuencia ajustada: ");
        Serial.print(frequency);
        Serial.println(" MHz");

        // Actualizar la frecuencia en el LCD
        lcd.setCursor(0, 1);
        lcd.print("           ");  // Limpiar la línea
        lcd.setCursor(0, 1);
        lcd.print(frequency);
        lcd.print(" MHz");
    }

    // En el caso de que el Encoder sea presionado
    if(digitalRead(SW) == LOW){
        // Esperamos 100ms por el antirebote
        delay(100);
        // En el caso de que sea presionado
        if (digitalRead(SW) == LOW) {  // Verificar si el botón fue presionado
          aux = !aux;  // Cambiar el estado de aux
          // Cambiando el estado de "aux", hacemos que el modulo bluetooth, que por defecto estaba activo, se desactive. Y el modulo de radio se active
          // Y al presionarlo de nuevo volvera a como estaba antes
          digitalWrite(KEY_Bluetooth, aux);
          digitalWrite(KEY_Radio, !aux);
        }
    }
}