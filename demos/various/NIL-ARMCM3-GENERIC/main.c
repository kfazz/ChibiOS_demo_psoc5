#include "ch.h"
#include "psoc_registers.h"
#include "psoc_regmacro.h"
#include "psoc_support.h"
#include <project.h>
#include "stdio.h"


#if !defined(SYSTEM_CLOCK)
#define SYSTEM_CLOCK 48000000U
#endif

#define USBFS_DEVICE    (0u)

//Ram copy of vector table
__attribute__ ((section(".ramvectors")))
uint32_t _vectors_relocated[CORTEX_NUM_VECTORS];

/* The buffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_BUFFER_SIZE (64u)
#define LINE_STR_LENGTH     (20u)


/*
 * @brief   System Timer handler.
 */
CH_IRQ_HANDLER(SysTick_Handler) {

  CH_IRQ_PROLOGUE();

  chSysLockFromISR();
  chSysTimerHandlerI();
  chSysUnlockFromISR();

  CH_IRQ_EPILOGUE();
}

static uint32_t seconds_counter;
static uint32_t minutes_counter;

static uint16 count;
static uint8 buffer[USBUART_BUFFER_SIZE];


/*
 * Seconds counter thread.
 */


static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  while (true) {
    chThdSleepMilliseconds(1000);
    //printf("Seconds Count: %d", seconds_counter);
    seconds_counter++;
  }
}



/*
 * Minutes counter thread.
 */
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;
    while(true)
    {
    Clear_Pin(REG_PRT2_PC1);
    chThdSleepSeconds(1);
    Set_Pin(REG_PRT2_PC1);
    chThdSleepSeconds(1);
  }
}

/*
 * Threads creation table, one entry per thread.
 */
THD_TABLE_BEGIN
  THD_TABLE_THREAD(0, "counter1", waThread1, Thread1, NULL)
  THD_TABLE_THREAD(1, "counter2", waThread2, Thread2, NULL)
THD_TABLE_END

int main()
{

  cyfitter_cfg();

  /*
   * Hardware initialization, in this simple demo just the systick timer is
   * initialized.
   */

  SysTick->LOAD = SYSTEM_CLOCK / 1000 - (systime_t)1;
  SysTick->VAL = (uint32_t)0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  NVIC_SetPriority(SysTick_IRQn, 38);



    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);


    // LED: OUT: Port 6.0 Strong, initially OFF
    // DM 2,1,0: Strong: 1 1 0 (DATA = 1 ON, 0 OFF)
    Set_Pin_Drive_Mode(REG_PRT2_PC1, PRT_PC__DRIVE_MODE__STRONG);
    Clear_Pin(REG_PRT2_PC1);

    // Button: IN: Port 6.1 Pull Up
    // DM 2,1,0: Pullup: 0 1 0 (Data = 1 pullup 5K Vcc) [not 0 - hard 0V]
    Set_Pin_Drive_Mode(REG_PRT2_PC2, PRT_PC__DRIVE_MODE__RES_PULLUP);
    Set_Pin(REG_PRT2_PC2);


  /*
   * System initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  chSysInit();
    CyGlobalIntEnable;


while(1)
{

        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data
                 * from host. */
                USBUART_CDC_Init();
            }
        }

        /* Service USB CDC when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data from host. */
            if (0u != USBUART_DataIsReady())
            {
                /* Read received data and re-enable OUT endpoint. */
                count = USBUART_GetAll(buffer);

                if (0u != count)
                {
                    /* Wait until component is ready to send data to host. */
                    while (0u == USBUART_CDCIsReady())
                    {
//    chThdSleepMilliseconds(10);
                    }

                    /* Send data back to host. */
                    USBUART_PutData(buffer, count);

                    /* If the last sent packet is exactly the maximum packet
                    *  size, it is followed by a zero-length packet to assure
                    *  that the end of the segment is properly identified by
                    *  the terminal.
                    */
                    if (USBUART_BUFFER_SIZE == count)
                    {
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USBUART_CDCIsReady())
                        {
  //  chThdSleepMilliseconds(10);
                        }

                        /* Send zero-length packet to PC. */
                        USBUART_PutData(NULL, 0u);
                    }
                }
            }
        }

  }
}


/* [] END OF FILE */
