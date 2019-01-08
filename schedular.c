#include "schedular.h"
#include <stdio.h>
#include "systicks.h"

// an task array to store all the task, and their params
// and the task schedular will read this list the run
// task according their params
static task_t task_list[TASK_SIZE];

static get_ticks_t schedular_get_ticks;

void schedular_init(get_ticks_t get_ticks){
    uint16_t i;
    for (i = 0; i < TASK_SIZE; i++){
        task_list[i].task = 0;
        task_list[i].ticks_interval = 0;
        task_list[i].ticks_previous = 0;
    }
    if (get_ticks != 0) schedular_get_ticks = get_ticks;
}

void schedular_add(func_t task, uint32_t interval){
    // find an empty task
    uint16_t i;
    for (i = 0; i < TASK_SIZE; i++){
        // if an empty location is found, add the new task to it
        if (task_list[i].task == 0){
            task_list[i].ticks_interval = interval;
            task_list[i].task = task;
            return;
        }
    }
}

void schedular_run(void){
    uint16_t i = 0;
    uint32_t ticks = 0;
    // loop through all the task
    for (i = 0; i < TASK_SIZE; i++){
        // find a task
        if (task_list[i].task != 0){
            // if task's timer is expired, run the task
            ticks = schedular_get_ticks();
            if ((ticks - task_list[i].ticks_previous) >= task_list[i].ticks_interval){
                task_list[i].ticks_previous = ticks;
                task_list[i].task();
            }
        }
    }
}

void schedular_print_task(void){
    uint16_t i = 0;
    for (i = 0; i < TASK_SIZE; i++){
        printf("task_%d addr        : %08X\n", i, (uint16_t)task_list[i].task);
        printf("task_%d interval    : %08X\n", i, (uint16_t)task_list[i].ticks_interval);
        printf("task_%d last        : %08X\n", i, (uint16_t)task_list[i].ticks_previous);
        printf("\n");
    }
}

uint16_t schedular_get_free_task(void){
    uint16_t free_task = 0;
    uint16_t i = 0;
    for(i = 0; i < TASK_SIZE; i++){
        if (task_list[i].task == 0U) free_task++;
    }
    return free_task;
}