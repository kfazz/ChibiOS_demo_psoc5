#ifndef CMPARAMS_H
#define CMPARAMS_H


#define CORTEX_MODEL            3
#define CORTEX_HAS_FPU          0
#define CORTEX_PRIORITY_BITS    3
#define CORTEX_NUM_VECTORS      32
#define CORTEX_VTOR_INIT __ramvec__

#define USBUART_EP1_Handler Vector40	//0
#define USBUART_EP2_Handler Vector44	//1
#define USBUART_EP3_Handler Vector48	//2
#define USBUART_DP_Handler Vector70	//12
#define USBUART_Sof_Handler Vector94	//21
#define USBUART_Arb_Handler Vector98	//22
#define USBUART_Reset_Handler Vector9C	//23
#define USBUART_EP0_Handler VectorA0  	//24
#define USBUART_Ord_Handler VectorA4	//25

#if !defined(_FROM_ASM_)
#include "core_cm3_psoc5.h"
#include "core_cm3.h"
#include "cmsis_gcc.h"

#endif
#endif
