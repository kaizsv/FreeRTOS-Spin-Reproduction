#include "FreeRTOS.h"
#include "task.h"
#include "plugs.h"

#define NO_SPIN SystemCoreClock

static void prvStarveQConsB3( void *pvParameters )
{
    (void) pvParameters;

    uint32_t spinning = 3065U;
    uint32_t counter = 0U;

    for ( ;; ) {
        /* Conditionally assign value to "spinning" without branching */
        spinning = spinning ^ ((3112U ^ spinning) & -(counter == 1));

        /* Busy waiting until a precise clock count */
        for ( ; (SysTick->VAL & SysTick_VAL_CURRENT_Msk) > spinning ; ) { }

        /* Reset CYCCNT counter */
        DWT->CYCCNT = 0;

        /* Stop counting after the check duration */
        if (counter < 3000)
            counter++;

        /* Yield the processor */
        taskYIELD();
    }
}

static void vEnableDWT( void )
{
    CoreDebug->DEMCR |= DWT_CTRL_EXCTRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


void vStartPlugTask( void )
{
    vEnableDWT();
    xTaskCreate(prvStarveQConsB3, "PLUG", configMINIMAL_STACK_SIZE, NULL,
            tskIDLE_PRIORITY, NULL);
}
