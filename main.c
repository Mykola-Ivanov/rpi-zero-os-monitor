#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct {
    unsigned long total_kb;
    unsigned long available_kb;
} memory_info_t;

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


/*
 * Read RAM information from /proc/meminfo
 */
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

    return 0;
}


/*
 * Read CPU statistics from /proc/stat
 */
int get_cpu_info(cpu_info_t *cpu)
{
    FILE *file;

    file = fopen("/proc/stat", "r");

    if (file == NULL) {
        perror("fopen /proc/stat");
        return -1;
    }

    int result = fscanf(
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


/*
 * Calculate CPU usage between two measurements
 */
double calculate_cpu_usage(
    const cpu_info_t *previous,
    const cpu_info_t *current)
{
    unsigned long long previous_idle =
        previous->idle + previous->iowait;

    unsigned long long current_idle =
        current->idle + current->iowait;

    unsigned long long previous_total =
        previous->user +
        previous->nice +
        previous->system +
        previous->idle +
        previous->iowait +
        previous->irq +
        previous->softirq +
        previous->steal;

    unsigned long long current_total =
        current->user +
        current->nice +
        current->system +
        current->idle +
        current->iowait +
        current->irq +
        current->softirq +
        current->steal;

    unsigned long long total_delta =
        current_total - previous_total;

    unsigned long long idle_delta =
        current_idle - previous_idle;

    if (total_delta == 0) {
        return 0.0;
    }

    return 100.0 *
           (double)(total_delta - idle_delta) /
           (double)total_delta;
}


int main(void)
{
    memory_info_t memory;

    cpu_info_t previous_cpu;
    cpu_info_t current_cpu;


    /*
     * Get initial CPU measurement.
     *
     * We need two measurements to calculate
     * CPU utilization.
     */
    if (get_cpu_info(&previous_cpu) != 0) {
        fprintf(stderr,
                "Failed to read CPU information\n");

        return EXIT_FAILURE;
    }


    while (1) {

        /*
         * Read RAM
         */
        if (get_memory_info(&memory) != 0) {
            fprintf(stderr,
                    "Failed to read memory information\n");

            return EXIT_FAILURE;
        }


        /*
         * Read current CPU statistics
         */
        if (get_cpu_info(&current_cpu) != 0) {
            fprintf(stderr,
                    "Failed to read CPU information\n");

            return EXIT_FAILURE;
        }


        /*
         * Calculate RAM usage
         */
        unsigned long used_kb =
            memory.total_kb - memory.available_kb;

        double memory_percent =
            (double)used_kb /
            memory.total_kb *
            100.0;


        /*
         * Calculate CPU usage
         */
        double cpu_percent =
            calculate_cpu_usage(
                &previous_cpu,
                &current_cpu
            );


        /*
         * Print results
         */
        printf(
            "RAM: %lu / %lu MB (%.1f%%) | "
            "CPU: %.1f%%\n",

            used_kb / 1024,
            memory.total_kb / 1024,
            memory_percent,

            cpu_percent
        );

        fflush(stdout);


        /*
         * Current CPU measurement becomes
         * the previous measurement for the
         * next iteration.
         */
        previous_cpu = current_cpu;


        /*
         * Wait one second before next measurement.
         */
        sleep(1);
    }

    return EXIT_SUCCESS;
}