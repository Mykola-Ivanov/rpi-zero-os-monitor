#ifndef CPU_H
#define CPU_H

typedef struct {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
} cpu_info_t;

int cpu_get_info(cpu_info_t *cpu);

double cpu_calculate_usage(
    const cpu_info_t *previous,
    const cpu_info_t *current
);

#endif