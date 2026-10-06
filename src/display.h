#ifndef DISPLAY_H
#define DISPLAY_H

int display_init(void);

void display_clear(void);

void display_show_memory(
    unsigned long used_mb,
    unsigned long total_mb,
    double percent
);

void display_show_cpu(double percent);

void display_deinit(void);

#endif