/*
 * Autor: David Martinez Mora (dmm00093@red.ujaen.es)
 * Funcionalidad: programación de los registros para habilitar SysTick.
 */

#ifndef SYSTICK_REGS_H_
#define SYSTICK_REGS_H_

/*
-------------------------------------------------------
 * DEL CORTEX M3 ARM. Manual -> STM32F103RB.
 * No del NUCLEO.
 * El manual del micro y de la placa son diferentes.
 * SysTick pertenece a una función interna del micro.
-------------------------------------------------------
*/

/*
---------------------------------
DEFINICION DEL REGISTRO GENERICA
---------------------------------
*/

typedef union {

	uint32_t REG;

	struct {
		uint32_t ENABLE    : 1;
		uint32_t TICK_INT  : 1;
		uint32_t CLKSOURCE : 1;
		uint32_t RESERV1   : 13;
		uint32_t COUNTFLAG : 1;
		uint32_t RESERV2   : 15;
	} BITS;

} STK_CTRL_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t RELOAD : 24;
		uint32_t RESERV : 8;
	} BITS;

} STK_LOAD_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t CURRENT : 24;
		uint32_t RESERV  : 8;
	}BITS;

} STK_VAL_Type;

typedef union {

	uint32_t REG;

	struct {
		uint32_t TENMS   : 24;
		uint32_t RESERV  : 8;
	}BITS;

} STK_CALIB_Type;

typedef struct {

	STK_CTRL_Type  STK_CTRL;
	STK_LOAD_Type  STK_LOAD;
	STK_VAL_Type   STK_VAL;
	STK_CALIB_Type STK_CALIB;

} STK_TypeDef;

/*
----------------------------------
ASIGNACIÓN DEL REGISTRO EN MEMORIA
----------------------------------
*/

/* Dirección base REAL del SysTick en el núcleo ARM Cortex-M3
 * Cortex-M3 Programming Manual (PM0056)
 * SCS (System Control Space) */

#define STK_DirBase 0xE000E010

#define SysTick ((volatile STK_TypeDef * ) STK_DirBase)

#endif /* SYSTICK_REGS_H_ */
