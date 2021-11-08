/*
 * MT_dimmer_leading_edge.h
 *
 * Created: 3/03/2021 09:09:45
 *  Author: mtorres
 */ 


#ifndef MT_DIMMER_LEADING_EDGE_H_
#define MT_DIMMER_LEADING_EDGE_H_

/* File inclusion */
#include <stdint.h>

/* Function declaration */
void Initialize_Dimmer_Leading_Edge_Table(float period);
uint8_t Get_Dimmer_Leading_Edge_Phase_Time(uint8_t percent, float *phase);
uint8_t Get_Dimmer_Leading_Edge_Integer_Phase_Time(uint8_t percent, uint16_t *phase);

#endif /* MT_DIMMER_LEADING_EDGE_H_ */