0. Run `make openocd` and `make gdb` in separate terminals.

1. In GDB, set breakpoints.
```
(gdb) hb vBlockingQueueProducer
(gdb) hb BlockQ.c:275
(gdb) hb BlockQ.c:282
```

2. Reset and halt the target.
```
(gdb) moniter reset halt
(gdb) load
(gdb) c
Continuing.

Breakpoint 1, vBlockingQueueProducer (pvParameters=0x20000e80 <ucHeap+2192>) at BlockQ.c:178
178         uint16_t usValue = 0;
(gdb) c
Continuing.
halted: PC: 0x080039da

Breakpoint 1, vBlockingQueueProducer (pvParameters=0x20001398 <ucHeap+3496>) at BlockQ.c:178
178         uint16_t usValue = 0;
(gdb) c
Continuing.
halted: PC: 0x080039da

Breakpoint 2, xAreBlockingQueuesStillRunning () at BlockQ.c:275
275                 xReturn = pdFALSE;
```

3. Check the error information
```
(gdb) p xTasks
$1 = 0
(gdb) p sBlockingProducerCount[0]
$2 = 0
(gdb) p sLastBlockingProducerCount[0]
$3 = 0
```

---

Report

After inserting the plug that starving the QProdB2 task, an error occurs after
the function `vBlockingQueueProducer` is called two times (it should be called
three times). This means a producer task is starved. In step 3,
we verify the starved task is indeed QProdB2 by checking
`sBlockingProducerCount[0]`. Because `sBlockingProducerCount[0]` is zero (so
is `sBlockingConsumerCount[0]`), QProdB2 has no progress (so does QConsB1).
We believe it is the plug task that starved QProdB2. Because QProdB2 is starved,
QConsB1 has no progress.
