#if !defined(_WIN32)
#define _POSIX_C_SOURCE 199309L
#endif

#include "../include/loading_screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif

static int g_initialized = 0;
#ifdef _WIN32
static HANDLE g_stdout_handle = INVALID_HANDLE_VALUE;
static DWORD g_orig_out_mode = 0;
#endif

int ls_init(void) {
    if (g_initialized) return 0;
#ifdef _WIN32
    g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (g_stdout_handle != INVALID_HANDLE_VALUE && GetConsoleMode(g_stdout_handle, &g_orig_out_mode)) {
        SetConsoleMode(g_stdout_handle, g_orig_out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleOutputCP(CP_UTF8);
    }
#endif
    g_initialized = 1;
    return 0;
}

void ls_cleanup(void) {
    if (!g_initialized) return;
    printf("\033[0m\033[?25h");
    fflush(stdout);
#ifdef _WIN32
    if (g_stdout_handle != INVALID_HANDLE_VALUE) SetConsoleMode(g_stdout_handle, g_orig_out_mode);
#endif
    g_initialized = 0;
}

static void set_cursor(int x, int y) {
    if (x > 0 || y > 0) printf("\033[%d;%dH", y > 0 ? y : 1, x > 0 ? x : 1);
    else printf("\r");
}

static void delay_ms(int ms) {
    if (ms <= 0) return;
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    struct timespec ts = {ms / 1000, (ms % 1000) * 1000000L};
    nanosleep(&ts, NULL);
#endif
}

ls_config_t ls_get_default_config(void) {
    ls_config_t config = {LS_TYPE_BLOCKS, LS_COLOR_CYAN, 40, 0, 0, 50, 1, "Loading", 0, 0, 0};
    return config;
}

void ls_display(const ls_config_t* config, int total_steps) {
    if (!g_initialized) ls_init();
    if (total_steps <= 0) return;

    printf("\033[?25l");
    if (config->type == LS_TYPE_SPINNER) {
        const char* frames[] = {"|", "/", "-", "\\"};
        for (int i = 0; i < total_steps; i++) {
            set_cursor(config->x, config->y);
            if (config->label) printf("%s ", config->label);
            printf("\033[1;%dm%s\033[0m", config->color, frames[i % 4]);
            fflush(stdout);
            delay_ms(config->delay_ms);
        }
    } else {
        const char* fills[] = {"#", "=", ">", "*", "-", "."};
        const char* smooth[] = {" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█"};
        for (int i = 0; i <= total_steps; i++) {
            set_cursor(config->x, config->y);
            if (config->label) printf("%s ", config->label);
            printf("[");
            double progress = (double)i / total_steps;
            double filled_f = progress * config->width;
            int filled_len = (int)filled_f;
            printf("\033[0;%dm", config->color);
            if (config->type == LS_TYPE_SMOOTH) {
                for (int j = 0; j < filled_len; j++) printf("█");
                if (filled_len < config->width) {
                    printf("%s", smooth[(int)((filled_f - filled_len) * 8)]);
                    filled_len++;
                }
            } else if (config->type == LS_TYPE_BRAILLE) {
                for (int j = 0; j < filled_len; j++) printf("⣿");
            } else {
                const char* f = (config->type <= LS_TYPE_DOTS) ? fills[config->type] : "#";
                for (int j = 0; j < filled_len; j++)
                    printf("%s", (config->use_arrow && j == filled_len - 1 && i < total_steps) ? ">" : f);
            }
            printf("\033[0m");
            for (int j = filled_len; j < config->width; j++) printf(" ");
            printf("]");
            if (config->show_percentage) printf(" %3d%%", (int)(progress * 100));
            fflush(stdout);
            if (i < total_steps) delay_ms(config->delay_ms);
        }
    }
    if (config->clear_on_finish) {
        set_cursor(config->x, config->y);
        printf("\033[2K\r");
    } else if (!config->stay_on_line) printf("\n");
    printf("\033[?25h");
    fflush(stdout);
}
