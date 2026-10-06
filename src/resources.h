#ifndef RESOURCES_H
#define RESOURCES_H

typedef struct {
    unsigned long total_kb;
    unsigned long available_kb;
    unsigned long used_kb;
    double used_percent;
} memory_info_t;

int resources_get_memory(memory_info_t *memory);

#endif