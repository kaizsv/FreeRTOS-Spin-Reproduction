0. Run `make openocd` and `make gdb` in separate terminals.

1. In GDB, set breakpoints.
```
(gdb) hb vBlockingQueueConsumer
(gdb) hb BlockQ.c:275
(gdb) hb BlockQ.c:282
```

2. Reset and halt the target.
```
(gdb) moniter reset halt
(gdb) load
(gdb) c
Continuing.

Breakpoint 1, vBlockingQueueConsumer (pvParameters=0x20000878 <ucHeap+648>) at BlockQ.c:213
213         uint16_t usData, usExpectedValue = 0;
(gdb) c
Continuing.
halted: PC: 0x08003a30

Breakpoint 1, vBlockingQueueConsumer (pvParameters=0x20001418 <ucHeap+3624>) at BlockQ.c:213
213         uint16_t usData, usExpectedValue = 0;
(gdb) c
Continuing.
halted: PC: 0x08003a30

Breakpoint 3, xAreBlockingQueuesStillRunning () at BlockQ.c:282
282                 xReturn = pdFALSE;
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

