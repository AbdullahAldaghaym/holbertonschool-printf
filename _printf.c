#include "main.h"
#include <stdarg.h>
#include <unistd.h>

/* Buffer Variables */
char output_buf[1024];
int buf_index = 0;

/**
 * putchar_buffer - adds character to buffer and flushes when full
 * @c: character to add
 * Return: 1 on success
 */
int putchar_buffer(char c)
{
	output_buf[buf_index++] = c;

	if (buf_index == 1024)
	{
		write(1, output_buf, buf_index);
		buf_index = 0;
	}

	return (1);
}

/**
 * flush_buffer - writes remaining buffer to stdout
 */
void flush_buffer(void)
{
	if (buf_index > 0)
	{
		write(1, output_buf, buf_index);
		buf_index = 0;
	}
}

/**
 * _putchar - writes the character c to stdout using buffer
 * @c: The character to print
 * Return: On success 1.
 */
int _putchar(char c)
{
	return (putchar_buffer(c));
}

/**
 * get_flags - extracts flag characters from format string
 * @format: format string
 * @i: pointer to current index in format string
 * Return: bitmask of flags found
 */
int get_flags(const char *format, int *i)
{
    int flags = 0;
    int found_flag = 1;

    while (found_flag)
    {
        found_flag = 0;
        
        if (format[*i] == '+')
        {
            flags |= FLAG_PLUS;
            (*i)++;
            found_flag = 1;
        }
        else if (format[*i] == ' ')
        {
            /* تأكد أن space flag يكون فقط قبل numeric specifiers */
            if (format[*i + 1] == 'd' || format[*i + 1] == 'i' || 
                format[*i + 1] == 'o' || format[*i + 1] == 'x' || 
                format[*i + 1] == 'X' || format[*i + 1] == 'u')
            {
                flags |= FLAG_SPACE;
                (*i)++;
                found_flag = 1;
            }
            else
            {
                /* إذا لم يكن متبوعاً بـ numeric specifier، توقف */
                break;
            }
        }
        else if (format[*i] == '#')
        {
            flags |= FLAG_HASH;
            (*i)++;
            found_flag = 1;
        }
    }

    return flags;
}

/**
 * handle_flags - handles flag characters for numeric conversions
 * @flags: bitmask of active flags
 * @is_negative: 1 if number is negative, 0 otherwise
 * @count: pointer to character counter
 * @specifier: conversion specifier character
 */
void handle_flags(int flags, int is_negative, int *count, int specifier)
{
    /* Handle + and space flags for signed numbers */
    if (specifier == 'd' || specifier == 'i')
    {
        if (!is_negative)
        {
            if (flags & FLAG_PLUS)
            {
                *count += _putchar('+');
            }
            else if (flags & FLAG_SPACE)
            {
                *count += _putchar(' ');
            }
        }
    }
    
    /* Handle # flag for octal and hexadecimal */
    if (flags & FLAG_HASH)
    {
        if (specifier == 'o')
        {
            *count += _putchar('0');
        }
        else if (specifier == 'x')
        {
            *count += _putchar('0');
            *count += _putchar('x');
        }
        else if (specifier == 'X')
        {
            *count += _putchar('0');
            *count += _putchar('X');
        }
    }
}

/**
 * print_number_int - helper for integer printing (without flags)
 * @num: number to print
 * @count: pointer to character counter
 */
void print_number_int(unsigned int num, int *count)
{
    if (num / 10)
        print_number_int(num / 10, count);

    *count += _putchar((num % 10) + '0');
}

/**
 * print_number - prints an integer number with flags
 * @n: number to print
 * @count: pointer to character counter
 * @flags: bitmask of flags
 */
void print_number(int n, int *count, int flags)
{
    unsigned int num;
    int is_negative = 0;

    /* Handle zero case first */
    if (n == 0)
    {
        /* For zero, only handle + and space flags */
        if (flags & FLAG_PLUS)
        {
            *count += _putchar('+');
        }
        else if (flags & FLAG_SPACE)
        {
            *count += _putchar(' ');
        }
        *count += _putchar('0');
        return;
    }

    if (n < 0)
    {
        is_negative = 1;
        num = -n;
    }
    else
    {
        num = n;
    }

    /* Handle flags before printing number (only for non-negative) */
    if (!is_negative)
    {
        if (flags & FLAG_PLUS)
        {
            *count += _putchar('+');
        }
        else if (flags & FLAG_SPACE)
        {
            *count += _putchar(' ');
        }
    }

    if (is_negative)
    {
        *count += _putchar('-');
    }

    if (num / 10)
        print_number_int(num / 10, count);

    *count += _putchar((num % 10) + '0');
}

/**
 * print_binary - prints a number in binary
 * @n: number to print
 * Return: number of characters printed
 */
int print_binary(unsigned int n)
{
    int count = 0;

    if (n == 0)
        return _putchar('0');

    if (n / 2)
        count += print_binary(n / 2);

    count += _putchar((n % 2) + '0');

    return count;
}

/**
 * print_unsigned - prints an unsigned integer
 * @n: number to print
 * Return: number of characters printed
 */
int print_unsigned(unsigned int n)
{
    int count = 0;

    if (n == 0)
        return _putchar('0');

    if (n / 10)
        count += print_unsigned(n / 10);

    count += _putchar((n % 10) + '0');

    return count;
}

/**
 * print_octal_recursive - helper for octal printing
 * @n: number to print
 * Return: number of characters printed
 */
int print_octal_recursive(unsigned int n)
{
    int count = 0;

    if (n / 8)
        count += print_octal_recursive(n / 8);

    count += _putchar((n % 8) + '0');

    return count;
}

/**
 * print_octal - prints a number in octal with flags
 * @n: number to print
 * @flags: bitmask of flags
 * Return: number of characters printed
 */
int print_octal(unsigned int n, int flags)
{
    int count = 0;
    int has_hash = (flags & FLAG_HASH) && (n != 0);

    if (n == 0)
    {
        /* For zero, just print 0 without hash prefix */
        count += _putchar('0');
        return count;
    }

    if (has_hash)
        count += _putchar('0');

    if (n / 8)
        count += print_octal_recursive(n / 8);

    count += _putchar((n % 8) + '0');

    return count;
}

/**
 * print_hex_recursive - helper for hexadecimal printing
 * @n: number to print
 * @uppercase: 1 for uppercase, 0 for lowercase
 * Return: number of characters printed
 */
int print_hex_recursive(unsigned int n, int uppercase)
{
    int count = 0;
    char *digits;

    if (uppercase)
        digits = "0123456789ABCDEF";
    else
        digits = "0123456789abcdef";

    if (n / 16)
        count += print_hex_recursive(n / 16, uppercase);

    count += _putchar(digits[n % 16]);

    return count;
}

/**
 * print_hex - prints a number in hexadecimal with flags
 * @n: number to print
 * @uppercase: 1 for uppercase, 0 for lowercase
 * @flags: bitmask of flags
 * Return: number of characters printed
 */
int print_hex(unsigned int n, int uppercase, int flags)
{
    int count = 0;
    char *digits;
    int has_hash = (flags & FLAG_HASH) && (n != 0);

    if (uppercase)
        digits = "0123456789ABCDEF";
    else
        digits = "0123456789abcdef";

    if (n == 0)
    {
        /* For zero, just print 0 without hash prefix */
        count += _putchar('0');
        return count;
    }

    if (has_hash)
    {
        count += _putchar('0');
        count += _putchar(uppercase ? 'X' : 'x');
    }

    if (n / 16)
        count += print_hex_recursive(n / 16, uppercase);

    count += _putchar(digits[n % 16]);

    return count;
}

/**
 * print_hex_long - prints a long unsigned int in hexadecimal
 * @n: number to print
 * @uppercase: 1 for uppercase, 0 for lowercase
 * Return: number of characters printed
 */
int print_hex_long(unsigned long n, int uppercase)
{
    int count = 0;
    char *digits;

    if (uppercase)
        digits = "0123456789ABCDEF";
    else
        digits = "0123456789abcdef";

    if (n / 16)
        count += print_hex_long(n / 16, uppercase);

    count += _putchar(digits[n % 16]);

    return count;
}

/**
 * print_custom_string - prints string with special handling for non-printable chars
 * @args: va_list arguments
 * Return: number of characters printed
 */
int print_custom_string(va_list args)
{
    char *str = va_arg(args, char *);
    int count = 0;
    unsigned char c;

    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        c = (unsigned char)*str;
        
        if (c < 32 || c >= 127)
        {
            count += _putchar('\\');
            count += _putchar('x');
            
            if (c / 16 < 10)
                count += _putchar((c / 16) + '0');
            else
                count += _putchar((c / 16) - 10 + 'A');
            
            if (c % 16 < 10)
                count += _putchar((c % 16) + '0');
            else
                count += _putchar((c % 16) - 10 + 'A');
        }
        else
        {
            count += _putchar(c);
        }
        str++;
    }

    return count;
}

/**
 * print_pointer - prints a pointer address
 * @args: va_list arguments
 * Return: number of characters printed
 */
int print_pointer(va_list args)
{
    void *ptr = va_arg(args, void *);
    unsigned long address = (unsigned long)ptr;
    int count = 0;
    
    if (ptr == NULL)
    {
        return (_printf("(nil)"));
    }
    
    /* Print "0x" prefix */
    count += _putchar('0');
    count += _putchar('x');
    
    /* Print hexadecimal address */
    count += print_hex_long(address, 0);
    
    return count;
}

/**
 * _printf - produces output according to a format
 * @format: character string containing directives
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
    va_list args;
    int i = 0, count = 0;
    char *str;
    int flags;

    if (format == NULL)
        return (-1);

    /* Reset buffer at start */
    buf_index = 0;

    va_start(args, format);

    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '\0')
            {
                flush_buffer();
                va_end(args);
                return (-1);
            }

            /* Extract flags فقط إذا كان الحرف flag صالح */
            flags = get_flags(format, &i);
            
            /* إذا كان الحرف الحالي ليس specifier صالح بعد الـ flags، عالج كحالة خاصة */
            if (format[i] == 'c')
            {
                count += _putchar(va_arg(args, int));
            }
            else if (format[i] == 's')
            {
                str = va_arg(args, char *);
                if (str == NULL)
                    str = "(null)";
                while (str[0] != '\0')
                {
                    count += _putchar(str[0]);
                    str++;
                }
            }
            else if (format[i] == 'S')
            {
                count += print_custom_string(args);
            }
            else if (format[i] == 'd' || format[i] == 'i')
            {
                print_number(va_arg(args, int), &count, flags);
            }
            else if (format[i] == 'b')
            {
                count += print_binary(va_arg(args, unsigned int));
            }
            else if (format[i] == 'u')
            {
                count += print_unsigned(va_arg(args, unsigned int));
            }
            else if (format[i] == 'o')
            {
                count += print_octal(va_arg(args, unsigned int), flags);
            }
            else if (format[i] == 'x')
            {
                count += print_hex(va_arg(args, unsigned int), 0, flags);
            }
            else if (format[i] == 'X')
            {
                count += print_hex(va_arg(args, unsigned int), 1, flags);
            }
            else if (format[i] == 'p')
            {
                count += print_pointer(args);
            }
            else if (format[i] == '%')
            {
                count += _putchar('%');
            }
            else
            {
                /* إذا لم يكن specifier معروف، اطبع % والحرف */
                count += _putchar('%');
                count += _putchar(format[i]);
            }
        }
        else
        {
            count += _putchar(format[i]);
        }
        i++;
    }

    flush_buffer();
    va_end(args);
    return (count);
}
