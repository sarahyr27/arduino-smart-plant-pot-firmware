//IMPORTAMOS LIBRERÍAS

//librería para el sensor de luz
#include <BH1750.h>
#include <Wire.h>

#include <DHT.h>		// importa la Librerias DHT22, sensor de temperatura y humedad
#include <DHT_U.h>

//SENSORES
BH1750 lightMeter; // Declaramos la variable para el sensor de luz (automáticamente sabrá que los datos entran en el terminal analógico A4, por la librería)
const int humSSensorPin = A1; //para indicar a qué pin analógico está conectado el sensor de humedad del suelo V1.2
const int tempSensorPin = 8; //para indicar a qué pin digital está conectado el sensor de temperatura/humedad DHT
DHT dht(tempSensorPin, DHT22);		// creacion del objeto DHT para la librería

//LED
const int greenLEDPin = 7; //para indicar que el led verde está conectado al puerto digital 7
const int yellowLEDPin = 6; //para indicar que el led amarillo está conectado al puerto digital 6
const int redLEDPin = 5; //para indicar que el led rojo está conectado al puerto digital 5
int LED = 0; //en esta variable cambiaremos el valor para saber quñe PIN encederemos, 1=verde, 2=amarillo, 3=rojo

//VALORES SENSORES
float lux; //para almacenar el valor que leerá el sensor de luz
float hR; //para almacenar el valor de humedad relativa que leerá el sensor DHT
float temp; //para almacenar el valor que leerá el sensor DHT
float humSu, humSuBajo, humSuAlto; //para almacenar el valor que leerá el sensor de humedad del suelo, y convertirlo a porcentaje
int switchState; //para poder encender y apagar los LED


//VARIABLES PARA ALMACENAR PARÁMETROS DE CADA PLANTA
//Humedad relativa mínima y máxima, Temperatura mínima y máxima, Iluminancia mínima y máxima, Humedad del suelo máxima
float hRMin, hRMax, tempMin, tempMax, iluMin, iluMax, humSuMin, humSuMax;
//Para no abusar con el código y poder evaluar estados más facilmente
boolean hROK, tempOK, iluOK, humSuOK, hRSOSO, tempSOSO, iluSOSO, humSuSOSO, hRBAD, tempBAD, iluBAD, humSuBAD;

//VARIABLES PARA STRINGS CON MENSAJES DE LA CONSOLA
String m1, m2, m3, m4, m5, m6;


void setup() {
  Serial.begin(9600); //para establecer conexión entre Arduino y el ordenador, y poder visualizar los valores de la entrada analógica

  //INICIALIZACIÓN DE LOS SENSORES
  //Iluminancia
  Wire.begin(); // Inicializar la interfaz de bus I2C (la librería BH1750 no lo hace automáticamente)
  lightMeter.begin(); //Activar el sensor
  //Temperatura y humedad relativa
  dht.begin();			// inicializacion de sensor
  
  //declaramos las variables de las string
  m1 = "Cantidad de luz: ";
  m2 = " lux";
  m3 = " - Temperatura: ";
  m4 = " ºC - Humedad: ";
  m5 = " %";
  m6 = " - Humedad del suelo: ";
  
  //para establecer los pines conectados a los LED en modo salida y su estado por defecto de forma más rápida
  for(int pinNumber = 5; pinNumber < 8; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

  humSuAlto = 799; //medición más alta tomada con el sensor en el aire
  humSuBajo = 390; //medición más baja tomada con el sensor sumergido en agua salada
  

}

void loop() {

  //OBTENER VALORES
  lux = lightMeter.readLightLevel(); //almacenamos el valor leído por el sensor BH1750 en la variable lux
  temp = dht.readTemperature();	// obtencion de valor de temperatura
  hR = dht.readHumidity();		// obtencion de valor de humedad
  humSu = map(analogRead(humSSensorPin), humSuAlto, humSuBajo, 0, 100); //convertimos los valores obtenidos a porsentajes de humedad
  switchState = digitalRead(2); //para almacenar el valor del voltaje del pin al que está conectado el pulsador, y saber si está presionado o no
  
  //IMPRIMIR VALORES
  Serial.println(m1 + lux + m2 + m3 + temp + m4 + hR + m5 + m6 + humSu + m5);

  paloDeBrasil();
  encenderLED();

  //DELAY PARA ESTABILIDAD DE VALORES
  delay(2000);//retardamos las lecturas para dar estabilidad
}

//HACEMOS FUNCIONES AUXILIARES 

//PARA LA LECTURA DE CADA UNA DE LOS 5 TIPOS DE PLANTAS SELECCIONADOS

void ficus(){
hRMin = 40.0;
hRMax = 70.0;
tempMin = 18.0;
tempMax = 25.0;
iluMin = 25000.0;
iluMax = 35000.0;
humSuMin = 30.0;
humSuMax = 40.0;
}

void potos(){
hRMin = 40.0;
hRMax = 80.0;
tempMin = 18.0;
tempMax = 26.0;
iluMin = 15000;
iluMax = 25000.0;
humSuMax = 40;
}

void monstera(){
hRMin = 50;
hRMax = 60;
tempMin = 10;
tempMax = 26;
iluMin = 10000;
iluMax = 20000;
humSuMax = 40;
}

void orquidea(){
hRMin = 50;
hRMax = 80;
tempMin = 16;
tempMax = 22;
iluMin = 8000;
iluMax = 12000;
humSuMax = 10;


}

void suculenta(){
hRMin = 30;
hRMax = 60;
tempMin = 6;
tempMax = 35;
iluMin = 20000;
iluMax = 50000;
humSuMax = 10;
}

void paloDeBrasil(){
hRMin = 30.0;
hRMax = 80.0;
tempMin = 15.0;
tempMax = 25.0;
iluMin = 400.0;
iluMax = 800.0;
humSuMin = 30.0;
humSuMax = 40.0;
}

//PARA LAS ACCIONES

void encenderLED(){
  //llamamos a las funciones
  evaluarEstado();

    if(switchState == HIGH){
    // Nivel de luz alto
    if (LED == 1) {       
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    } 
    // Nivel de luz medio
    else if (LED == 2) { 
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    } 
    // Nivel de luz bajo
    else if (LED == 3) {  
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
    }
    //LED apagados
  } else {
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
  }


}

// EVALUAR ESTADO DE LA PLANTA
void evaluarEstado() {
  // Estado óptimo
  hROK = (hR >= hRMin && hR <= hRMax);
  iluOK = (lux >= iluMin && lux <= iluMax);
  tempOK = (temp >= tempMin && temp <= tempMax);
  humSuOK = (humSu >= humSuMin && humSu <= humSuMax);

  // Estado regular
  hRSOSO = ((hR >= (hRMin - 10)) && (hR < hRMin)) || ((hR > hRMax) && (hR <= (hRMax + 10)));
  iluSOSO = ((lux >= (iluMin - 5000)) && (lux < iluMin)) || ((lux > iluMax) && (lux <= (iluMax + 5000)));
  tempSOSO = ((temp >= (tempMin - 8)) && (temp < tempMin)) || ((temp > tempMax) && (temp <= (tempMax + 8)));
  humSuSOSO = ((humSu >= (humSuMin - 15)) && (humSu < humSuMin)) || ((humSu > humSuMax) && (humSu <= (humSuMax + 15)));

  // Estado crítico
  hRBAD = (hR < (hRMin - 20) || hR > (hRMax + 20));
  iluBAD = (lux < (iluMin - 10000) || lux > (iluMax + 10000));
  tempBAD = (temp < (tempMin - 15) || temp > (tempMax + 15));
  humSuBAD = (humSu < (humSuMin - 25) || humSu > (humSuMax + 25));

  // Decidir el estado del LED
  if (hROK && iluOK && tempOK && humSuOK) {
    LED = 1; // LED Verde
  } else if (hRSOSO || iluSOSO || tempSOSO || humSuSOSO) {
    LED = 2; // LED Amarillo
  } else if (hRBAD || iluBAD || tempBAD || humSuBAD) {
    LED = 3; // LED Rojo
  }
}
