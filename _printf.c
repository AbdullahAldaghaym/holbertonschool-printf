#include "main.h"
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

char output_buf[1024];
int buf_index = 0;

/* Buffer Management */
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

/* Format Parsing */
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
        else if (format[*i] == '0')
        {
            flags |= FLAG_ZERO;
            (*i)++;
            found_flag = 1;
        }
        else if (format[*i] == '-')
        {
            flags |= FLAG_MINUS;
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

int get_precision(const char *format, int *i, va_list args)
{
    int precision = -1;

    if (format[*i] == '.')
    {
        (*i)++;
        precision = 0;
        
        if (format[*i] >= '0' && format[*i] <= '9')
        {
            precision = 0;
            while (format[*i] >= '0' && format[*i] <= '9')
            {
                precision = precision * 10 + (format[*i] - '0');
                (*i)++;
            }
        }
        else if (format[*i] == '*')
        {
            precision = va_arg(args, int);
            (*i)++;
        }
        else
        {
            precision = 0;
        }
    }
    
    return precision;
}

/* Helper functions for length calculation */
int calculate_number_length(int n, int flags)
{
    int length = 0;
    unsigned int num;
    
    if (n == 0)
        return 1;
    
    if (n < 0)
    {
        length++;
        num = (n == -2147483648) ? 2147483648U : (unsigned int)(-n);
    }
    else
    {
        num = n;
        if (flags & FLAG_PLUS || flags & FLAG_SPACE)
            length++;
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
        length++;
    return length;
}

int calculate_hex_length(unsigned int n, int flags)
{
    int length = calculate_unsigned_length(n, 16);
    if ((flags & FLAG_HASH) && n != 0)
        length += 2;
    return length;
}

void print_padding(int width, char pad_char, int *count)
{
    while (width > 0)
    {
        *count += _putchar(pad_char);
        width--;
    }
}

void print_zero_padding(int width, int *count)
{
    print_padding(width, '0', count);
}

/* Basic Number Printing Functions */
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
		if (n == -2147483648)
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
		if (n == -9223372036854775807L-1)
			num = (unsigned long)9223372036854775807UL + 1;
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
    return print_unsigned((unsigned int)n);
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
    return print_octal((unsigned int)n, flags);
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
    return print_hex((unsigned int)n, uppercase, flags);
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

/* Precision Handling Functions */

/* Integer with precision */
void print_number_with_precision(int n, int *count, int precision, int flags)
{
    unsigned int num;
    int is_negative = 0;
    int num_len = 0;
    unsigned int temp;

    if (n == 0 && precision == 0)
        return;

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

    /* Calculate length of number */
    temp = num;
    do {
        num_len++;
        temp /= 10;
    } while (temp > 0);

    /* Print sign */
    if (is_negative)
        *count += _putchar('-');
    else if (flags & FLAG_PLUS)
        *count += _putchar('+');
    else if (flags & FLAG_SPACE)
        *count += _putchar(' ');

    /* Print zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, count);

    /* Print the number */
    if (num == 0)
        *count += _putchar('0');
    else if (num / 10)
        print_number_int(num / 10, count);
    
    if (num != 0)
        *count += _putchar((num % 10) + '0');
}

/* Unsigned with precision */
int print_unsigned_with_precision(unsigned int n, int precision, int flags)
{
    int count = 0;
    int num_len = 0;
    unsigned int temp = n;

    (void)flags;

    if (n == 0 && precision == 0)
        return 0;

    /* Calculate length */
    do {
        num_len++;
        temp /= 10;
    } while (temp > 0);

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    /* Print number */
    if (n == 0)
        count += _putchar('0');
    else
        count += print_unsigned(n);

    return count;
}

/* Octal with precision */
int print_octal_with_precision(unsigned int n, int precision, int flags)
{
    int count = 0;
    int num_len = calculate_unsigned_length(n, 8);
    int has_hash = (flags & FLAG_HASH) && n != 0;

    if (n == 0 && precision == 0)
    {
        if (has_hash)
            count += _putchar('0');
        return count;
    }

    if (has_hash)
        count += _putchar('0');

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_octal(n, 0);
    return count;
}

/* Hex with precision */
int print_hex_with_precision(unsigned int n, int precision, int flags, int uppercase)
{
    int count = 0;
    int num_len = calculate_unsigned_length(n, 16);
    int has_hash = (flags & FLAG_HASH) && n != 0;

    if (n == 0 && precision == 0)
        return count;

    if (has_hash)
    {
        count += _putchar('0');
        count += _putchar(uppercase ? 'X' : 'x');
    }

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_hex(n, uppercase, 0);
    return count;
}

/* String with precision */
int print_string_with_precision(char *str, int precision)
{
    int count = 0;
    int i = 0;

    if (str == NULL)
        str = "(null)";

    while (str[i] && (precision == -1 || i < precision))
    {
        count += _putchar(str[i]);
        i++;
    }

    return count;
}

/* Combined Width and Precision Handling */

int print_char_with_width_precision(char c, int width, int precision, int flags)
{
    int count = 0;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    (void)precision;

    if (!(flags & FLAG_MINUS) && width > 1)
        print_padding(width - 1, pad_char, &count);

    count += _putchar(c);

    if (flags & FLAG_MINUS && width > 1)
        print_padding(width - 1, ' ', &count);

    return count;
}

int print_string_with_width_precision(char *str, int width, int precision, int flags)
{
    int count = 0;
    int len = 0;
    char *temp = str;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (str == NULL)
        str = "(null)";

    /* Calculate actual print length considering precision */
    while (*temp && (precision == -1 || len < precision))
    {
        len++;
        temp++;
    }

    if (!(flags & FLAG_MINUS) && width > len)
        print_padding(width - len, pad_char, &count);

    count += print_string_with_precision(str, precision);

    if (flags & FLAG_MINUS && width > len)
        print_padding(width - len, ' ', &count);

    return count;
}

int print_percent_with_width(int width, int flags)
{
    int count = 0;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (!(flags & FLAG_MINUS) && width > 1)
        print_padding(width - 1, pad_char, &count);

    count += _putchar('%');

    if (flags & FLAG_MINUS && width > 1)
        print_padding(width - 1, ' ', &count);

    return count;
}

void print_number_with_width_precision(int n, int *count, int width, int precision, int flags)
{
    int total_len = 0;
    int num_len = 0;
    unsigned int num;
    int is_negative = 0;
    char pad_char = ' ';
    unsigned int temp;

    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', count);
        return;
    }

    /* Calculate lengths */
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

    temp = num;
    do {
        num_len++;
        temp /= 10;
    } while (temp > 0);

    if (num == 0)
        num_len = 1;

    total_len = (precision > num_len) ? precision : num_len;
    if (is_negative || (flags & FLAG_PLUS) || (flags & FLAG_SPACE))
        total_len++;

    /* Determine padding character */
    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    /* Left padding */
    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, count);

    /* Print sign */
    if (is_negative)
        *count += _putchar('-');
    else if (flags & FLAG_PLUS)
        *count += _putchar('+');
    else if (flags & FLAG_SPACE)
        *count += _putchar(' ');

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, count);

    /* Print number */
    if (num == 0)
        *count += _putchar('0');
    else if (num / 10)
        print_number_int(num / 10, count);
    
    if (num != 0)
        *count += _putchar((num % 10) + '0');

    /* Right padding */
    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', count);
}

/* Similar functions for long and short versions */
void print_long_number_with_width_precision(long n, int *count, int width, int precision, int flags)
{
    print_number_with_width_precision((int)n, count, width, precision, flags);
}

void print_short_number_with_width_precision(short n, int *count, int width, int precision, int flags)
{
    print_number_with_width_precision((int)n, count, width, precision, flags);
}

/* Unsigned with width and precision */
int print_unsigned_with_width_precision(unsigned int n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = calculate_unsigned_length(n, 10);
    int total_len = (precision > num_len) ? precision : num_len;
    char pad_char = ' ';

    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, &count);

    count += print_unsigned_with_precision(n, precision, flags);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

/* Long unsigned with width and precision */
int print_long_unsigned_with_width_precision(unsigned long n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
    char pad_char = ' ';

    (void)flags;

    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    /* Calculate length */
    do {
        num_len++;
        temp /= 10;
    } while (temp > 0);
    if (n == 0) num_len = 1;

    total_len = (precision > num_len) ? precision : num_len;

    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, &count);

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_long_unsigned(n);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

/* Short unsigned with width and precision */
int print_short_unsigned_with_width_precision(unsigned short n, int width, int precision, int flags)
{
    return print_unsigned_with_width_precision((unsigned int)n, width, precision, flags);
}

/* Octal with width and precision */
int print_octal_with_width_precision(unsigned int n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = calculate_unsigned_length(n, 8);
    int total_len = (precision > num_len) ? precision : num_len;
    int has_hash = (flags & FLAG_HASH) && n != 0;
    char pad_char = ' ';

    if (n == 0 && precision == 0)
    {
        if (has_hash && width > 1)
            print_padding(width - 1, ' ', &count);
        else if (width > 0)
            print_padding(width, ' ', &count);
        if (has_hash)
            count += _putchar('0');
        return count;
    }

    if (has_hash)
        total_len++;

    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, &count);

    if (has_hash)
        count += _putchar('0');

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_octal(n, 0);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

/* Long octal with width and precision */
int print_long_octal_with_width_precision(unsigned long n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
    int has_hash = (flags & FLAG_HASH) && n != 0;
    char pad_char = ' ';

    if (n == 0 && precision == 0)
    {
        if (has_hash && width > 1)
            print_padding(width - 1, ' ', &count);
        else if (width > 0)
            print_padding(width, ' ', &count);
        if (has_hash)
            count += _putchar('0');
        return count;
    }

    /* Calculate length */
    do {
        num_len++;
        temp /= 8;
    } while (temp > 0);
    if (n == 0) num_len = 1;

    total_len = (precision > num_len) ? precision : num_len;
    if (has_hash)
        total_len++;

    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, &count);

    if (has_hash)
        count += _putchar('0');

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_long_octal(n, 0);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

/* Short octal with width and precision */
int print_short_octal_with_width_precision(unsigned short n, int width, int precision, int flags)
{
    return print_octal_with_width_precision((unsigned int)n, width, precision, flags);
}

/* Hex with width and precision */
int print_hex_with_width_precision(unsigned int n, int width, int precision, int flags, int uppercase)
{
    int count = 0;
    int num_len = calculate_unsigned_length(n, 16);
    int total_len = (precision > num_len) ? precision : num_len;
    int has_hash = (flags & FLAG_HASH) && n != 0;
    char pad_char = ' ';

    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    if (has_hash)
        total_len += 2;

    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    if (!(flags & FLAG_MINUS) && width > total_len)
        print_padding(width - total_len, pad_char, &count);

    if (has_hash)
    {
        count += _putchar('0');
        count += _putchar(uppercase ? 'X' : 'x');
    }

    /* Zero padding for precision */
    if (precision > num_len)
        print_zero_padding(precision - num_len, &count);

    count += print_hex(n, uppercase, 0);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

/* Long hex with width and precision */
int print_long_hex_with_width_precision(unsigned long n, int width, int precision, int flags, int uppercase)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
    int has_hash = (flags & FLAG_HASH) && n != 0;
    char pad_char = ' ';

    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    /* Calculate length */
    do {
        num_len++;
        temp /= 16;
    } while (temp > 0);
    if (n == 0) num_len = 1;

    total_len = (precision > num_len) ? precision : num_len;
    if (has_hash)
        total_len += 2;

    if (flags & FLAG_ZERO && !(flags & FLAG_MINUS) &&
