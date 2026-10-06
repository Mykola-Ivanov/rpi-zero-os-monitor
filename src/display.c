#include "display.h"

int display_init(void)
{
    /*
     * TODO:
     * Initialize TFT and SPI.
     */

    return 0;
}


void display_clear(void)
{
    /*
     * TODO:
     * Clear TFT screen.
     */
}


void display_show_memory(
    unsigned long used_mb,
    unsigned long total_mb,
    double percent)
{
    /*
     * TODO:
     * Draw RAM information on TFT.
     */

    (void)used_mb;
    (void)total_mb;
    (void)percent;
}


void display_show_cpu(double percent)
{
    /*
     * TODO:
     * Draw CPU information on TFT.
     */

    (void)percent;
}


void display_deinit(void)
{
    /*
     * TODO:
     * Close SPI and release TFT resources.
     */
}