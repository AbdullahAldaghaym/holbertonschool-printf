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
			num = -n;
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
	{
		unsigned int temp = num / 10;
		if (temp / 10)
			print_number(temp, count, 0);
		else
			*count += _putchar(temp + '0');
	}

	*count += _putchar((num % 10) + '0');
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
	int flags;

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
