/* Programa: PuertoSerie_USART_PC.c
 * Creador : dmm00093 (David MM)
 *
 * Función : El programa activa GPIO necesarios para transmisión USART entre el STM32 y el PC mediante
 * puerto serie. Se usan registros (contenidos en regs_cfg.h) para habilitar el control de reloj RCC,
 * habilitar el GPIO vinculado al USART (USART2 para comunicaciones usando STLINK) y para habilitar el
 * USART en sí.
 *
 * Además, se crean funciones (contenidas más abajo) destinadas al procesamiento (escritura y lectura)
 * de datos (por ahora, caracteres o strings de caracteres) usando USART y su Data Register (USART_DR).
 *
 * Para comunicarnos usando el puerto serie, debemos usar el programa "PuertoSerie_Comm.c" de forma
 * paralela en nuestro ordenador. Además, este programa debe estar cargado en el STM32 y, para que este
 * funcione de forma correcta, cualquier programa que use o acceda al COM habilitado por ST_LINK debe de
 * cerrarse, de forma contraria, el programa no funciona (excepto PuertoSerie_Comm.c (se abre en CMD)).
 *
 * El programa está ampliamente comentado para que se pueda entender.
 */

#include <stdint.h>
#include <stdio.h>
#include "regs_cfg_volatile.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


// ------------------------------------------------------------------------------------------
//										   FUNCIONES
// ------------------------------------------------------------------------------------------

void BlinkCustomizable (int reps) {

	for (int i = 0 ; i <= reps ; i++) {

		for (volatile int j = 0 ; j < 50000 ; j++)

		GPIOA->GPIO_BSRR.REG = (1 << 5);

		for (volatile int j = 0 ; j < 50000 ; j++)

		GPIOA->GPIO_BSRR.REG = (1 << 21);
	}

}

// -------------------
// Comunicacion USART:
// -------------------

void USART2_EscribirChar (char c) {

	// Esperamos a que el Data Register (DR) esté vacío (TXE->0).

	while (USART2->USART_SR.BITS.TXE == 0);

    // El bit TXE a 0 indica que el hardware ya movió el byte anterior al Shift Register. Es decir:
	// El registro de datos USART_DR está vacío y podemos mandarle un bit de información.

	USART2->USART_DR.REG = c;

	// USART_DR solo puede almacenar un byte. El programa simplemente trocea uno a uno y los manda.
	// Aqui lo cargamos en DR y lo pasará al registro de desplazamiento para enviarlo bit a bit.

}

void USART2_EscribirString (char *str) { // Se remite al comentario en USART2_EscribirChar.

	for (int i = 0; str [i] != '\0'; i++){ // Recorremos el array y lo envía cada caracter uno por uno.

		USART2_EscribirChar (str[i]);
	}
}

char USART2_LeerChar (void) { // Leemos un caracter. Devuelve un void.

	while (USART2->USART_SR.BITS.RXNE == 0); // 0: Data is not received.
											 // 1: Received data is ready to be read.

	// En este while, comprobamos si puede recibir datos en RXNE. Si puede (LOW) devolvemos el char.

	return (char)(USART2->USART_DR.REG);
}

void USART2_LeerString (char *buf, int cap) { // Funcion para leer un string con buffer.
											  // Dirigida para comunicacion PuertoSerie.

	int i = 0; char letra;

	int contadorBarraN = 0; // Nuestro mensaje tendrá dos \n.

	for (i = 0 ; i < (cap - 1) ; i++) { // Recorremos mensaje desde 0 hasta la capacidad_max - 1

		letra = USART2_LeerChar();

		if (letra == '\r' || letra == '\0') {

			break; // Si llegamos a alguna de estas cosas hemos terminado de leer, logicamente.

		}

		buf [i] = letra; // Si no, le asignamos que el buffer en i sea esa letra.

		if (letra == '\n') {  // Si vemos un \n lo contamos
			contadorBarraN = contadorBarraN + 1;
		}

		if (contadorBarraN == 2) { // Si ya hay dos, avanzamos i para no joderla y break.
			i++;
			break;

			// Llegamos a la posicion 11 del array, hay un \n, cortamos y metemos en la 12 un \0.
		}
	}

	buf [i] = '\0'; // Fin de cadena donde terminó el texto, ponemos \0 para finalizar el mensaje.

	// Por ejemplo si leemos Miau : ['M','i','a','u','\0'].
	// i < (cap - 1) -> aseguramos que haya hueco sin salirnos del array.
}

// ------------------------------------------------------------------------------------------
//										    MAIN
// ------------------------------------------------------------------------------------------

int main(void)
{

	// Consultar la pagina 146 del manual.

	// Debemos de consultar el registro APB2ENR.
	// LED verde de usuario -> STM32 I/O PA5, contenido en APB2ENR.

	// 1. Activaremos el registro IOPAEN, ya que es el IO port A para el LED y USART.

	RCC->APB2ENR.BITS.IOPAEN = 1;

	// El PA5 es para el LED interno verde. Debemos configurar el pin 5 como una salida a 2 MHz.
	// Para esto usamos el registro CRL o CRH, segun el numero de pin. Consultar manual.

	GPIOA->GPIO_CRL.BITS.MODE5 = 2; // Modo Output: Salida a 50 MHz.

	// 00: Input mode (reset state)
	// 01: Output mode, max speed 10 MHz.
	// 10: Output mode, max speed 2 MHz.
	// 11: Output mode, max speed 50 MHz.

	GPIOA->GPIO_CRL.BITS.CNF5 = 0; // Salida push-pull GPIO.

	// In output mode (MODE[1:0] > 00):
	// 00: General purpose output push-pull
	// 01: GPO Open Drain
	// 10: Alternate fuction output push-pull
	// 11: AFO Open Drain

	// ---------------------------------------------
	// 2. Habilitamos USART2 (RCC -> GPIO -> USART).
	// ---------------------------------------------

	// USART2EN  -> APB1ENR.
	// USART2_TX -> PA2.
	// USART2_RX -> PA3.

	RCC->APB1ENR.BITS.USART2EN = 1; // USART1_EN, CLK.
	RCC->APB2ENR.BITS.AFIOEN   = 1; // Reloj AFIO habilitado, alternate function para USART.

	// USART1_TX.
	// USART1_TX solo obedece a USART, no obedece a BSRR.

	GPIOA->GPIO_CRL.BITS.CNF2  = 2; // Alt.Func. PushPull.
	GPIOA->GPIO_CRL.BITS.MODE2 = 3; // 50 MHz.

	// USART1_RX.

	GPIOA->GPIO_CRL.BITS.CNF3 = 1;  // Input floating.
	GPIOA->GPIO_CRL.BITS.MODE3 = 0; // Es una entrada.

	// ---------------------------------------------------------------------------------------
	// 3. Configuracion baudios en frecuencia. (reloj interno PCLK2, pag 799) (Pag 92 es clocks).
	//	  Por defecto, se inicializa a 8 MHz el HSI interno, pero puedo configurarlo a 72 MHz.
	// ---------------------------------------------------------------------------------------

	// Calculo de mantisa y fraccion:

	// USARTDIV = fclk / 16 * Baudrate
	// Frac = DIV_Decimal * 16

	USART2->USART_BRR.BITS.DIV_Mantissa = 52; // Calculo usando CLK por defecto HSI de 8 MHz.
	USART2->USART_BRR.BITS.DIV_Fraction = 1;

	// --------------------------------------------------------
	// 4. Habilitación de USART (TE, RE, UE en Control Register 1)
	// --------------------------------------------------------

	USART2->USART_CR1.BITS.UE = 1; // USART Enable.
	USART2->USART_CR1.BITS.TE = 1; // Transmitter Enable (Transmitter es TX, TE es su enable)
	USART2->USART_CR1.BITS.RE = 1; // Receiver Enable (Receiver es RX, RE es su enable)

	// --------------------------------------------------------------------------------------
	// 										  LOOP
	// --------------------------------------------------------------------------------------

	while (1) {

		int op;

		op = 3;

		/*if (op == 0) { // Prueba de mandar texto continuamente a la consola, funciones con TXE.
					   // Estará constantemente mandando texto, parpadeando el LED (comprobacion).

			// Encendemos LED
			GPIOA->GPIO_BSRR.REG = (1 << 5);

			// Enviamos un texto completo
			USART2_EscribirString("Hola desde mi STM32!\r\n");

			// Retardo grande para que nos de tiempo a verlo y no sature la pantalla
			for (volatile int i = 0 ; i < 1000000 ; i++);

			// Apagamos LED
			GPIOA->GPIO_BSRR.REG = (1 << 21);

			// Otro retardo
			for (volatile int i = 0 ; i < 1000000 ; i++);

		}

		if (op == 1) { // Prueba transmision y recepcion con funciones que tocan TXE y RXNE
					   // El LED va a parpadear cada vez que nosotros presionemos una tecla.

			char c;

			for (volatile int i = 0 ; i < 100000 ; i++); // Retardo de encender LED.

			// USART2_EscribirChar ('A');

			GPIOA->GPIO_BSRR.REG = (1 << 5);

			c = USART2_LeerChar (); // Programa espera a que teclees una letra, luego, la lee.
									// Bloquea programa hasta que RXNE sea 1 (dato listo).

			USART2_EscribirChar (c); // Ahora, el programa va a mostrar esa letra en la consola.

			for (volatile int i = 0 ; i < 100000 ; i++); // Retardo para ver que se realizó.

			GPIOA->GPIO_BSRR.REG = (1 << 21); // Apagamos led.

		}*/


		if (op == 2) { // Prueba de transmisión PuertoSerie con PC usando COM de STLINK.

			for (volatile int i = 0 ; i < 100000 ; i++); // Delay. Iniciamos LED para decir que empezamos.

			GPIOA -> GPIO_BSRR.REG = (1 << 5); // LED on.

			char string [100];

			USART2_LeerString (string, 100); // Leemos el string que nos ha enviado el PC.

			BlinkCustomizable(3); // Parpadeamos 3 veces para indicar que nos ha llegado el mensaje.

			USART2_EscribirString ("Mensaje almacenado: ");

			USART2_EscribirString(string); // Escribimos el mensaje almacenado tras la comunicación.
			USART2_EscribirString("\r\n"); // Retorno carro y espacio.

		}

		if (op == 3) { // Prueba de transmisión PuertoSerie con PC usando COM de STLINK.
					   // Version final de maquina expendedora.

			for (volatile int i = 0 ; i < 100000 ; i++); // Delay. Iniciamos LED para decir que empezamos.

			GPIOA -> GPIO_BSRR.REG = (1 << 5); // LED on.

			char string[13]; // Creamos un buffer de 13 almacenados en RAM para rellenar.

			USART2_LeerString (string, 13); // Leemos el string que nos ha enviado el PC.

			int cap = 13; // El mensaje de entrada maximo tendrá 13 caracteres.

			int testn = 0;
			int contadorSaldo = 0;

			uint16_t testInt = 0;
			uint16_t carrito = 0;
			uint16_t saldo = 0;
			uint16_t multiplicadores [5] = {10000, 1000, 100, 10, 1};

			for (int i = 0 ; i < (cap) ; i++) {

				if (string [i] == '\0') {break;} // Terminamos bucle si hemos llegado a fin de linea.

				if (string [i] == '\n') { testn = 1; } // Si detectamos un \n, sabemos que pasamos a saldo.

				if (testn == 0) { // Si no detectamos un \n, entonces, estamos en carrito.

					testInt = string [i] - '0';

					carrito = carrito + (testInt * multiplicadores [i]);
				}

				// Ahora, si estamos en saldo, lo obtenemos hasta encontrar otro \n o un \0, por seguridad.
				if (testn != 0 && string[i] != '\n' && string[i] != '\0') {

					testInt = string [i] - '0';

					saldo = saldo + (testInt * multiplicadores [contadorSaldo]); // nuevo cont a 0 para mults.

					contadorSaldo = contadorSaldo + 1;
				}
			}

			int resultadoCompra; // Mandamos a PC un numero segun el resultado de la diferencia.

			char cambioString [20];

			if (saldo < carrito) {
				resultadoCompra = -1;

				sprintf(cambioString, "%d\n", resultadoCompra);
			}


			if (saldo >= carrito) {
				resultadoCompra = 0;

				uint16_t cambioInt = saldo-carrito;

				sprintf(cambioString, "%d\n%05u\n", resultadoCompra, cambioInt);
			}

			// Enviamos si ha sido satisfactorio y el cambio.

			USART2_EscribirString(cambioString);
		}
	}
}
