#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

/* Flag macros */
#define FLAG_PLUS 1
#define FLAG_SPACE 2
#define FLAG_HASH 4
#define FLAG_ZERO 8
#define FLAG_MINUS 16

/* Length macros */
#define LENGTH_NONE 0
#define LENGTH_L 1
#define LENGTH_H 2

/* Main printf function */
int _printf(const char *format, ...);

/* Buffer management */
int _putchar(char c);
int putchar_buffer(char c);
void flush_buffer(void);

/* Number printing functions */
void print_number(int n, int *count, int flags);
void print_long_number(long n, int *count, int flags);
void print_short_number(short n, int *count, int flags);

/* Conversion specifier functions */
int print_binary(unsigned int n);
int print_unsigned(unsigned int n);
int print_long_unsigned(unsigned long n);
int print_short_unsigned(unsigned short n);
int print_octal(unsigned int n, int flags);
int print_long_octal(unsigned long n, int flags);
int print_short_octal(unsigned short n, int flags);
int print_hex(unsigned int n, int uppercase, int flags);
int print_long_hex(unsigned long n, int uppercase, int flags);
int print_short_hex(unsigned short n, int uppercase, int flags);
int print_custom_string(va_list args);
int print_pointer(va_list args);
int print_hex_long(unsigned long n, int uppercase);

/* Format parsing functions */
int get_flags(const char *format, int *i);
int get_length(const char *format, int *i);
int get_width(const char *format, int *i, va_list args);
int get_precision(const char *format, int *i, va_list args);

/* Width and Precision handling functions */
int print_char_with_width_precision(char c, int width, int precision, int flags);
int print_string_with_width_precision(char *str, int width, int precision, int flags);
int print_percent_with_width(int width, int flags);
int print_unsigned_with_width_precision(unsigned int n, int width, int precision, int flags);
int print_long_unsigned_with_width_precision(unsigned long n, int width, int precision, int flags);
int print_short_unsigned_with_width_precision(unsigned short n, int width, int precision, int flags);
int print_octal_with_width_precision(unsigned int n, int width, int precision, int flags);
int print_long_octal_with_width_precision(unsigned long n, int width, int precision, int flags);
int print_short_octal_with_width_precision(unsigned short n, int width, int precision, int flags);
int print_hex_with_width_precision(unsigned int n, int width, int precision, int flags, int uppercase);
int print_long_hex_with_width_precision(unsigned long n, int width, int precision, int flags, int uppercase);
int print_short_hex_with_width_precision(unsigned short n, int width, int precision, int flags, int uppercase);
int print_binary_with_width(unsigned int n, int width, int flags);
int print_pointer_with_width_precision(void *ptr, int width, int precision, int flags);
int print_custom_string_with_width_precision(va_list args, int width, int precision, int flags);

/* Number functions with width and precision */
void print_number_with_width_precision(int n, int *count, int width, int precision, int flags);
void print_long_number_with_width_precision(long n, int *count, int width, int precision, int flags);
void print_short_number_with_width_precision(short n, int *count, int width, int precision, int flags);

/* Helper functions */
int calculate_number_length(int n, int flags);
int calculate_unsigned_length(unsigned int n, int base);
int calculate_octal_length(unsigned int n, int flags);
int calculate_hex_length(unsigned int n, int flags);
void print_padding(int width, char pad_char, int *count);
void print_zero_padding(int width, int *count);

#endif /* MAIN_H */
