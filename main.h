#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

/* Flag macros */
#define FLAG_PLUS 1
#define FLAG_SPACE 2
#define FLAG_HASH 4

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

/* Width handling functions */
int print_char_with_width(char c, int width);
int print_string_with_width(char *str, int width);
int print_percent_with_width(int width);
int print_unsigned_with_width(unsigned int n, int width);
int print_long_unsigned_with_width(unsigned long n, int width);
int print_short_unsigned_with_width(unsigned short n, int width);
int print_octal_with_width(unsigned int n, int flags, int width);
int print_long_octal_with_width(unsigned long n, int flags, int width);
int print_short_octal_with_width(unsigned short n, int flags, int width);
int print_hex_with_width(unsigned int n, int uppercase, int flags, int width);
int print_long_hex_with_width(unsigned long n, int uppercase, int flags, int width);
int print_short_hex_with_width(unsigned short n, int uppercase, int flags, int width);
int print_binary_with_width(unsigned int n, int width);
int print_pointer_with_width(va_list args, int width);
int print_custom_string_with_width(va_list args, int width);

/* Width calculation helper functions */
int calculate_number_length(int n, int flags);
int calculate_unsigned_length(unsigned int n, int base);
int calculate_octal_length(unsigned int n, int flags);
int calculate_hex_length(unsigned int n, int flags);

/* Width printing helper functions */
void print_number_with_width(int n, int *count, int flags, int width);
void print_long_number_with_width(long n, int *count, int flags, int width);
void print_short_number_with_width(short n, int *count, int flags, int width);
void print_padding(int width, int *count);

#endif /* MAIN_H */
