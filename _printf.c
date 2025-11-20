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

void print_padding(int width, int *count)
{
    while (width > 0)
    {
        *count += _putchar(' ');
        width--;
    }
}

int print_string_with_width(char *str, int width)
{
    int count = 0;
    int len = 0;
    char *temp = str;

    if (str == NULL)
        str = "(null)";

    while (*temp++)
        len++;

    if (width > len)
        print_padding(width - len, &count);

    while (*str)
        count += _putchar(*str++);

    return count;
}

int print_char_with_width(char c, int width)
{
    int count = 0;

    if (width > 1)
        print_padding(width - 1, &count);

    count += _putchar(c);
    return count;
}

void print_number_int(unsigned int num, int *count)
{
    if (num / 10)
        print_number_int(num / 10, count);
    *count += _putchar((num % 10) + '0');
}

void print_long_number_int(unsigned long num, int *count)
{
    if (num / 10)
        print_long_number_int(num / 10, count);
    *count += _putchar((num % 10) + '0');
}

void print_number_with_width(int n, int *count, int flags, int width)
{
    char buffer[32];
    int idx = 0, is_negative = 0;
    unsigned int num;

    if (n == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        if (n < 0)
        {
            is_negative = 1;
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

    int total_len = idx;
    if (is_negative || (flags & FLAG_PLUS) || (flags & FLAG_SPACE))
        total_len++;

    if (width > total_len)
        print_padding(width - total_len, count);

    if (is_negative)
        *count += _putchar('-');
    else if (flags & FLAG_PLUS)
        *count += _putchar('+');
    else if (flags & FLAG_SPACE)
        *count += _putchar(' ');

    while (idx > 0)
        *count += _putchar(buffer[--idx]);
}

void print_long_number_with_width(long n, int *count, int flags, int width)
{
    print_number_with_width((int)n, count, flags, width);
}

void print_short_number_with_width(short n, int *count, int flags, int width)
{
    print_number_with_width((int)n, count, flags, width);
}

void print_number(int n, int *count, int flags)
{
	unsigned int num;
	int is_negative = 0;

	if (n == 0)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
		*count += _putchar('0');
		return;
	}

	if (n < 0)
	{
		is_negative = 1;
		if (n == INT_MIN)
			num = 2147483648U;
		else
			num = (unsigned int)(-n);
	}
	else
	{
		num = n;
	}

	if (!is_negative)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
	}

	if (is_negative)
		*count += _putchar('-');

	if (num / 10)
		print_number_int(num / 10, count);

	*count += _putchar((num % 10) + '0');
}

void print_long_number(long n, int *count, int flags)
{
	unsigned long num;
	int is_negative = 0;

	if (n == 0)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
		*count += _putchar('0');
		return;
	}

	if (n < 0)
	{
		is_negative = 1;
		if (n == LONG_MIN)
			num = (unsigned long)LONG_MAX + 1;
		else
			num = (unsigned long)(-n);
	}
	else
	{
		num = n;
	}

	if (!is_negative)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
	}

	if (is_negative)
		*count += _putchar('-');

	if (num / 10)
		print_long_number_int(num / 10, count);

	*count += _putchar((num % 10) + '0');
}

void print_short_number(short n, int *count, int flags)
{
	unsigned short num;
	int is_negative = 0;

	if (n == 0)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
		*count += _putchar('0');
		return;
	}

	if (n < 0)
	{
		is_negative = 1;
		num = (unsigned short)(-n);
	}
	else
	{
		num = n;
	}

	if (!is_negative)
	{
		if (flags & FLAG_PLUS)
			*count += _putchar('+');
		else if (flags & FLAG_SPACE)
			*count += _putchar(' ');
	}

	if (is_negative)
		*count += _putchar('-');

	if (num / 10)
		print_number_int(num / 10, count);

	*count += _putchar((num % 10) + '0');
}

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

int print_long_unsigned(unsigned long n)
{
	int count = 0;

	if (n == 0)
		return _putchar('0');

	if (n / 10)
		count += print_long_unsigned(n / 10);

	count += _putchar((n % 10) + '0');

	return count;
}

int print_short_unsigned(unsigned short n)
{
	int count = 0;

	if (n == 0)
		return _putchar('0');

	if (n / 10)
		count += print_unsigned(n / 10);

	count += _putchar((n % 10) + '0');

	return count;
}

int print_octal(unsigned int n, int flags)
{
	int count = 0;
	int has_hash = (flags & FLAG_HASH) && (n != 0);

	if (n == 0)
	{
		count += _putchar('0');
		return count;
	}

	if (has_hash)
		count += _putchar('0');

	if (n / 8)
		count += print_octal(n / 8, 0);

	count += _putchar((n % 8) + '0');

	return count;
}

int print_long_octal(unsigned long n, int flags)
{
	int count = 0;
	int has_hash = (flags & FLAG_HASH) && (n != 0);

	if (n == 0)
	{
		count += _putchar('0');
		return count;
	}

	if (has_hash)
		count += _putchar('0');

	if (n / 8)
		count += print_long_octal(n / 8, 0);

	count += _putchar((n % 8) + '0');

	return count;
}

int print_short_octal(unsigned short n, int flags)
{
	int count = 0;
	int has_hash = (flags & FLAG_HASH) && (n != 0);

	if (n == 0)
	{
		count += _putchar('0');
		return count;
	}

	if (has_hash)
		count += _putchar('0');

	if (n / 8)
		count += print_octal(n / 8, 0);

	count += _putchar((n % 8) + '0');

	return count;
}

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
		count += _putchar('0');
		return count;
	}

	if (has_hash)
	{
		count += _putchar('0');
		count += _putchar(uppercase ? 'X' : 'x');
	}

	if (n / 16)
		count += print_hex(n / 16, uppercase, 0);

	count += _putchar(digits[n % 16]);

	return count;
}

int print_long_hex(unsigned long n, int uppercase, int flags)
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
		count += _putchar('0');
		return count;
	}

	if (has_hash)
	{
		count += _putchar('0');
		count += _putchar(uppercase ? 'X' : 'x');
	}

	if (n / 16)
		count += print_long_hex(n / 16, uppercase, 0);

	count += _putchar(digits[n % 16]);

	return count;
}

int print_short_hex(unsigned short n, int uppercase, int flags)
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
		count += _putchar('0');
		return count;
	}

	if (has_hash)
	{
		count += _putchar('0');
		count += _putchar(uppercase ? 'X' : 'x');
	}

	if (n / 16)
		count += print_hex(n / 16, uppercase, 0);

	count += _putchar(digits[n % 16]);

	return count;
}

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

int print_pointer(va_list args)
{
    void *ptr = va_arg(args, void *);
    unsigned long address = (unsigned long)ptr;
    int count = 0;
    
    if (ptr == NULL)
    {
        return (_printf("(nil)"));
    }
    
    count += _putchar('0');
    count += _putchar('x');
    
    count += print_hex_long(address, 0);
    
    return count;
}

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

int _printf(const char *format, ...)
{
	va_list args;
	int i = 0, count = 0;
	char *str;
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
			
			if (format[i] == 'c')
			{
				count += print_char_with_width(va_arg(args, int), width);
			}
			else if (format[i] == 's')
			{
				str = va_arg(args, char *);
				count += print_string_with_width(str, width);
			}
			else if (format[i] == 'S')
			{
				count += print_custom_string(args);
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
					count += print_long_unsigned(va_arg(args, unsigned long));
				else if (length == LENGTH_H)
					count += print_short_unsigned((unsigned short)va_arg(args, unsigned int));
				else
					count += print_unsigned(va_arg(args, unsigned int));
			}
			else if (format[i] == 'o')
			{
				if (length == LENGTH_L)
					count += print_long_octal(va_arg(args, unsigned long), flags);
				else if (length == LENGTH_H)
					count += print_short_octal((unsigned short)va_arg(args, unsigned int), flags);
				else
					count += print_octal(va_arg(args, unsigned int), flags);
			}
			else if (format[i] == 'x')
			{
				if (length == LENGTH_L)
					count += print_long_hex(va_arg(args, unsigned long), 0, flags);
				else if (length == LENGTH_H)
					count += print_short_hex((unsigned short)va_arg(args, unsigned int), 0, flags);
				else
					count += print_hex(va_arg(args, unsigned int), 0, flags);
			}
			else if (format[i] == 'X')
			{
				if (length == LENGTH_L)
					count += print_long_hex(va_arg(args, unsigned long), 1, flags);
				else if (length == LENGTH_H)
					count += print_short_hex((unsigned short)va_arg(args, unsigned int), 1, flags);
				else
					count += print_hex(va_arg(args, unsigned int), 1, flags);
			}
			else if (format[i] == 'b')
			{
				count += print_binary(va_arg(args, unsigned int));
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
