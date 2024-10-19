#include "FreeRTOS.h"
#include "task.h"

#include "plug.h"

static void prvPlug( void *pvParameters );

void vStartPlugTask( void )
{
    /* Enable DWT */
    CoreDebug->DEMCR |= DWT_CTRL_EXCTRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    xTaskCreate(prvPlug, "PLUG", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

#define STARVE_1_MINUTE     60000U /* 1 * 60 * 1000 */
#define STARVE_5_MINUTE     300000U
#define STARVE_1_HOUR       3600000U
#define STARVE_UINT32_MAX   4294967295U /* about 49 days and 17 hours */
#define FOREVER_STARVED     4294967296U

#define START_STARVING      3000U
#define STARVE_DURATION     FOREVER_STARVED

#define NO_SPIN SystemCoreClock

static void prvPlug( void *pvParameters )
{
    (void) pvParameters;

    uint32_t spinning = NO_SPIN;
    uint32_t counter = 0U;

    for ( ;; ) {
        /* Conditionally assign value to "spinning" without branching */
        spinning = spinning ^ ((216U ^ spinning) & -(counter == START_STARVING));
        spinning = spinning ^ ((NO_SPIN ^ spinning) & -(counter == STARVE_DURATION));

        /* Busy waiting until a precise clock count */
        for ( ; (SysTick->VAL & SysTick_VAL_CURRENT_Msk) > spinning ; ) { }

        /* Reset CYCCNT counter */
        DWT->CYCCNT = 0;

        if (counter < STARVE_DURATION)
            counter++;

        /* Yield the processor */
        taskYIELD();
    }
}
