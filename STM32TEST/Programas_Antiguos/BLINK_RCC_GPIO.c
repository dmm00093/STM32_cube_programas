#include <stdint.h>
#include "regs_cfg.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main_viejo_2(void)
{
    /* Loop forever */
	//for(;;);

	// Pagina 146 -> APB2ENR
	// LED verde de usuario -> STM32 I/O PA5 (SPI1_SCK)
	// Bit 12 SPI1EN: SPI 1 clock enable
	// Set and cleared by software.
	// 0: SPI 1 clock disabled
	// 1: SPI 1 clock enabled

	// 1. Activaremos el registro IOPAEN, unicamente ese, ya que es el IO port para el LED.

	RCC->APB2ENR.BITS.IOPAEN = 1;

	// 2. El PA5 es para el LED interno verde. Debemos configurar el pin 5 como una salida a 2 MHz.
	// Para esto usamos el registro CRL o CRH, segun el numero de pin.

	GPIOA->GPIO_CRL.BITS.MODE5 = 2; // USAMOS NOMBRE ESTABLECIDO EN EL TYPEDEF STRUCT.
	// 2 = 10. Modo a 2 MHz.
	// MODEy[1:0]: Port x mode bits (y= 0 .. 7)
	// These bits are written by software to configure the corresponding I/O port.
	// Refer to Table 20: Port bit configuration table.
	// 00: Input mode (reset state)
	// 01: Output mode, max speed 10 MHz.
	// 10: Output mode, max speed 2 MHz.
	// 11: Output mode, max speed 50

	GPIOA->GPIO_CRL.BITS.CNF5 = 0;   // In output mode (MODE[1:0] >00):
										   // 00: General purpose output push-pull
	// Puesto a modo de salida push-pull GPIO.

	// 3. Blinkeo del LED.
	// Para esto, debemos usar el registro ODR. Este registro es el que maneja el transistor.
	// El registro conecta físicamente a los transistores que pasan 3.3V O 0.
	// Si escribes un 1 en el bit 5 del ODR -> El chip cierra el circuito y salen 3.3V por el pin PA5.
	// LED encendido.
	// Si escribes un 0 en el bit 5 del ODR -> El chip abre el circuito y el pin se va a 0V (tierra).
	// LED apagado.

	while (1) {

		// Encendemos LED.

		GPIOA->GPIO_ODR.REG |= (1 << 5); // Desplazamos registro ODRy a la posición sexta (bit 5 (PA5)).

		for (volatile int i = 0 ; i < 500000 ; i++); // Retardo. Volatile -> comp. no lo elimina al optimizar.

		GPIOA->GPIO_ODR.REG &= ~(1 << 5); // Apagamos el LED. Ponemos a 0 el bit 5.

		for (volatile int i = 0 ; i < 500000 ; i++); // Retardo

	}

	// Nota: El "Chismoso": IDR (Input Data Register)

	// Este registro hace lo contrario. No sirve para mandar electricidad,
	// sino para mirar si hay electricidad viniendo de fuera
	// (por ejemplo, si alguien ha pulsado un botón).
	// Es solo de lectura.
}
