#include <stdio.h>

#include "temperature.h"

#define CPU_TEMPERATURE_PATH \
    "/sys/class/thermal/thermal_zone0/temp"


int temperature_get_cpu(double *temperature_c)
{
    FILE *file;
    long temperature_millidegrees;

    if (temperature_c == NULL) {
        return -1;
    }

    file = fopen(CPU_TEMPERATURE_PATH, "r");

    if (file == NULL) {
        perror("Failed to open CPU temperature");
        return -1;
    }

    if (fscanf(file, "%ld", &temperature_millidegrees) != 1) {

        fclose(file);

        return -1;
    }

    fclose(file);

    /*
     * Linux reports temperature in millidegrees Celsius.
     *
     * Example:
     *
     * 48234 -> 48.234 °C
     */
    *temperature_c =
        (double)temperature_millidegrees / 1000.0;

    return 0;
}