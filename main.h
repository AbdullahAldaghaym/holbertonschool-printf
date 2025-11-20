#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

#define FLAG_PLUS 1
#define FLAG_SPACE 2
#define FLAG_HASH 4
#define LENGTH_NONE 0
#define LENGTH_L 1
#define LENGTH_H 2

int _printf(const char *format, ...);
int _putchar(char c);
void print_number(int n, int *count, int flags);
void print_long_number(long n, int *count, int flags);
void print_short_number(short n, int *count, int flags);
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
int putchar_buffer(char c);
void flush_buffer(void);
int print_custom_string(va_list args);
int print_pointer(va_list args);
int print_hex_long(unsigned long n, int uppercase);
int get_flags(const char *format, int *i);
int get_length(const char *format, int *i);

#endif
