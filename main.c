#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct {
    unsigned long total_kb;
    unsigned long available_kb;
} memory_info_t;

int get_memory_info(memory_info_t *memory)
{
    FILE *file;
    char buffer[BUFFER_SIZE];

    file = fopen("/proc/meminfo", "r");

    if (file == NULL) {
        perror("fopen /proc/meminfo");
        return -1;
    }

    memory->total_kb = 0;
    memory->available_kb = 0;

    while (fgets(buffer, sizeof(buffer), file)) {

        if (sscanf(buffer, "MemTotal: %lu kB",
                   &memory->total_kb) == 1) {
            continue;
        }

        if (sscanf(buffer, "MemAvailable: %lu kB",
                   &memory->available_kb) == 1) {
            continue;
        }
    }

    fclose(file);

    return 0;
}

int main(void)
{
    memory_info_t memory;

    while (1) {

        if (get_memory_info(&memory) != 0) {
            return EXIT_FAILURE;
        }

        unsigned long used_kb =
            memory.total_kb - memory.available_kb;

        double used_percent =
            (double)used_kb / memory.total_kb * 100.0;

        printf(
            "RAM: %lu / %lu MB (%.1f%%)\n",
            used_kb / 1024,
            memory.total_kb / 1024,
            used_percent
        );

        fflush(stdout);

        sleep(1);
    }

    return EXIT_SUCCESS;
}