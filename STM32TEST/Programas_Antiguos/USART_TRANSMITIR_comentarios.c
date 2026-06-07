#include <stdint.h>
#include "regs_cfg_volatile.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main_viejo_1(void)
{

	// Pagina 146 -> APB2ENR
	// LED verde de usuario -> STM32 I/O PA5.

	// 1. Activaremos el registro IOPAEN, ya que es el IO port A para el LED y USART.

	RCC->APB2ENR.BITS.IOPAEN = 1;

	// 2. El PA5 es para el LED interno verde. Debemos configurar el pin 5 como una salida a 2 MHz.
	// Para esto usamos el registro CRL o CRH, segun el numero de pin.

	GPIOA->GPIO_CRL.BITS.MODE5 = 2;

	// USAMOS NOMBRE ESTABLECIDO EN EL TYPEDEF STRUCT.
	// 2 = 10. Modo a 2 MHz.
	// MODEy[1:0]: Port x mode bits (y= 0 .. 7)
	// These bits are written by software to configure the corresponding I/O port.
	// Refer to Table 20: Port bit configuration table.
	// 00: Input mode (reset state)
	// 01: Output mode, max speed 10 MHz.
	// 10: Output mode, max speed 2 MHz.
	// 11: Output mode, max speed 50

	GPIOA->GPIO_CRL.BITS.CNF5 = 0;

	// Puesto a modo de salida push-pull GPIO.
	// In output mode (MODE[1:0] >00):
	// 00: General purpose output push-pull
	// 01: GPO Open Drain
	// 10: Alternate fuction output push-pull
	// 11: AFO Open Drain

	// --------------------------------------------
	// 2. Habilitamos USART (RCC -> GPIO -> USART).
	// --------------------------------------------

	// Habilitamos USART2_TX,RX. Esto es porque USART2 el USB, Virtual COM. Cuelga de bus APB1.

	// USART2_TX -> PA2.
	// USART2_RX -> PA3.

	RCC->APB1ENR.BITS.USART2EN = 1; // USART1_EN, CLK.
	RCC->APB2ENR.BITS.AFIOEN   = 1; // Reloj AFIO habilitado, alternate function para USART.

	// USART1_TX.

	GPIOA->GPIO_CRL.BITS.CNF2  = 2; // Alt.Func. PushPull.
	GPIOA->GPIO_CRL.BITS.MODE2 = 3; // 50 MHz.

	// USART1_RX:

	GPIOA->GPIO_CRL.BITS.CNF3 = 1;  // Input floating.
	GPIOA->GPIO_CRL.BITS.MODE3 = 0; // Es una entrada.

	// No necesitamos ni debemos usar BSRR para los pines USART.
	// TX solo va a hacer caso al USART, no le hace caso al BSRR.

	// ---------------------------------------------------------------------------------------
	// Configuracion baudios en frecuencia. (reloj interno PCLK2, pag 799) (Pag 92 es clocks).
	// Por defecto, se inicializa a 8 MHz el HSI interno, pero puedo configurarlo a 72
	// ---------------------------------------------------------------------------------------

	// USARTDIV = fclk / 16 * Baudrate
	// Frac = DIV_Decimal * 16

	USART2->USART_BRR.BITS.DIV_Mantissa = 52; // Por defecto es el clock HSI de 8 MHz.
	USART2->USART_BRR.BITS.DIV_Fraction = 1;

	// --------------------------------------------------------
	// Habilitación de USART (TE, RE, UE en Control Register 1)
	// --------------------------------------------------------

	USART2->USART_CR1.BITS.UE = 1; // USART Enable.
	USART2->USART_CR1.BITS.TE = 1; // Transmitter Enable (Transmitter es TX, TE es su enable)
	USART2->USART_CR1.BITS.RE = 1; // Receiver Enable (Receiver es RX, RE es su enable)

	// -------------------------
	// LOOP (BLINK + USART COMM)
	// -------------------------

	while (1) {

		// ------------------
		// 1. Encendemos LED.
		// ------------------
		
		// ----------------------------------------
		// Iniciamos Puerto GPIO LED verde interno.
		// ----------------------------------------

		GPIOA->GPIO_BSRR.REG = (1 << 5); // Bit 5, es el puerto. El BSRR es atómico:

		// Si escribes un 1 en los bits 0-15, el pin se pone a 1 (Set).

		// Si escribes un 1 en los bits 16-31, el pin se pone a 0 (Reset).

		// Escribir un 0 no hace nada. No necesitas leer antes de escribir.

		for (volatile int i = 0 ; i < 1000000 ; i++); // Retardo. Volatile -> comp. no lo elimina al optimizar.

		GPIOA->GPIO_BSRR.REG = (1 << 21); // Apagamos el LED. 16 + 5 = 21, ponemos bit 21 a 0.

		for (volatile int i = 0 ; i < 1000000 ; i++); // Retardo

		// ---------------------
		// 2. Comunicacion USART
		// ---------------------

		// --------------------------------------------------------------------------------------
		// Comprobar TXE en USART_SR (Status Reg), ver si el reg. de transf. de datos esta vacio.
		// Si esta a 0, es que esta vacio. El bit se usa durante single buffer transmission.
		// lLuego, usar USART_DR (Data Register) para almacernar datos.
		// --------------------------------------------------------------------------------------

		while (USART2->USART_SR.BITS.TXE == 0);

		USART2->USART_DR.REG = 'H';

		// H es una letra. Para el compilador de C, 'H' es un alias para un número.
		// El lenguaje C usa el estándar ASCII.

		// Tabla ASCII:
		// La letra 'H' mayúscula equivale al número 72 en decimal
		// Que en hexadecimal es 0x48.

	}

	// Nota: IDR (Input Data Register)

	// Este registro hace lo contrario. No sirve para mandar electricidad,
	// sino para mirar si hay electricidad viniendo de fuera
	// (por ejemplo, si alguien ha pulsado un botón).
	// Es solo de lectura.
}
