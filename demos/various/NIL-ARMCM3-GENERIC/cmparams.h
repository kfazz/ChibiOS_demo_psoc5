#ifndef CMPARAMS_H
#define CMPARAMS_H


#define CORTEX_MODEL            3
#define CORTEX_HAS_FPU          0
#define CORTEX_PRIORITY_BITS    3
#define CORTEX_NUM_VECTORS      32
#define CORTEX_VTOR_INIT __ramvec__

#if !defined(_FROM_ASM_)
#include "core_cm3_psoc5.h"
#include "core_cm3.h"
#include "cmsis_gcc.h"

#endif
#endif
