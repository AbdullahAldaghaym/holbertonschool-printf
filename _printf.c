#include "main.h"
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

char output_buf[1024];
int buf_index = 0;

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

void flush_buffer(void)
{
	if (buf_index > 0)
	{
		write(1, output_buf, buf_index);
		buf_index = 0;
	}
}

int _putchar(char c)
{
	return (putchar_buffer(c));
}

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
            flags |= FLAG_SPACE;
            (*i)++;
            found_flag = 1;
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

int get_length(const char *format, int *i)
{
    int length = LENGTH_NONE;

    if (format[*i] == 'l')
    {
        length = LENGTH_L;
        (*i)++;
    }
    else if (format[*i] == 'h')
    {
        length = LENGTH_H;
        (*i)++;
    }

    return length;
}

int get_width(const char *format, int *i, va_list args)
{
    int width = 0;

    if (format[*i] >= '0' && format[*i] <= '9')
    {
        while (format[*i] >= '0' && format[*i] <= '9')
        {
            width = width * 10 + (format[*i] - '0');
            (*i)++;
        }
    }
    else if (format[*i] == '*')
    {
        width = va_arg(args, int);
        (*i)++;
    }

    return width;
}

/* Helper function to calculate number length for width calculation */
int calculate_number_length(int n, int flags)
{
    int length = 0;
    unsigned int num;
    
    if (n == 0)
        return 1;
    
    if (n < 0)
    {
        length++; /* for minus sign */
        num = (n == -2147483648) ? 2147483648U : (unsigned int)(-n);
    }
    else
    {
        num = n;
        if (flags & FLAG_PLUS || flags & FLAG_SPACE)
            length++; /* for plus/space */
    }
    
    while (num > 0)
    {
        length++;
        num /= 10;
    }
    
    return length;
}

int calculate_unsigned_length(unsigned int n, int base)
{
    int length = 0;
    unsigned int num = n;
    
    if (n == 0)
        return 1;
    
    while (num > 0)
    {
        length++;
        num /= base;
    }
    
    return length;
}

int calculate_octal_length(unsigned int n, int flags)
{
    int length = calculate_unsigned_length(n, 8);
    if ((flags & FLAG_HASH) && n != 0)
        length++; /* for the extra '0' */
    return length;
}

int calculate_hex_length(unsigned int n, int flags)
{
    int length = calculate_unsigned_length(n, 16);
    if ((flags & FLAG_HASH) && n != 0)
        length += 2; /* for '0x' or '0X' */
    return length;
}

void print_padding(int width, int *count)
{
    while (width > 0)
    {
        *count += _putchar(' ');
        width--;
    }
}

/* Updated functions with width support */

int print_char_with_width(char c, int width)
{
    int count = 0;

    if (width > 1)
        print_padding(width - 1, &count);

    count += _putchar(c);
    return count;
}

int print_string_with_width(char *str, int width)
{
    int count = 0;
    int len = 0;
    char *temp = str;

    if (str == NULL)
        str = "(null)";

    while (*temp)
    {
        len++;
        temp++;
    }

    if (width > len)
        print_padding(width - len, &count);

    while (*str)
    {
        count += _putchar(*str);
        str++;
    }

    return count;
}

int print_percent_with_width(int width)
{
    int count = 0;

    if (width > 1)
        print_padding(width - 1, &count);

    count += _putchar('%');
    return count;
}

/* Number functions with width support */

void print_number_with_width(int n, int *count, int flags, int width)
{
    char buffer[32];
    int idx = 0, is_negative = 0;
    unsigned int num;
    int total_len;
    int j;

    if (n == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        if (n < 0)
        {
            is_negative = 1;
            if (n == -2147483648)
                num = 2147483648U;
            else
                num = (unsigned int)(-n);
        }
        else
        {
            num = n;
        }

        while (num)
        {
            buffer[idx++] = (num % 10) + '0';
            num /= 10;
        }
    }

    total_len = idx;
    if (is_negative)
        total_len++;
    else if (flags & FLAG_PLUS)
        total_len++;
    else if (flags & FLAG_SPACE)
        total_len++;

    if (width > total_len)
        print_padding(width - total_len, count);

    if (is_negative)
        *count += _putchar('-');
    else if (flags & FLAG_PLUS)
        *count += _putchar('+');
    else if (flags & FLAG_SPACE)
        *count += _putchar(' ');

    j = idx;
    while (j > 0)
    {
        *count += _putchar(buffer[--j]);
    }
}

void print_long_number_with_width(long n, int *count, int flags, int width)
{
    print_number_with_width((int)n, count, flags, width);
}

void print_short_number_with_width(short n, int *count, int flags, int width)
{
    print_number_with_width((int)n, count, flags, width);
}

/* Unsigned functions with width support */

int print_unsigned_with_width(unsigned int n, int width)
{
    int count = 0;
    int len = calculate_unsigned_length(n, 10);

    if (width > len)
        print_padding(width - len, &count);

    count += print_unsigned(n);
    return count;
}

int print_long_unsigned_with_width(unsigned long n, int width)
{
    int count = 0;
    int len = 0;
    unsigned long temp = n;

    if (n == 0)
        len = 1;
    else
    {
        while (temp > 0)
        {
            len++;
            temp /= 10;
        }
    }

    if (width > len)
        print_padding(width - len, &count);

    count += print_long_unsigned(n);
    return count;
}

int print_short_unsigned_with_width(unsigned short n, int width)
{
    return print_unsigned_with_width((unsigned int)n, width);
}

/* Octal functions with width support */

int print_octal_with_width(unsigned int n, int flags, int width)
{
    int count = 0;
    int len = calculate_octal_length(n, flags);

    if (width > len)
        print_padding(width - len, &count);

    count += print_octal(n, flags);
    return count;
}

int print_long_octal_with_width(unsigned long n, int flags, int width)
{
    int count = 0;
    int len = 0;
    unsigned long temp = n;
    int has_hash = (flags & FLAG_HASH) && (n != 0);

    if (n == 0)
        len = 1;
    else
    {
        while (temp > 0)
        {
            len++;
            temp /= 8;
        }
    }
    if (has_hash)
        len++;

    if (width > len)
        print_padding(width - len, &count);

    count += print_long_octal(n, flags);
    return count;
}

int print_short_octal_with_width(unsigned short n, int flags, int width)
{
    return print_octal_with_width((unsigned int)n, flags, width);
}

/* Hex functions with width support */

int print_hex_with_width(unsigned int n, int uppercase, int flags, int width)
{
    int count = 0;
    int len = calculate_hex_length(n, flags);

    if (width > len)
        print_padding(width - len, &count);

    count += print_hex(n, uppercase, flags);
    return count;
}

int print_long_hex_with_width(unsigned long n, int uppercase, int flags, int width)
{
    int count = 0;
    int len = 0;
    unsigned long temp = n;
    int has_hash = (flags & FLAG_HASH) && (n != 0);

    if (n == 0)
        len = 1;
    else
    {
        while (temp > 0)
        {
            len++;
            temp /= 16;
        }
    }
    if (has_hash)
        len += 2;

    if (width > len)
        print_padding(width - len, &count);

    count += print_long_hex(n, uppercase, flags);
    return count;
}

int print_short_hex_with_width(unsigned short n, int uppercase, int flags, int width)
{
    return print_hex_with_width((unsigned int)n, uppercase, flags, width);
}

/* Binary with width support */

int print_binary_with_width(unsigned int n, int width)
{
    int count = 0;
    int len = calculate_unsigned_length(n, 2);

    if (width > len)
        print_padding(width - len, &count);

    count += print_binary(n);
    return count;
}

/* Pointer with width support */

int print_pointer_with_width(va_list args, int width)
{
    int count = 0;
    void *ptr = va_arg(args, void *);
    
    if (ptr == NULL)
    {
        if (width > 5)
            print_padding(width - 5, &count);
        count += _printf("(nil)");
    }
    else
    {
        unsigned long address = (unsigned long)ptr;
        int len = 2; /* for '0x' */
        unsigned long temp = address;
        
        if (address == 0)
            len += 1;
        else
        {
            while (temp > 0)
            {
                len++;
                temp /= 16;
            }
        }
        
        if (width > len)
            print_padding(width - len, &count);
            
        count += _putchar('0');
        count += _putchar('x');
        count += print_hex_long(address, 0);
    }
    
    return count;
}

/* Custom string with width support */

int print_custom_string_with_width(va_list args, int width)
{
    int count = 0;
    char *str = va_arg(args, char *);
    unsigned char c;
    int len = 0;
    char *temp = str;

    if (str == NULL)
        str = "(null)";

    /* Calculate length first */
    while (*temp)
    {
        c = (unsigned char)*temp;
        if (c < 32 || c >= 127)
            len += 4; /* \xXX */
        else
            len++;
        temp++;
    }

    if (width > len)
        print_padding(width - len, &count);

    count += print_custom_string(args);
    return count;
}

/* Keep your existing number printing functions as they are */
/* [All your existing print_number, print_long_number, etc. functions remain here] */
/* [All your existing print_unsigned, print_octal, print_hex, etc. functions remain here] */
/* [All your existing print_custom_string, print_pointer, etc. functions remain here] */

/* Updated _printf function with complete width support */

int _printf(const char *format, ...)
{
	va_list args;
	int i = 0, count = 0;
	int flags, length, width;

	if (format == NULL)
		return (-1);

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

			flags = get_flags(format, &i);
			width = get_width(format, &i, args);
			length = get_length(format, &i);
			
			if (format[i] == '\0')
			{
				flush_buffer();
				va_end(args);
				return (-1);
			}
			
			/* Handle all conversion specifiers with width */
			if (format[i] == 'c')
			{
				count += print_char_with_width(va_arg(args, int), width);
			}
			else if (format[i] == 's')
			{
				char *str = va_arg(args, char *);
				count += print_string_with_width(str, width);
			}
			else if (format[i] == 'S')
			{
				count += print_custom_string_with_width(args, width);
			}
			else if (format[i] == 'd' || format[i] == 'i')
			{
				if (length == LENGTH_L)
					print_long_number_with_width(va_arg(args, long), &count, flags, width);
				else if (length == LENGTH_H)
					print_short_number_with_width((short)va_arg(args, int), &count, flags, width);
				else
					print_number_with_width(va_arg(args, int), &count, flags, width);
			}
			else if (format[i] == 'u')
			{
				if (length == LENGTH_L)
					count += print_long_unsigned_with_width(va_arg(args, unsigned long), width);
				else if (length == LENGTH_H)
					count += print_short_unsigned_with_width((unsigned short)va_arg(args, unsigned int), width);
				else
					count += print_unsigned_with_width(va_arg(args, unsigned int), width);
			}
			else if (format[i] == 'o')
			{
				if (length == LENGTH_L)
					count += print_long_octal_with_width(va_arg(args, unsigned long), flags, width);
				else if (length == LENGTH_H)
					count += print_short_octal_with_width((unsigned short)va_arg(args, unsigned int), flags, width);
				else
					count += print_octal_with_width(va_arg(args, unsigned int), flags, width);
			}
			else if (format[i] == 'x')
			{
				if (length == LENGTH_L)
					count += print_long_hex_with_width(va_arg(args, unsigned long), 0, flags, width);
				else if (length == LENGTH_H)
					count += print_short_hex_with_width((unsigned short)va_arg(args, unsigned int), 0, flags, width);
				else
					count += print_hex_with_width(va_arg(args, unsigned int), 0, flags, width);
			}
			else if (format[i] == 'X')
			{
				if (length == LENGTH_L)
					count += print_long_hex_with_width(va_arg(args, unsigned long), 1, flags, width);
				else if (length == LENGTH_H)
					count += print_short_hex_with_width((unsigned short)va_arg(args, unsigned int), 1, flags, width);
				else
					count += print_hex_with_width(va_arg(args, unsigned int), 1, flags, width);
			}
			else if (format[i] == 'b')
			{
				count += print_binary_with_width(va_arg(args, unsigned int), width);
			}
			else if (format[i] == 'p')
			{
				count += print_pointer_with_width(args, width);
			}
			else if (format[i] == '%')
			{
				count += print_percent_with_width(width);
			}
			else
			{
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
