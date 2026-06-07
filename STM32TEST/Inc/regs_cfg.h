// -----------------------------------------------
// Registros RCC, USART, GPIO para usar en el main.
// -----------------------------------------------

// ---------------------------------------------------
// Consultar -> MANUAL REFERENCIA (~1000 pags).
// Direcciones Registros --> Pagina 50.
// RCC -> Pagina 156.
// USART -> Pagina 827.
// ---------------------------------------------------

#ifndef REGS_CFG_H
#define REGS_CFG_H

#include <stdint.h> // para usar uint32,16_t

// -----------------------------
// 1 - DEFINICIÓN REGISTRO RCC.
// -----------------------------

// Primero definimos los Type de registros.
// Luego, agrupamo en TypeDef.
// Finalmente, asignamos memoria y puntero correspondiente.
// El compilador es secuencial, no funcionará de otra forma.

// ------------------------
// Definimos registros RCC.
// ------------------------

typedef union {

	uint32_t REG; // Aqui estamos haciendo que sea el struct entero una "caja" de 32 bits.
				  // Entonces, podemos poner tono a 0 de un plumazo si queremos.
	// Por ejemplo, si hacemos RCC->APB2RSTR.REG = 0; , ponemos TODOS a 0.

	struct {
		uint32_t AFIORST : 1; // Cuidado!!! 0 a 31 SON 32 BITS.
		uint32_t RESERV1 : 1; // Debemos sumar 32 BITS EN TOTAL.
		uint32_t IOPARST : 1;
		uint32_t IOPBRST : 1;
		uint32_t IOPCRST : 1;
		uint32_t IOPRST1 : 1;
		uint32_t IOPERST : 1;
		uint32_t RESERV2 : 2;
		uint32_t ADC1RST : 1;
		uint32_t ADC2RST : 1;
		uint32_t TIM1RST : 1;
		uint32_t SPI1RST : 1;
		uint32_t RESERV3 : 1;
		uint32_t USART1RST : 1;
		uint32_t RESERV4 : 17;
	} BITS;

} RCC_APB2RSTR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t TIM2RST : 1;
		uint32_t TIM3RST : 1;
		uint32_t TIM4RST : 1;
		uint32_t TM5RST : 1;
		uint32_t TM6RST : 1;
		uint32_t TM7RST : 1;
		uint32_t TIM12RST : 1;
		uint32_t TIM13RST : 1;
		uint32_t TIM14RST : 1;
		uint32_t RESERV1 : 1;
		uint32_t RESERV2 : 1;
		uint32_t WWDGRST : 1;
		uint32_t RESERV3 : 1;
		uint32_t RESERV4 : 1;
		uint32_t SPI2RST : 1;
		uint32_t SPI3RST : 1;
		uint32_t RESERV5 : 1;
		uint32_t USART2RST : 1;
		uint32_t USART3RST : 1;
		uint32_t UART4RST : 1;
		uint32_t UART5RST : 1;
		uint32_t I2C1RST : 1;
		uint32_t I2C2RST : 1;
		uint32_t USBRST : 1;
		uint32_t RESERV6 : 1;
		uint32_t CANRST : 1;
		uint32_t RESERV7 : 1;
		uint32_t BKPRST : 1;
		uint32_t PWRRST : 1;
		uint32_t DACRST : 1;
		uint32_t RESERV8 : 1;
		uint32_t RESERV9 : 1;
	} BITS;

} RCC_APB1RSTR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t AFIOEN : 1;
		uint32_t RESERV1 : 1;
		uint32_t IOPAEN : 1;
		uint32_t IOPBEN : 1;
		uint32_t IOPCEN : 1;
		uint32_t IOPDEN : 1;
		uint32_t IOPEEN : 1;
		uint32_t IOPFEN : 1;
		uint32_t IOPGEN : 1;
		uint32_t ADC1EN : 1;
		uint32_t ADC2EN : 1;
		uint32_t TIM1EN : 1;
		uint32_t SPI1EN: 1;
		uint32_t TIM8EN : 1;
		uint32_t USART1EN : 1;
		uint32_t ADC3EN : 1;
		uint32_t RESERV2 : 1;
		uint32_t RESERV3 : 1;
		uint32_t RESERV4 : 1;
		uint32_t TIM9EN : 1;
		uint32_t TIM10EN : 1;
		uint32_t TIM11EN : 1;
		uint32_t RESERV5 : 10;
	} BITS;

} RCC_APB2ENR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t TIM2EN : 1;
		uint32_t TIM3EN : 1;
		uint32_t TIM4EN : 1;
		uint32_t TIM5EN : 1;
		uint32_t TIM6EN : 1;
		uint32_t TIM7EN : 1;
		uint32_t TIM12EN : 1;
		uint32_t TIM13EN : 1;
		uint32_t TIM14EN : 1;
		uint32_t RESERV1 : 2;
		uint32_t WWDGEN : 1;
		uint32_t RESERV2 : 2;
		uint32_t SPI2EN : 1;
		uint32_t SPI3EN : 1;
		uint32_t RESERV3 : 1;
		uint32_t USART2EN : 1;
		uint32_t USART3EN : 1;
		uint32_t UART4EN : 1;
		uint32_t UART5EN : 1;
		uint32_t I2C1EN : 1;
		uint32_t I2C2EN : 1;
		uint32_t USBEN : 1;
		uint32_t RESERV4 : 1;
		uint32_t CANEN : 1;
		uint32_t RESERV5 : 1;
		uint32_t BKPEN : 1;
		uint32_t PWREN : 1;
		uint32_t DACEN : 1;
		uint32_t RESERV6 : 2;
	}BITS;

} RCC_APB1ENR_Type;

// -----------------------
// Agrupamos registros RCC.
// -----------------------

// IMP: Debe estar el espacio en memoria tal y cual se define en el manual.

typedef struct {

	uint32_t 		  RESERV1;  // 0x000
	uint32_t 		  RESERV2;  // 0x004
	uint32_t 	      RESERV3;  // 0x008
	RCC_APB2RSTR_Type APB2RSTR; // 0x00C
    RCC_APB1RSTR_Type APB1RSTR; // 0x010
	uint32_t 		  RESERV4;  // 0x014
	RCC_APB2ENR_Type  APB2ENR;  // 0x018
	RCC_APB1ENR_Type  APB1ENR;  // 0x01C

} RCC_TypeDef; // TypeDef es para diferenciar el struct / union.


// -----------------------------------
// Asignamos registros RCC en memoria.
// -----------------------------------

#define RCC_BASE_ADDR 0x40021000 // Definimos cte de direccion del registro RCC (pag 50).
#define RCC ((RCC_TypeDef *) RCC_BASE_ADDR) // Puntero para acceder a todos los RCC (primer struct).

/* ==============================================================================
 * EXPLICACIÓN DE LA ARQUITECTURA DE MEMORIA Y PUNTEROS (Ejemplo con RCC)
 * ==============================================================================
 *
 * 1. LA DIRECCIÓN BASE (El "Edificio")
 * #define RCC_BASE_ADDR 0x40021000
 * La memoria del microcontrolador está dividida en bloques. La dirección base
 * es la puerta principal del bloque del RCC. A partir de esa dirección (0x40021000)
 * empiezan a colocarse todos sus registros. No importa qué haya antes en la memoria,
 * nuestro código "aterriza" directamente aquí.
 *
 * 2. LA LÓGICA DEL PUNTERO MAESTRO
 * #define RCC ((RCC_TypeDef *) RCC_BASE_ADDR)
 * * Esta línea es una sustitución de texto (macro) que hace 3 cosas vitales:
 * * a) RCC_BASE_ADDR: Para el compilador, 0x40021000 es solo un número gigante.
 * No sabe que es una dirección física.
 *
 * b) El Casting -> (RCC_TypeDef *): Aquí está la magia. El asterisco (*) le
 * dice al compilador: "Este número no es un número normal, es una DIRECCIÓN
 * DE MEMORIA". Y el nombre del struct le dice: "Lo que hay dentro de esa
 * dirección tiene la forma y el tamaño del plano que diseñé en RCC_TypeDef".
 *
 * c) Los paréntesis exteriores ((...)): Envuelven todo para que al usar la
 * palabra "RCC" en el código, se respete la prioridad matemática y no haya
 * errores de compilación.
 *
 * 3. ¿CÓMO FUNCIONA EN LA PRÁCTICA?
 * No lleva comas porque no es una función. Cuando en el main escribimos:
 * RCC->APB2RSTR.BITS.IOPAEN = 1;
 * * Antes de compilar, C sustituye la palabra "RCC" por su macro:
 * ((RCC_TypeDef *) 0x40021000)->APB2RSTR.BITS.IOPAEN = 1;
 * * Traducción final para el microcontrolador: "Ve a la calle 0x40021000, aplica
 * el plano de habitaciones (RCC_TypeDef), entra en la habitación APB2RSTR
 * (que gracias a los 'dummy' cayó exactamente en el offset 0x0C) y enciende el
 * interruptor IOPAEN".
 * ==============================================================================
 */

// ------------------------------
// 2 - DEFINICIÓN REGISTRO USART.
// ------------------------------

// --------------------------
// Definimos registros USART.
// --------------------------

typedef union {

	uint32_t REG;

	struct {
		uint32_t PE : 1;
		uint32_t FE : 1;
		uint32_t NE : 1;
		uint32_t ORE : 1;
		uint32_t IDLE : 1;
		uint32_t RXNE : 1;
		uint32_t TC : 1;
		uint32_t TXE : 1;
		uint32_t LBD : 1;
		uint32_t CTS : 1;
		uint32_t RESERV : 22;
	}BITS;

} USART_SR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t DR : 9; // Cuidado corchetes!!!!
		uint32_t RESERV : 23;
	}BITS;

} USART_DR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t DIV_Fraction : 4;
		uint32_t DIV_Mantissa : 12;
		uint32_t RESERV : 16;
	}BITS;

} USART_BRR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t SBK : 1;
		uint32_t RWU : 1;
		uint32_t RE : 1;
		uint32_t TE : 1;
		uint32_t IDLEIE : 1;
		uint32_t RXNEIE : 1;
		uint32_t TCIE : 1;
		uint32_t TXEIE : 1;
		uint32_t PEIE : 1;
		uint32_t PS : 1;
		uint32_t PCE : 1;
		uint32_t WAKE : 1;
		uint32_t M : 1;
		uint32_t UE : 1;
		uint32_t RESERV : 18;
	}BITS;

} USART_CR1_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t ADD : 4;
		uint32_t RESERV1 : 1;
		uint32_t LBDL : 1;
		uint32_t LBDIE : 1;
		uint32_t RESERV2 : 1;
		uint32_t LBCL : 1;
		uint32_t CPHA : 1;
		uint32_t CPOL : 1;
		uint32_t CLKEN : 1;
		uint32_t STOP : 2;
		uint32_t LINEN : 1;
		uint32_t RESERV3 : 17;
	}BITS;

} USART_CR2_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t EIE : 1;
		uint32_t IREN : 1;
		uint32_t IRLP : 1;
		uint32_t HDSEL : 1;
		uint32_t NACK : 1;
		uint32_t SCEN : 1;
		uint32_t DMAR : 1;
		uint32_t DMAT : 1;
		uint32_t RTSE : 1;
		uint32_t CTSE : 1;
		uint32_t CTSIE : 1;
		uint32_t RESERV : 21;
	}BITS;

} USART_CR3_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t PSC : 8;
		uint32_t GT  : 8;
		uint32_t RESERV : 16;
	}BITS;

} USART_GTPR_Type;

// --------------------------
// Agrupamos registros USART.
// --------------------------

typedef struct {

	USART_SR_Type   USART_SR;   // 0x00
	USART_DR_Type   USART_DR;   // 0x04
	USART_BRR_Type  USART_BRR;  // 0x08
	USART_CR1_Type  USART_CR1;  // 0x0C
	USART_CR2_Type  USART_CR2;  // 0x10
	USART_CR3_Type  USART_CR3;  // 0x14
	USART_GTPR_Type USART_GTPR; // 0x18

} USART_TypeDef;

// -------------------------------------
// Asignamos registros USART en memoria.
// -------------------------------------

// Todos los USART del chip son exactamente iguales por dentro. Son clones.
// Tienen los mismos registros, en el mismo orden y con los mismos bits.
// Simplemente usamos la direccion del USART1. Podriamos usar la del 2, 3... da igual.
// Si queremos otro USART, pues lo añadimos aquí.

#define USART1_BASE_ADDR 0x40013800 // Usamos Dir. USART1.
#define USART2_BASE_ADDR 0x40004400 // Usamos Dir. USART2.

#define USART1 ((USART_TypeDef *) USART1_BASE_ADDR)
#define USART2 ((USART_TypeDef *) USART2_BASE_ADDR)

// -----------------------------
// 3 - DEFINICIÓN REGISTROS GPIO.
// -----------------------------

// -------------------------
// Definimos registros GPIO.
// -------------------------

typedef union {

	uint32_t REG;

	struct {
		uint32_t MODE0 : 2;
		uint32_t CNF0 : 2;
		uint32_t MODE1 : 2;
		uint32_t CNF1 : 2;
		uint32_t MODE2 : 2;
		uint32_t CNF2 : 2;
		uint32_t MODE3 : 2;
		uint32_t CNF3 : 2;
		uint32_t MODE4 : 2;
		uint32_t CNF4 : 2;
		uint32_t MODE5 : 2;
		uint32_t CNF5 : 2;
		uint32_t MODE6 : 2;
		uint32_t CNF6 : 2;
		uint32_t MODE7 : 2;
		uint32_t CNF7 : 2;
	}BITS;

} GPIOx_CRL_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t MODE8 : 2;
		uint32_t CNF8 : 2;
		uint32_t MODE9 : 2;
		uint32_t CNF9 : 2;
		uint32_t MODE10 : 2;
		uint32_t CNF10 : 2;
		uint32_t MODE11 : 2;
		uint32_t CNF11 : 2;
		uint32_t MODE12 : 2;
		uint32_t CNF12 : 2;
		uint32_t MODE13 : 2;
		uint32_t CNF13 : 2;
		uint32_t MODE14 : 2;
		uint32_t CNF14 : 2;
		uint32_t MODE15 : 2;
		uint32_t CNF15 : 2;
	}BITS;

} GPIOx_CRH_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t IDRy   : 16;
		uint32_t RESERV : 16;
	}BITS;

} GPIOx_IDR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t ODRy   : 16;
		uint32_t RESERV : 16;
	}BITS;

} GPIOx_ODR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t BSR : 16;
		uint32_t BR  : 16;
	}BITS;

} GPIOx_BSRR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t BR 	: 16;
		uint32_t RESERV : 16;
	}BITS;

} GPIOx_BRR_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t LCK	: 16;
		uint32_t LCKK   :  1;
		uint32_t RESERV : 15;
	}BITS;

} GPIOx_LCKR_Type;

// -------------------------
// Agrupamos registros GPIO.
// -------------------------

typedef struct {

	GPIOx_CRL_Type  GPIO_CRL;   // 0x00
	GPIOx_CRH_Type  GPIO_CRH;   // 0x04
	GPIOx_IDR_Type	GPIO_IDR;	// 0x08
	GPIOx_ODR_Type	GPIO_ODR;	// 0x0C
	GPIOx_BSRR_Type GPIO_BSRR;  // 0x10
	GPIOx_BRR_Type  GPIO_BRR;	// 0x14
	GPIOx_LCKR_Type GPIO_LCKR; 	// 0x18

} GPIO_TypeDef;

// ------------------------------------
// Asignamos registros GPIO en memoria.
// ------------------------------------

#define GPIOA_BASE_ADDR 0x40010800
#define GPIOB_BASE_ADDR 0x40010C00
#define GPIOC_BASE_ADDR 0x40011000
#define GPIOD_BASE_ADDR 0x40011400
#define GPIOE_BASE_ADDR 0x40011800
#define GPIOF_BASE_ADDR 0x40011C00
#define GPIOG_BASE_ADDR 0x40012000


#define GPIOA ((GPIO_TypeDef*) GPIOA_BASE_ADDR)
#define GPIOB ((GPIO_TypeDef*) GPIOB_BASE_ADDR)
#define GPIOC ((GPIO_TypeDef*) GPIOC_BASE_ADDR)
#define GPIOD ((GPIO_TypeDef*) GPIOD_BASE_ADDR)
#define GPIOE ((GPIO_TypeDef*) GPIOE_BASE_ADDR)
#define GPIOF ((GPIO_TypeDef*) GPIOF_BASE_ADDR)
#define GPIOG ((GPIO_TypeDef*) GPIOG_BASE_ADDR)

#endif
