#ifndef MAIN_H
#define MAIN_H

typedef enum {
    VIEW_MENU_MAIN,
    VIEW_MENU_INDIVIDUAL,
    VIEW_RUNNING,
    VIEW_RESULTS,
    VIEW_MAX
} view_t;

typedef enum {
    MENU_CTRL_FULL_BENCHMARK,
    MENU_CTRL_INDIVIDUAL_TESTS,
    MENU_CTRL_RESULTS,
    MENU_CTRL_QUIT,
    MENU_CTRL_MAX
} menu_control_t;

typedef enum {
    MENU_CTRL_INDIVIDUAL_CPU,
    MENU_CTRL_INDIVIDUAL_CPU_ST,
    MENU_CTRL_INDIVIDUAL_MEMORY,
    MENU_CTRL_INDIVIDUAL_MEMORY2,
    MENU_CTRL_INDIVIDUAL_IO,
    MENU_CTRL_INDIVIDUAL_BACK,
    MENU_CTRL_INDIVIDUAL_MAX
} menu_individual_control_t;

#endif
