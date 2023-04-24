# Código de Calibración de Báscula y Medición de Peso

Este código fue diseñado para programar una báscula electrónica y enviar la medición de peso a un servidor en línea. 


Este proyecto esta basado en su gran mayoria en el proyecto:
https://www.youtube.com/watch?v=LIuf2egMioA&t=43s

A continuación se explica cómo configurar el programa y usar la báscula:

## Configuración Wifi

Antes de poder enviar la medición de peso a un servidor en línea, es necesario conectar la báscula a Wi-Fi. Para hacer esto, siga los siguientes pasos:

1. Abra el código y busque las líneas que dicen "ssid" y "password".

```
char* ssid = "Nombre_WIFI";
char* password = "Clave_WIFI";
```

## Configuración Ubidots

El programa también puede enviar la medición de peso a un servidor en línea a través de MQTT. Para hacer esto, siga los siguientes pasos:

1. Cree una cuenta en un servidor de MQTT en línea como Ubidots o similar.
https://ubidots.com/stem
1.1 presione el botón "Create free Acount"


2. Abra el código y modifique las siguientes líneas
```
const char* mqttServer = "industrial.api.ubidots.com";
int port = 1883;
char* subscribeTopic = "/v1.6/devices/{nombre_device}/{nombre_parametro_a_medir}}";
const char* mqtt_password = "";
const char* mqtt_user = "{token_ubidots}";

```
nombre_device = corresponde al nombre que recibirá el sensor, ejemplo panal1
nombre_parametro_a_medir = que variable va a medir, ejemplo temperatura
token_ubidots = este token se obtiene una vez creada la cuenta

La báscula enviará automáticamente la medición de peso al servidor de MQTT cada segundo.

## Configuración de la Báscula

Antes de usar la báscula, es necesario calibrarla. Esto significa ajustar la lectura de peso para que coincida con el peso real de los objetos que se colocan en ella. Para hacer esto, siga los siguientes pasos:

1. Inicie el programa sin tener ningún objeto encima de la báscula.
2. Una vez que se comiencen a mostrar las mediciones de peso en la pantalla, coloque un objeto con un peso conocido sobre la báscula.
3. Luego, en la terminal serial escriba el factor e ingreselo (no sé como es en windows, en mac es cmd+enter)

Por ejemplo, puede poner un kilo de arroz. Es recomendable usar un peso conocido para poder ajustar el factor de calibración con precisión.
Una vez que la medición coincide con el peso conocido, anote el valor del factor de calibración y lo ingresa en las siguientes líneas de código

```
float factor_calibracion = 25500.0;
```

El factor de calibración es un número que se utiliza para ajustar la lectura de la báscula. Este número puede ser muy diferente para cada báscula y depende de la configuración de la célula de carga y los sensores de carga. Por lo tanto, es importante realizar la calibración con un peso conocido para obtener mediciones precisas.

## Frecuencia de la medición de peso
Para ajustar la frecuencia con la cual se obtiene el muestreo, modifique la línea que se muestra a continuación, considerar que la variable esta en milisegundos
1000ms = 1s
```
int tiempo_monitoreo_peso = 1000;
```

