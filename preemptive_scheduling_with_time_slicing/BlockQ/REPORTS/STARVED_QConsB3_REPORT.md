0. Run `make openocd` and `make gdb` in separate terminals.

1. In GDB, set breakpoints.
```
(gdb) hb vBlockingQueueConsumer
(gdb) hb BlockQ.c:292
(gdb) hb BlockQ.c:299
```

2. Reset and halt the target.
```
(gdb) moniter reset halt
(gdb) load
(gdb) c
Continuing.

Breakpoint 1, vBlockingQueueConsumer (pvParameters=0x200005f8 <ucHeap+636>) at BlockQ.c:223
223	    uint16_t usData, usExpectedValue = 0;
(gdb) c
Continuing.
halted: PC: 0x080007ac

Breakpoint 1, vBlockingQueueConsumer (pvParameters=0x20001158 <ucHeap+3548>) at BlockQ.c:223
223	    uint16_t usData, usExpectedValue = 0;
(gdb) c
Continuing.
halted: PC: 0x080007ac

Breakpoint 3, xAreBlockingQueuesStillRunning () at BlockQ.c:299
299	            xReturn = pdFALSE;
```

3. Check the error information
```
(gdb) p xTasks
$1 = 1
(gdb) p sBlockingConsumerCount[1]
$2 = 1
(gdb) p sBlockingProducerCount[1]
$3 = 0
```

---

Report

The error information in Step 3 is telling that a producer task is starved,
not a consumer task. We finally find that this mistake is caused by
misplacing `sBlockingConsumerCount[1]` and `sBlockingProducerCount[1]` in
the original `BlockQ.c`. The following two line of code should be swapped.

[BlockQ.c:147](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Common/Minimal/BlockQ.c#L147)
[BlockQ.c:152](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Common/Minimal/BlockQ.c#L152)

After figuring out the mistake, the error information in step 3 is actually
telling that QConsB3 is never executed while QProdB4 has send one message
to the buffer. In other words, QConsB3 is starved for the check duration (3000 ms).

