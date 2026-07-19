#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LS_COLOR_DEFAULT = 0, LS_COLOR_RED = 31, LS_COLOR_GREEN = 32, LS_COLOR_YELLOW = 33,
    LS_COLOR_BLUE = 34, LS_COLOR_MAGENTA = 35, LS_COLOR_CYAN = 36, LS_COLOR_WHITE = 37
} ls_color_t;

typedef enum {
    LS_TYPE_BLOCKS, LS_TYPE_EQUALS, LS_TYPE_GREATER, LS_TYPE_ASTERISK, LS_TYPE_HYPHEN,
    LS_TYPE_DOTS, LS_TYPE_SMOOTH, LS_TYPE_BRAILLE, LS_TYPE_SPINNER
} ls_type_t;

typedef struct {
    ls_type_t type;
    ls_color_t color;
    int width;
    int x, y;
    int delay_ms;
    int show_percentage;
    const char* label;
    int use_arrow;
    int clear_on_finish;
    int stay_on_line;
} ls_config_t;

int ls_init(void);
void ls_cleanup(void);
void ls_display(const ls_config_t* config, int total_steps);
ls_config_t ls_get_default_config(void);

#ifdef __cplusplus
}
#endif

#endif
