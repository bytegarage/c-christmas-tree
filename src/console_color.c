/*
* CUTX_C - ByteGarage CONSOLE UTIL LIBRARY - COLORS
* Author: Byte Garage
*
* @reference: 
* @link: https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
* @link: https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
*
*/
#ifndef __CUTX_C__
#define __CUTX_C__

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

// override the puts standard behaviour to skip the newline
#define __puts(s) fputs(s, stdout)

typedef enum 
{
    color_current_color = -1,
    color_black = 30,
    color_red,
    color_green,
    color_yellow,
    color_blue,
    color_purple,
    color_cyan,
    color_white,
    color_bright_black = 90,
    color_bright_red,
    color_bright_green,
    color_bright_yellow,
    color_bright_blue,
    color_bright_purple,
    color_bright_cyan,
    color_bright_white
} console_color;

typedef enum 
{
    color_normal = 0x00,              // color + 30
    color_bold = 0x01,                // "\033[1m"
    color_italic = 0x02,              // "\033[3m"
    color_underline = 0x04,           // "\033[4m"
    color_double_underline = 0x08,    // "\033[21m"
    color_strikeout = 0x10,           // "\033[9m"
    color_bright = 0x20,              // color + 90
} console_format;

typedef struct {
    int color;
    int background;
    bool bold;
    bool italic;
    bool underline;
    bool double_underline;
    bool strikeout;
} style_info;

style_info __current_style;
style_info __default_style;

// RGB Color Patter
#define RGB_FOREGROUND_PATTERN "\033[38;2;%d;%d;%dm" 
#define RGB_BACKGROUND_PATTERN "\033[48;2;%d;%d;%dm"

int __set_color(console_color c)
{
    console_color prev_color = __current_style.color;

    if (c >= 0)
    {
        __current_style.color = c;
        if(c < 0x0F000000) {
           printf("\033[%dm", c);
        }
        else {
            int r = (c & 0x00FF0000) >> 16;
            int g = (c & 0x0000FF00) >> 8;
            int b = (c & 0x000000FF);
            printf(RGB_FOREGROUND_PATTERN, r, g, b);        
        }
    }

    return prev_color;
}

int set_color(console_color c) {
    return __set_color(c);
}

int set_color_rgb(u_int8_t r, u_int8_t g, u_int8_t b) {
    return __set_color(0x0F000000 | r << 16 | g << 8 | b);
}

int __set_background(console_color c)
{
    console_color prev_color = __current_style.background;

    if (c >= 0)
    {
        __current_style.background = c;
        if(c < 0x0F000000) {
           printf("\033[%dm", c + 10);
        }
        else {
            int r = (c & 0x00FF0000) >> 16;
            int g = (c & 0x0000FF00) >> 8;
            int b = (c & 0x000000FF);
            printf(RGB_BACKGROUND_PATTERN, r, g, b);        
        }
    }

    return prev_color;
}

int set_background(console_color c)
{
    return __set_background(c);
}

int set_background_rgb(int r, int g, int b) {
    return __set_background(0x0F000000 | r << 16 | g << 8 | b);
}



bool __default_style_initialized = false;
style_info get_default_style() {
    return __default_style;
}

style_info set_default_style(style_info sinfo) {
    __default_style_initialized = true;
    return __default_style = sinfo;
}

style_info get_style() {
    return __current_style;
}

void __reset_style() {
    __puts("\033[0m\033[0K");
}

style_info set_style(style_info finfo)
{
    style_info previous_style = __current_style;

    __reset_style();
    if (finfo.bold)
        __puts("\033[1m");
    if (finfo.italic)           
        __puts("\033[2m");
    if (finfo.underline)        
        __puts("\033[4m");
    if (finfo.double_underline) 
        __puts("\033[21m");
    if (finfo.strikeout)        
        __puts("\033[9m");

    __current_style = finfo;

    set_color(finfo.color);
    set_background(finfo.background);

    return previous_style;
}

void reset_style(bool hold_current_colors)
{
    __reset_style();
    __current_style.bold = false;
    __current_style.italic = false;
    __current_style.underline = false;
    __current_style.double_underline = false;
    __current_style.strikeout = false;

    if (hold_current_colors) {
        set_color(__current_style.color);
        set_background(__current_style.background);
    }
    else {
        __current_style.color = -1;
        __current_style.background = -1;
    }
}

style_info set_format(console_format format)
{
    style_info sinfo = __current_style;
    //char *snrml = (format & color_normal || format == NULL) ? "\033[0m" : "";
    sinfo.bold = format & color_bold;
    sinfo.italic = format & color_italic;
    sinfo.underline = format & color_underline;
    sinfo.double_underline = format & color_double_underline;
    sinfo.strikeout = format & color_strikeout;
    sinfo.color = __current_style.color;
    sinfo.background = __current_style.background;

    return set_style(sinfo);
}

int vcprintf(console_color foreground, console_color background, console_format format,
 const char *string_format, va_list ap)
{
    style_info sinfo = set_format(format);
    sinfo.color = set_color(foreground);
    sinfo.background = set_background(background);
    int res = vprintf(string_format, ap);
    set_style(sinfo);
    return res;
}

int cprintf(console_color foreground, console_color background, console_format format,
 const char *string_format, ...)
{
    va_list ap;
    va_start(ap, string_format);
    int res = vcprintf(foreground, background, format, string_format, ap);
    va_end(ap);
    return res;
}

int vcprintln(console_color foreground, console_color background, console_format format,
 const char *string_format, va_list ap)
{
    style_info sinfo = set_format(format);
    set_color(foreground);
    set_background(background);
    int res = vprintf(string_format, ap);
    puts("\033[0K");
    set_style(sinfo);
    return res;
}

int cprintln(console_color foreground, console_color background, console_format format,
 const char *string_format, ...)
{
    va_list ap;
    va_start(ap, string_format);
    int res = vcprintln(foreground, background, format, string_format, ap);
    va_end(ap);
    return res;
}

/* --------------------------------------------------------
* Screen functions
* -------------------------------------------------------- */
#define CLEAR_SCREEN "\033[1J"
#define GOTO_ROW_COL "\033[%d;%dH"
void clrscr() {
    __puts(CLEAR_SCREEN);
}

void gotoxy(int row, int col) {
    fprintf(stdout, GOTO_ROW_COL, row, col);

}

/* --------------------------------------------------------
* Console Colorized output helper
* -------------------------------------------------------- */
#define __c_println(foreground, background, format, string_format) \
    va_list ap;                                                   \
    va_start(ap, string_format);                                  \
    vcprintln(foreground, background, format, string_format, ap);  \
    va_end(ap);

void printf_title(const char* format, ...) {
    __c_println(color_black, color_bright_white, color_bold, format);
}

void printf_error(const char* format, ...) {
    __c_println(color_red, -1, color_bold, format);
}

void printf_warning(const char* format, ...) {
    __c_println(color_yellow, -1, color_bold, format);
}

void printf_ok(const char* format, ...) {
    __c_println(color_bright_green, -1, color_bold, format);
}


#endif