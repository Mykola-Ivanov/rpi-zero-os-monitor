#include <stdio.h>

#include "cpu.h"

int cpu_get_info(cpu_info_t *cpu)
{
    FILE *file;
    int result;

    if (cpu == NULL) {
        return -1;
    }

    file = fopen("/proc/stat", "r");

    if (file == NULL) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    result = fscanf(
        file,
        "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &cpu->user,
        &cpu->nice,
        &cpu->system,
        &cpu->idle,
        &cpu->iowait,
        &cpu->irq,
        &cpu->softirq,
        &cpu->steal
    );

    fclose(file);

    if (result != 8) {
        return -1;
    }

    return 0;
}


double cpu_calculate_usage(
    const cpu_info_t *previous,
    const cpu_info_t *current)
{
    unsigned long long previous_idle;
    unsigned long long current_idle;

    unsigned long long previous_total;
    unsigned long long current_total;

    unsigned long long total_delta;
    unsigned long long idle_delta;

    if (previous == NULL || current == NULL) {
        return 0.0;
    }

    previous_idle =
        previous->idle +
        previous->iowait;

    current_idle =
        current->idle +
        current->iowait;

    previous_total =
        previous->user +
        previous->nice +
        previous->system +
        previous->idle +
        previous->iowait +
        previous->irq +
        previous->softirq +
        previous->steal;

    current_total =
        current->user +
        current->nice +
        current->system +
        current->idle +
        current->iowait +
        current->irq +
        current->softirq +
        current->steal;

    total_delta =
        current_total -
        previous_total;

    idle_delta =
        current_idle -
        previous_idle;

    if (total_delta == 0) {
        return 0.0;
    }

    return 100.0 *
           (double)(total_delta - idle_delta) /
           (double)total_delta;
}