#include "FreeRTOS.h"
#include "task.h"

#include "plug.h"

/* NOTE
 * The plug task starves SUSP_RX for 128 ticks. Because SUSP_RX has no progress
 * for a time, SUSP_TX eventually raises an error.
 */

static void prvPlug( void *pvParameters );

void vStartPlugTask( void )
{
    /* Enable DWT */
    CoreDebug->DEMCR |= DWT_CTRL_EXCTRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    xTaskCreate(prvPlug, "PLUG", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

#define BLOCK_PERIOD 128U /* Same as priSLEEP_TIME in dynamic.c */

#define NO_SPIN SystemCoreClock

static void prvPlug( void *pvParameters )
{
    (void) pvParameters;

    uint32_t spinning = 3866U;
    uint32_t counter = 0U;

    for ( ;; ) {
        /* Conditionally assign value to "spinning" without branching */
        spinning = spinning ^ ((2286U ^ spinning) & -(counter == 1));
        spinning = spinning ^ ((218U ^ spinning) & -(counter == 2));
        spinning = spinning ^ ((1547U ^ spinning) & -(counter > 3 && counter % 3 == 1));
        spinning = spinning ^ ((218U ^ spinning) & -(counter > 3 && counter % 3 == 2));
        spinning = spinning ^ ((1377U ^ spinning) & -(counter == 128));
        spinning = spinning ^ ((NO_SPIN ^ spinning) & -(counter == 129));

        /* Busy waiting until a precise clock count */
        for ( ; (SysTick->VAL & SysTick_VAL_CURRENT_Msk) > spinning ; ) { }

        /* Reset CYCCNT counter */
        DWT->CYCCNT = 0;

        /* Stop counting after the check duration */
        if (counter < BLOCK_PERIOD)
            counter++;

        /* Yield the processor */
        taskYIELD();
    }
}
