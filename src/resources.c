#include <stdio.h>

#include "resources.h"

#define BUFFER_SIZE 256

int resources_get_memory(memory_info_t *memory)
{
    FILE *file;
    char buffer[BUFFER_SIZE];

    if (memory == NULL) {
        return -1;
    }

    file = fopen("/proc/meminfo", "r");

    if (file == NULL) {
        perror("Failed to open /proc/meminfo");
        return -1;
    }

    memory->total_kb = 0;
    memory->available_kb = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        if (sscanf(buffer,
                   "MemTotal: %lu kB",
                   &memory->total_kb) == 1) {
            continue;
        }

        if (sscanf(buffer,
                   "MemAvailable: %lu kB",
                   &memory->available_kb) == 1) {
            continue;
        }
    }

    fclose(file);

    if (memory->total_kb == 0) {
        return -1;
    }

    memory->used_kb =
        memory->total_kb - memory->available_kb;

    memory->used_percent =
        (double)memory->used_kb /
        (double)memory->total_kb *
        100.0;

    return 0;
}