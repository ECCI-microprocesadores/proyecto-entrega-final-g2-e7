[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=19677800&assignment_repo_type=AssignmentRepo)
# Proyecto final

## Integrantes
[Francisco Esteban Andrade Garzon](https://github.com/ElectroFran2005)

[Laura Alejandra Cubides Salamanca](https://github.com/LauraCubides)

[Soranji Daniela Laverde Fonseca](https://github.com/Soranji1125)

## Nombre del proyecto:

### Sistema de Detección y Climatización Inteligente

## Documentación

En el diseño de sistemas modernos, los microcontroladores desempeñan un papel central al permitir la interacción entre componentes digitales y el entorno físico mediante la integración de múltiples periféricos internos. El microcontrolador PIC18F45K22, desarrollado por Microchip Technology, destaca por su arquitectura eficiente, su amplio conjunto de registros de propósito específico y la incorporación de módulos funcionales como el Convertidor Analógico-Digital (ADC) y la interfaz I2C. Estos recursos permiten al microcontrolador adquirir datos provenientes del mundo real, procesarlos y generar respuestas automáticas, todo desde un solo chip.

En este proyecto académico se desarrolló un sistema de control automático de ventilación, cuyo objetivo es supervisar el entorno físico mediante sensores y responder en consecuencia. La temperatura ambiente es monitoreada a través del sensor LM35, un dispositivo de precisión que entrega una señal analógica proporcional a la temperatura en grados Celsius (10 mV/°C). Al mismo tiempo, un sensor de movimiento PIR HC-SR501 detecta la presencia de personas u objetos en movimiento. La información proveniente del LM35 es digitalizada por el módulo ADC del PIC18F45K22, mientras que la salida del PIR es leída directamente desde un pin digital. Cuando ambos sensores superan sus respectivos umbrales (específicamente, cuando la temperatura es superior a 30 °C y hay movimiento presente) el sistema activa un ventilador conectado al pin RB2, emitiendo una señal digital de encendido.

Uno de los componentes claves del sistema es la pantalla LCD de 16x2, que se comunica con el microcontrolador mediante el protocolo I2C, lo que reduce la cantidad de pines requeridos y simplifica las conexiones. En dicha pantalla se muestra en tiempo real el valor de la temperatura medida y el estado del ventilador (ON/OFF), proporcionando una interfaz visual clara para el usuario.

Para llevar a cabo este proyecto se utilizaron los siguientes archivos fuente y cabeceras:

- [main_proyecto.c](/src/main_proyecto.c)
- [adc.c](/src/adc.c)
- [adc.h](/src/adc.h)
- [i2c.c](/src/i2c.c)
- [i2c.h](/src/i2c.h)
- [i2c_lcd.c](/src/i2c_lcd.c)
- [i2c_lcd.h](/src/i2c_lcd.h)

### Descripción de las librerías:

#### ADC.C

La librería adc.c implementa el conjunto de funciones necesarias para habilitar y utilizar el módulo ADC (Analog-to-Digital Converter) del microcontrolador PIC18F45K22. Este módulo permite transformar señales analógicas continuas (como las provenientes del sensor de temperatura LM35) en valores digitales que el microcontrolador puede procesar. La resolución del ADC en este microcontrolador es de 10 bits, lo que significa que puede representar 1024 niveles distintos (0–1023), siendo adecuado para medir variaciones pequeñas de voltaje con alta precisión.

La función ```void ADC_Init(void)``` se encarga de configurar e inicializar el módulo ADC del PIC18F45K22, selecciona el canal analógico AN0 (RA0) como entrada por defecto los demás canales como digitales.

La función ```uint16_t ADC_Read(void)``` realiza una lectura completa del canal AN0 (RA0) y devuelve el valor digital resultante de la conversión A/D, se asegura de que el canal AN0 esté seleccionado. Aunque ya fue establecido en ADC_Init, esto garantiza flexibilidad si en el futuro se quisieran leer otros canales. Se activa el bit GO para iniciar la conversión y el microcontrolador entra en espera activa hasta que la conversión termine (cuando GO regrese a 0).
Los 10 bits de resultado están distribuidos en los registros ADRESH (los bits más significativos) y ADRESL (menos significativos), se combinan mediante un desplazamiento y una operación OR para obtener el valor digital final de 10 bits, devuelto como un entero sin signo de 16 bits.

#### I2C.C

La función ```void I2C_init(void)``` se encarga de configurar el módulo I2C del microcontrolador en modo maestro. Para ello, prepara los pines de datos (SDA) y de reloj (SCL) como entradas digitales y desactiva cualquier función analógica que puedan tener. Además, establece la configuración de velocidad y activa el módulo de comunicación serie síncrona (SSP), permitiendo que el microcontrolador inicie y controle las transmisiones por el bus I2C.

La función ```void I2C_start(void)``` genera una condición de inicio en el bus I2C. Esta condición es esencial para comenzar cualquier comunicación, ya que les indica a los dispositivos esclavos conectados que el maestro está por enviar una dirección y posiblemente datos. La función espera a que el hardware confirme que esta condición se ha completado correctamente antes de continuar.

La función ```void I2C_stop(void)``` crea la condición de parada en el bus I2C. Esta condición es necesaria para finalizar la transmisión de datos y liberar el bus, permitiendo que otros dispositivos puedan usarlo. Igual que con la función de inicio, esta rutina espera a que el módulo SSP indique que la operación de detener la comunicación ha concluido.

Por último, la función ```void I2C_write(unsigned char data)``` se utiliza para enviar un byte de datos al dispositivo esclavo una vez que la comunicación ha sido iniciada. El dato se coloca en el registro de transmisión del microcontrolador y se espera a que la transmisión finalice. Esta función es clave para escribir configuraciones o enviar comandos a dispositivos como sensores, pantallas o memorias que utilizan I2C.

#### I2C_LCD.C

La función ```void lcd_init(void)``` inicializa la pantalla LCD. Realiza una serie de comandos necesarios para poner en marcha el modo de operación de 4 bits del display. Estos comandos configuran el número de líneas, el cursor, el tipo de caracteres y limpian la pantalla. Además, incluye retrasos temporales para dar tiempo suficiente a que la pantalla procese cada instrucción. Esta función debe llamarse una sola vez al iniciar el programa para que la pantalla quede lista para mostrar datos.

La función ```void lcd_cmd(unsigned char cmd)``` se encarga de enviar comandos a la LCD. Los comandos permiten controlar el comportamiento interno del display, como mover el cursor, borrar la pantalla, o seleccionar una línea específica. El comando recibido se divide en dos partes: los 4 bits más altos (data_u) y los 4 bits más bajos (data_l), y se transmiten por I2C con las señales de control adecuadas para que la LCD los interprete como instrucciones.

La función ```void lcd_write_char(char c)``` permite mostrar un solo carácter en la pantalla LCD. Funciona de manera similar a lcd_cmd, dividiendo el carácter en dos mitades de 4 bits, pero cambia el bit de control para indicar que lo que se está enviando no es un comando, sino un dato a mostrar. Este método es útil para mostrar letras, números o símbolos uno por uno en cualquier posición del display.

La función ```void lcd_set_cursor(unsigned char row, unsigned char col)``` posiciona el cursor en una ubicación específica del display. Recibe como parámetros la fila (row) y la columna (col) donde se desea que aparezca el siguiente carácter. Internamente, envía un comando a la LCD con la dirección correspondiente al lugar indicado.

La función ```void lcd_write_string(const char *str)``` permite escribir cadenas de texto completas en la pantalla LCD. Utiliza un bucle que recorre cada carácter de la cadena hasta encontrar el final ('\0'), y va llamando a lcd_write_char para escribir carácter por carácter.

Finalmente, la función ```void lcd_clear(void)``` borra completamente el contenido de la pantalla y devuelve el cursor a la posición inicial (fila 0, columna 0). Este proceso también requiere un pequeño retardo después del comando para que la pantalla tenga tiempo de ejecutar la instrucción de limpieza.

#### MAIN_PROYECTO.C

El archivo main_proyecto.c contiene la lógica principal del sistema desarrollado, el cual integra sensores físicos (temperatura y movimiento), una salida de control (ventilador) y una interfaz visual (LCD) utilizando el microcontrolador PIC18F45K22. Este archivo cumple el rol de orquestador del comportamiento del sistema, combinando la lectura de entradas, la toma de decisiones y la actualización visual en tiempo real.

Establece la frecuencia de reloj del sistema en 16 MHz, necesaria para que las funciones de retardo (__delay_ms, __delay_us) funcionen correctamente.

El registro "UMBRAL_ADC" define el valor mínimo del ADC en 61 para activar el ventilador (~30 °C), mientras "VENTILADOR" se define como un alias de la salida digital RB2, facilitando su uso en el código.

La función ```void setup(void)``` configura los periféricos y puertos del microcontrolador. Cumple el rol de inicialización del sistema.

```c
ADC_Init();
I2C_init();
lcd_init();
```

Se inicializan los módulos ADC, I2C y LCD, usando las funciones contenidas en sus respectivas librerías.

Se configura RA0 como entrada analógica (para el sensor LM35) y RA1 como entrada digital (para el sensor PIR), así como, se configura RB2 como salida digital para controlar el ventilador y se asegura que inicie apagado.

La función ```void lcd_write_number(unsigned int number)``` convierte un número entero en una representación en formato decimal con un dígito decimal, adaptado a la temperatura. Se extraen los dígitos individuales de una variable entera para mostrarla como "XX.X" y escribe en la pantalla el número formateado seguido de la unidad de temperatura (°C).

La función principal ```void main(void)```: 

Una vez ejecutada la función setup(), se entra en un bucle infinito while(1) que implementa el comportamiento del sistema en tiempo real, se lee el valor del sensor LM35 a través del ADC y  el estado del sensor PIR desde el pin digital RA1, convierte el valor del ADC a temperatura en grados Celsius. El factor 0.488 ≈ (5000 mV / 1023) es aproximado considerando una alimentación de 5V.

En cuanto a la LCD se posiciona el cursor en la primera línea y escribe el valor de la temperatura, también, se posiciona el cursor en la segunda línea y se escribe el título del estado del ventilador.

Si la temperatura es mayor a 30 °C y el sensor PIR detecta movimiento, el ventilador se activa (RB2 = 1). En caso contrario, se apaga.
Se muestra el estado actual del ventilador en la pantalla (ON u OFF)

#### En el [video](/Imagenes/Funcionamiento.mp4) se puede evidenciar el funcionamiento del proyecto.

## Diagramas

### Montaje del Sistema

![Montaje_del_sistema](/Imagenes/Montaje_del_sistema.jpg)

### Diagrama de Flujo

![Diagrama de Flujo](/Imagenes/proyecto_diagrama.png)

### Diagrama de Conexiones

![Diagrama de Conexiones](/Imagenes/Diagrama_Conexiones.jpg)

Este circuito es un sistema de detección y climatización inteligente basado en el microcontrolador PIC18F45K22, que integra un sensor de temperatura LM35, un sensor de movimiento PIR, un ventilador controlado mediante un transistor NPN (2N2222), y una pantalla LCD con módulo I2C para la visualización de datos. El LM35 entrega una señal analógica proporcional a la temperatura que es leída por el pin RA0 del PIC, mientras que el sensor PIR detecta presencia y envía una señal digital al pin RB0. Cuando el sistema detecta movimiento y la temperatura supera el umbral definido, el PIC activa el transistor desde el pin RC2, permitiendo el paso de corriente al ventilador para refrescar el ambiente.

La pantalla LCD conectada a través del protocolo I2C (líneas SDA y SCL en RC4 y RC3, respectivamente) muestra información en tiempo real como la temperatura ambiente, el estado del ventilador y la detección de movimiento.

## Conclusiones

El desarrollo del sistema de control automático de ventilación permitió aplicar de manera efectiva los recursos del microcontrolador PIC18F45K22, destacando su versatilidad y capacidad de integración de periféricos tanto analógicos como digitales. Uno de los aspectos más relevantes fue la incorporación del protocolo I2C para la comunicación con la pantalla LCD, lo cual simplificó notablemente el cableado al reducir la cantidad de pines utilizados, facilitando así el diseño y montaje del sistema en general. Esta elección también contribuyó a mantener un circuito más ordenado y menos propenso a errores por conexiones incorrectas.

Otro punto crucial en el éxito del proyecto fue la realización de pruebas previas con los sensores involucrados. Verificar con anticipación el comportamiento del sensor de temperatura LM35 y del sensor de movimiento PIR HC-SR501 permitió ajustar adecuadamente los umbrales de activación y asegurar una lectura confiable de datos. Gracias a esta validación, se logró una respuesta precisa y coherente del sistema frente a las condiciones del entorno, cumpliendo con los objetivos planteados.

En conjunto, el proyecto demostró cómo un diseño bien planificado, apoyado en pruebas sistemáticas y en el uso eficiente de interfaces de comunicación como I2C, puede dar lugar a sistemas automatizados robustos y funcionales con aplicaciones prácticas en el monitoreo y control ambiental.

<!-- Crear una carpeta src e incluir en ella los códigos y/o el proyecto de mplab-->
