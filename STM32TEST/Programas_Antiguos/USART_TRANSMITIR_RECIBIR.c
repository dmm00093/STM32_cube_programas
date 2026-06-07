#include <stdint.h>
#include "regs_cfg_volatile.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// ------------------------------------------------------------------------------------------
//										   FUNCIONES
// ------------------------------------------------------------------------------------------

// -------------------
// Comunicacion USART
// -------------------

// --------------------------------------------------------------------------------------
// Comprobar TXE en USART_SR (Status Reg), ver si el reg. de transf. de datos esta vacio.
// Si esta a 0, es que esta vacio. El bit se usa durante single buffer transmission.
// Luego, usar USART_DR (Data Register) para almacernar datos.
// --------------------------------------------------------------------------------------

void USART2_EscribirChar (char c) {

	while (USART2->USART_SR.BITS.TXE == 0);

	USART2->USART_DR.REG = c;

}

void USART2_EscribirString (char *str) {

	for (int i = 0; str [i] != '\0'; i++){

		USART2_EscribirChar (str[i]);
	}
}

char USART2_LeerChar (void) {

	while (USART2->USART_SR.BITS.RXNE == 0); // 0: Data is not received.
											 // 1: Received data is ready to be read.

	// Comprobamos si puede recibir datos en RXNE.

	return (char)(USART2->USART_DR.REG);
}

// ------------------------------------------------------------------------------------------
//										    MAIN
// ------------------------------------------------------------------------------------------

int main(void)
{

	// Pagina 146 -> APB2ENR
	// LED verde de usuario -> STM32 I/O PA5.

	// 1. Activaremos el registro IOPAEN, ya que es el IO port A para el LED y USART.

	RCC->APB2ENR.BITS.IOPAEN = 1;

	// El PA5 es para el LED interno verde. Debemos configurar el pin 5 como una salida a 2 MHz.
	// Para esto usamos el registro CRL o CRH, segun el numero de pin.

	GPIOA->GPIO_CRL.BITS.MODE5 = 2;

	// 00: Input mode (reset state)
	// 01: Output mode, max speed 10 MHz.
	// 10: Output mode, max speed 2 MHz.
	// 11: Output mode, max speed 50

	GPIOA->GPIO_CRL.BITS.CNF5 = 0;

	// Puesto a modo de salida push-pull GPIO.
	// In output mode (MODE[1:0] > 00):
	// 00: General purpose output push-pull
	// 01: GPO Open Drain
	// 10: Alternate fuction output push-pull
	// 11: AFO Open Drain

	// --------------------------------------------
	// 2. Habilitamos USART (RCC -> GPIO -> USART).
	// --------------------------------------------

	// USART2_TX -> PA2.
	// USART2_RX -> PA3.

	RCC->APB1ENR.BITS.USART2EN = 1; // USART1_EN, CLK.
	RCC->APB2ENR.BITS.AFIOEN   = 1; // Reloj AFIO habilitado, alternate function para USART.

	// USART1_TX.

	GPIOA->GPIO_CRL.BITS.CNF2  = 2; // Alt.Func. PushPull.
	GPIOA->GPIO_CRL.BITS.MODE2 = 3; // 50 MHz.

	// USART1_RX.

	GPIOA->GPIO_CRL.BITS.CNF3 = 1;  // Input floating.
	GPIOA->GPIO_CRL.BITS.MODE3 = 0; // Es una entrada.

	// TX solo va a hacer caso al USART, no le hace caso al BSRR.

	// ---------------------------------------------------------------------------------------
	// 3. Configuracion baudios en frecuencia. (reloj interno PCLK2, pag 799) (Pag 92 es clocks).
	// Por defecto, se inicializa a 8 MHz el HSI interno, pero puedo configurarlo a 72
	// ---------------------------------------------------------------------------------------

	// USARTDIV = fclk / 16 * Baudrate
	// Frac = DIV_Decimal * 16

	USART2->USART_BRR.BITS.DIV_Mantissa = 52; // Por defecto es el clock HSI de 8 MHz.
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

		op = 1;

		if (op == 0) { // Prueba de mandar texto continuamente a la consola, funciones con TXE.
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

		}
	}
}
