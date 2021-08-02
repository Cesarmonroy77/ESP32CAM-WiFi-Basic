/*
 * Conexión simple a WiFi
 * Por: Cesar Monroy
 * Fecha: 27 de junio de 2021
 *
 * Este programa muestra una conexión básica a WiFi para
 * ESP32CAM. Es necesario que definir el SSID y la
 * contraseña con los datos correspondientes a la red que
 * desea conectar. Se puede observar el estado de la conexión
 * con el monitor serial. No se requieren componentes adicionales.
 *
 * En este programa se usan los leds soldados sobre la placa
 * ESP32CAM, el led flash y el led de status.
 *
 * Componente     PinESP32CAM     Estados lógicos
 * ledStatus------GPIO 33---------On=>LOW, Off=>HIGH
 * ledFlash-------GPIO 4----------On=>HIGH, Off=>LOW
 */

//**************************************
//*********** LIBRERIAS ****************
//**************************************
#include <WiFi.h>  // Biblioteca para manejar el WiFi del ESP32CAM

//**************************************
//*********** GLOBALES   ***************
//**************************************
double timeLast, timeNow; // Variables para el control de tiempo no bloqueante
bool statusLed = 0;// Bandera que me dice si el led de Status esta encendido o apagado

//************************
//****** FUNCIONES *******
//************************
void conectar_wifi(const char*,const char*,int);

// Inicialización del programa
void setup() {
  //**************************************
  //*********** PINES CONFIG *************
  //**************************************
  int statusLedPin = 33;  // Esta variable controla el led de status ubicado en el pin 33
  int flashLedPin = 4; // Esta variable controla el led Flash ubicado en el pin 4
  pinMode (statusLedPin, OUTPUT);// Se configura el pin 33 como salida
  pinMode (flashLedPin, OUTPUT);// Se configura el pin 4 como salida
  digitalWrite (statusLedPin, HIGH);// Se comienza con el led de Status apagado
  digitalWrite (flashLedPin, LOW);// Se comienza con el Flash apagado

  //**************************************
  //*********** WIFICONFIG ***************
  //**************************************
  const char* ssid = "wifiCasa2.4G";  // Poner el nombre de la red a la que se desea conectar
  const char* password = "YoAdopte265/#$eninternet";  // Escribir la contraseña de la red

  conectar_wifi(ssid, password, statusLedPin);  //Función para realizar la conexion
  timeLast = millis (); // Inicia el control de tiempo
}

void loop() {
  double wait = 500;  // Espera de 500 mili segundos para consultar conexión
  int ledFlash = 4; // Esta variable controla el led Flash

  timeNow = millis ();  // Seguimiento de tiempo
  if (timeNow - timeLast > wait) {
    if (statusLed==0) { //Comprobar que el Flash este apagado
      digitalWrite (ledFlash, HIGH);// Encender el flash
      statusLed = 1;//Indicar que el led flash se encuentra encendido
      timeLast = millis (); // Inicia el control de tiempo
    }else{
      digitalWrite (ledFlash, LOW);// Apagar Led el flash
      statusLed = 0;//Indicar que el led flash se encuentra apagado
      timeLast = millis (); // Inicia el control de tiempo
    }
  }
}

void conectar_wifi(const char* ssid,const char* password,int ledStatus){
  Serial.begin (115200);  //Inicialización de comunicación serial a 1152000 baudios
  //Imprimir en el monitor serial
  Serial.print("\n\nConectando a ");
  Serial.println(ssid);

  // Iniciar el WiFi
  WiFi.begin(ssid, password); // Esta es la función que realiz la conexión a WiFi

  while (WiFi.status() != WL_CONNECTED) { // Este bucle espera a que se realice la conexión
    digitalWrite (ledStatus, HIGH); //Se apaga el led de Status
    delay(100); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    digitalWrite (ledStatus, LOW); //Se enciende el led de Status
    Serial.print(".");  // Indicador de progreso
    delay (5);
  }

  // Cuando se haya logrado la conexión, el programa avanzará, Se imprime en el monitor serial
  Serial.println("\nWiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite (ledStatus, LOW);  // Se enciende el led de Status
}
