/*
 * MT_numeric_integration.h
 *
 * Created: 13/05/2021 08:56:09
 *  Author: mtorres
 */ 


#ifndef MT_NUMERIC_INTEGRATION_H_
#define MT_NUMERIC_INTEGRATION_H_

/* File inclusion */
#include <stdint.h>

/* Macro definition */
#define DELTA_X     1


/* Function declaration */
float Integration_Trapezoid_Method(float y_val, uint8_t reset);
float Integration_Midpoint_Method(float y_val, uint8_t reset);
float Integration_Simpson_Method(float y_val, uint8_t reset);

#endif /* MT_NUMERIC_INTEGRATION_H_ */