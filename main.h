#include <stdarg.h>

#define FLAG_PLUS 1
#define FLAG_SPACE 2
#define FLAG_HASH 4

int _printf(const char *format, ...);
int _putchar(char c);
void print_number(int n, int *count, int flags);
int print_binary(unsigned int n);
int print_unsigned(unsigned int n);
int print_octal(unsigned int n, int flags);
int print_hex(unsigned int n, int uppercase, int flags);
int putchar_buffer(char c);
void flush_buffer(void);
int print_custom_string(va_list args);
int print_pointer(va_list args);
int print_hex_long(unsigned long n, int uppercase);
int get_flags(const char *format, int *i);

#endif
