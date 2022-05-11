countsem.c consists of two tasks. In order to show that one of the task can
be forever starved, we redesign the timer call back function.
If a task has progress, the timer call back function toggles the corresponding
on-board LEDs. In countsem.c, the task CNT1 corresponds to the green light while
the task CNT2 corresponds to the red light. If a LED is stopped toggling,
the corresponding task is starved.

To insert the plug task. Use `make PLUG=cnt1`.
