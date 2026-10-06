#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "resources.h"
#include "cpu.h"
#include "display.h"

#define UPDATE_INTERVAL_SECONDS 1


int main(void)
{
    memory_info_t memory;

    cpu_info_t previous_cpu;
    cpu_info_t current_cpu;

    double cpu_percent;


    /*
     * Initialize display.
     *
     * Currently this does nothing because
     * display.c is only a stub.
     */
    if (display_init() != 0) {
        fprintf(stderr,
                "Failed to initialize display\n");

        return EXIT_FAILURE;
    }


    /*
     * CPU utilization requires two measurements.
     *
     * Get the initial measurement before
     * entering the monitoring loop.
     */
    if (cpu_get_info(&previous_cpu) != 0) {
        fprintf(stderr,
                "Failed to read CPU information\n");

        display_deinit();

        return EXIT_FAILURE;
    }


    while (1) {

        /*
         * -------------------------
         * RAM
         * -------------------------
         */

        if (resources_get_memory(&memory) != 0) {
            fprintf(stderr,
                    "Failed to read memory information\n");

            break;
        }


        /*
         * -------------------------
         * CPU
         * -------------------------
         */

        if (cpu_get_info(&current_cpu) != 0) {
            fprintf(stderr,
                    "Failed to read CPU information\n");

            break;
        }

        cpu_percent =
            cpu_calculate_usage(
                &previous_cpu,
                &current_cpu
            );


        /*
         * -------------------------
         * Terminal output
         * -------------------------
         */

        printf(
            "RAM: %lu / %lu MB (%5.1f%%) | "
            "CPU: %5.1f%%\n",

            memory.used_kb / 1024,
            memory.total_kb / 1024,
            memory.used_percent,

            cpu_percent
        );

        fflush(stdout);


        /*
         * -------------------------
         * Display output
         * -------------------------
         */

        display_show_memory(
            memory.used_kb / 1024,
            memory.total_kb / 1024,
            memory.used_percent
        );

        display_show_cpu(cpu_percent);


        /*
         * Current CPU measurement becomes
         * the previous measurement for
         * the next iteration.
         */
        previous_cpu = current_cpu;


        /*
         * Wait before the next measurement.
         */
        sleep(UPDATE_INTERVAL_SECONDS);
    }


    display_deinit();

    return EXIT_SUCCESS;
}