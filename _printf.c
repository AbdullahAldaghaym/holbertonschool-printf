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

/* Custom String Functions - FIXED VERSION */
int print_custom_string_internal(char *str, int precision)
{
    int count = 0;
    int i = 0;
    unsigned char c;

    if (str == NULL)
        str = "(null)";

    while (str[i] && (precision == -1 || i < precision))
    {
        c = (unsigned char)str[i];
        
        if (c < 32 || c >= 127)
        {
            count += _putchar('\\');
            count += _putchar('x');
            
            /* Always print 2 hexadecimal digits in uppercase */
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
        i++;
    }
    return count;
}

int print_custom_string(va_list args)
{
    char *str = va_arg(args, char *);
    return print_custom_string_internal(str, -1);
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

/* Reversed String Functions */
int print_reversed_string(char *str, int width, int precision, int flags)
{
    int count = 0;
    int len = 0;
    int i;
    char *temp = str;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (str == NULL)
        str = "(null)";

    /* Calculate string length considering precision */
    while (*temp && (precision == -1 || len < precision))
    {
        len++;
        temp++;
    }

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: reversed string first, then padding */
        for (i = len - 1; i >= 0; i--)
        {
            count += _putchar(str[i]);
        }
        
        if (width > len)
            print_padding(width - len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then reversed string */
        if (width > len)
            print_padding(width - len, pad_char, &count);
            
        for (i = len - 1; i >= 0; i--)
        {
            count += _putchar(str[i]);
        }
    }

    return count;
}

int print_reversed_string_arg(va_list args, int width, int precision, int flags)
{
    char *str = va_arg(args, char *);
    return print_reversed_string(str, width, precision, flags);
}

/* Rot13 String Functions */
char apply_rot13(char c)
{
    if ((c >= 'a' && c <= 'z'))
    {
        if (c + 13 <= 'z')
            return (c + 13);
        else
            return (c - 13);
    }
    else if ((c >= 'A' && c <= 'Z'))
    {
        if (c + 13 <= 'Z')
            return (c + 13);
        else
            return (c - 13);
    }
    return (c);
}

int print_rot13_string(char *str, int width, int precision, int flags)
{
    int count = 0;
    int len = 0;
    int i;
    char *temp = str;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (str == NULL)
        str = "(null)";

    /* Calculate string length considering precision */
    while (*temp && (precision == -1 || len < precision))
    {
        len++;
        temp++;
    }

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: ROT13 string first, then padding */
        for (i = 0; i < len; i++)
        {
            count += _putchar(apply_rot13(str[i]));
        }
        
        if (width > len)
            print_padding(width - len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then ROT13 string */
        if (width > len)
            print_padding(width - len, pad_char, &count);
            
        for (i = 0; i < len; i++)
        {
            count += _putchar(apply_rot13(str[i]));
        }
    }

    return count;
}

int print_rot13_string_arg(va_list args, int width, int precision, int flags)
{
    char *str = va_arg(args, char *);
    return print_rot13_string(str, width, precision, flags);
}

/* Combined Width and Precision Handling with Left Alignment Support */
int print_char_with_width_precision(char c, int width, int precision, int flags)
{
    int count = 0;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    (void)precision;

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: char first, then padding */
        count += _putchar(c);
        
        if (width > 1)
            print_padding(width - 1, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then char */
        if (width > 1)
            print_padding(width - 1, pad_char, &count);
            
        count += _putchar(c);
    }

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

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: string first, then padding */
        count += print_string_with_precision(str, precision);
        
        if (width > len)
            print_padding(width - len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then string */
        if (width > len)
            print_padding(width - len, pad_char, &count);
            
        count += print_string_with_precision(str, precision);
    }

    return count;
}

int print_percent_with_width(int width, int flags)
{
    int count = 0;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: % first, then padding */
        count += _putchar('%');
        
        if (width > 1)
            print_padding(width - 1, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then % */
        if (width > 1)
            print_padding(width - 1, pad_char, &count);
            
        count += _putchar('%');
    }

    return count;
}

/* Custom string with width and precision - FIXED VERSION */
int print_custom_string_with_width_precision(va_list args, int width, int precision, int flags)
{
    int count = 0;
    char *str = va_arg(args, char *);
    int len = 0;
    char *temp = str;
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (str == NULL)
        str = "(null)";

    /* Calculate custom string length considering precision */
    temp = str;
    while (*temp && (precision == -1 || len < precision))
    {
        unsigned char c = (unsigned char)*temp;
        if (c < 32 || c >= 127)
            len += 4;  /* \x + 2 hex digits */
        else
            len++;
        temp++;
    }

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: custom string first, then padding */
        count += print_custom_string_internal(str, precision);
        
        if (width > len)
            print_padding(width - len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then custom string */
        if (width > len)
            print_padding(width - len, pad_char, &count);
            
        count += print_custom_string_internal(str, precision);
    }

    return count;
}

/* Helper function for string precision */
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

/* Integer with width, precision and flags */
void print_number_with_width_precision(int n, int *count, int width, int precision, int flags)
{
    char buffer[32];
    int idx = 0, is_negative = 0;
    unsigned int num;
    int total_len, num_len, i;
    char pad_char = ' ';

    /* Handle zero precision with zero value */
    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', count);
        return;
    }

    /* Handle negative numbers */
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

    /* Convert number to string in reverse */
    if (num == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        while (num)
        {
            buffer[idx++] = (num % 10) + '0';
            num /= 10;
        }
    }
    num_len = idx;

    /* Calculate total length */
    total_len = (precision > num_len) ? precision : num_len;
    if (is_negative || (flags & FLAG_PLUS) || (flags & FLAG_SPACE))
        total_len++;

    /* Determine padding character */
    if ((flags & FLAG_ZERO) && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    /* LEFT ALIGNMENT HANDLING */
    if (flags & FLAG_MINUS)
    {
        /* Left alignment: content first, then padding */
        
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

        /* Print the number digits */
        for (i = idx - 1; i >= 0; i--)
        {
            *count += _putchar(buffer[i]);
        }

        /* Right padding for left alignment */
        if (width > total_len)
            print_padding(width - total_len, ' ', count);
    }
    else
    {
        /* Right alignment: padding first, then content */
        
        /* Handle zero padding with sign */
        if (pad_char == '0')
        {
            /* Print sign first with zero padding */
            if (is_negative)
                *count += _putchar('-');
            else if (flags & FLAG_PLUS)
                *count += _putchar('+');
            else if (flags & FLAG_SPACE)
                *count += _putchar(' ');
            
            /* Then padding */
            if (width > total_len)
                print_padding(width - total_len, pad_char, count);
        }
        else
        {
            /* Space padding - padding first, then sign */
            if (width > total_len)
                print_padding(width - total_len, pad_char, count);
            
            /* Then sign */
            if (is_negative)
                *count += _putchar('-');
            else if (flags & FLAG_PLUS)
                *count += _putchar('+');
            else if (flags & FLAG_SPACE)
                *count += _putchar(' ');
        }

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, count);

        /* Print the number digits */
        for (i = idx - 1; i >= 0; i--)
        {
            *count += _putchar(buffer[i]);
        }
    }
}

void print_long_number_with_width_precision(long n, int *count, int width, int precision, int flags)
{
    print_number_with_width_precision((int)n, count, width, precision, flags);
}

void print_short_number_with_width_precision(short n, int *count, int width, int precision, int flags)
{
    print_number_with_width_precision((int)n, count, width, precision, flags);
}

/* Unsigned with width, precision and flags */
int print_unsigned_with_width_precision(unsigned int n, int width, int precision, int flags)
{
    int count = 0;
    char buffer[32];
    int idx = 0, num_len, total_len, i;
    char pad_char = ' ';

    /* Handle zero precision with zero value */
    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    /* Convert number to string */
    if (n == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        while (n)
        {
            buffer[idx++] = (n % 10) + '0';
            n /= 10;
        }
    }
    num_len = idx;

    total_len = (precision > num_len) ? precision : num_len;

    /* Determine padding character */
    if ((flags & FLAG_ZERO) && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    /* LEFT ALIGNMENT HANDLING */
    if (flags & FLAG_MINUS)
    {
        /* Left alignment: content first */
        
        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print number */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }

        /* Right padding */
        if (width > total_len)
            print_padding(width - total_len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first */
        
        if (width > total_len)
            print_padding(width - total_len, pad_char, &count);

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print number */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }
    }

    return count;
}

int print_long_unsigned_with_width_precision(unsigned long n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
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
        temp /= 10;
    } while (temp > 0);
    if (n == 0) num_len = 1;

    total_len = (precision > num_len) ? precision : num_len;

    /* Determine padding character */
    if ((flags & FLAG_ZERO) && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

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

int print_short_unsigned_with_width_precision(unsigned short n, int width, int precision, int flags)
{
    return print_unsigned_with_width_precision((unsigned int)n, width, precision, flags);
}

/* Octal with width, precision and flags */
int print_octal_with_width_precision(unsigned int n, int width, int precision, int flags)
{
    int count = 0;
    char buffer[32];
    int idx = 0, num_len, total_len, i;
    int has_hash = (flags & FLAG_HASH) && (n != 0);
    char pad_char = ' ';

    /* Handle zero precision with zero value */
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

    /* Convert number to octal string */
    if (n == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        while (n)
        {
            buffer[idx++] = (n % 8) + '0';
            n /= 8;
        }
    }
    num_len = idx;

    total_len = (precision > num_len) ? precision : num_len;
    if (has_hash)
        total_len++;

    /* Determine padding character */
    if ((flags & FLAG_ZERO) && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    /* LEFT ALIGNMENT HANDLING */
    if (flags & FLAG_MINUS)
    {
        /* Left alignment: content first */
        
        /* Print hash prefix */
        if (has_hash)
            count += _putchar('0');

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print octal digits */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }

        /* Right padding */
        if (width > total_len)
            print_padding(width - total_len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first */
        
        if (width > total_len)
            print_padding(width - total_len, pad_char, &count);

        /* Print hash prefix */
        if (has_hash)
            count += _putchar('0');

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print octal digits */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }
    }

    return count;
}

int print_long_octal_with_width_precision(unsigned long n, int width, int precision, int flags)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
    int has_hash = (flags & FLAG_HASH) && (n != 0);
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

int print_short_octal_with_width_precision(unsigned short n, int width, int precision, int flags)
{
    return print_octal_with_width_precision((unsigned int)n, width, precision, flags);
}

/* Hex with width, precision and flags */
int print_hex_with_width_precision(unsigned int n, int width, int precision, int flags, int uppercase)
{
    int count = 0;
    char buffer[32];
    int idx = 0, num_len, total_len, i;
    int has_hash = (flags & FLAG_HASH) && (n != 0);
    char pad_char = ' ';
    char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    /* Handle zero precision with zero value */
    if (n == 0 && precision == 0)
    {
        if (width > 0)
            print_padding(width, ' ', &count);
        return count;
    }

    /* Convert number to hex string */
    if (n == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        while (n)
        {
            buffer[idx++] = digits[n % 16];
            n /= 16;
        }
    }
    num_len = idx;

    total_len = (precision > num_len) ? precision : num_len;
    if (has_hash)
        total_len += 2;

    /* Determine padding character */
    if ((flags & FLAG_ZERO) && !(flags & FLAG_MINUS) && precision == -1)
        pad_char = '0';

    /* LEFT ALIGNMENT HANDLING */
    if (flags & FLAG_MINUS)
    {
        /* Left alignment: content first */
        
        /* Print hash prefix */
        if (has_hash)
        {
            count += _putchar('0');
            count += _putchar(uppercase ? 'X' : 'x');
        }

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print hex digits */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }

        /* Right padding */
        if (width > total_len)
            print_padding(width - total_len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first */
        
        /* Handle zero padding with hash prefix */
        if (pad_char == '0' && has_hash)
        {
            /* Print hash prefix first with zero padding */
            count += _putchar('0');
            count += _putchar(uppercase ? 'X' : 'x');
            
            if (width > total_len)
                print_padding(width - total_len, pad_char, &count);
        }
        else
        {
            /* Space padding - padding first */
            if (width > total_len)
                print_padding(width - total_len, pad_char, &count);
            
            /* Then hash prefix */
            if (has_hash)
            {
                count += _putchar('0');
                count += _putchar(uppercase ? 'X' : 'x');
            }
        }

        /* Zero padding for precision */
        if (precision > num_len)
            print_zero_padding(precision - num_len, &count);

        /* Print hex digits */
        for (i = idx - 1; i >= 0; i--)
        {
            count += _putchar(buffer[i]);
        }
    }

    return count;
}

int print_long_hex_with_width_precision(unsigned long n, int width, int precision, int flags, int uppercase)
{
    int count = 0;
    int num_len = 0;
    unsigned long temp = n;
    int total_len;
    int has_hash = (flags & FLAG_HASH) && (n != 0);
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

    count += print_long_hex(n, uppercase, 0);

    if (flags & FLAG_MINUS && width > total_len)
        print_padding(width - total_len, ' ', &count);

    return count;
}

int print_short_hex_with_width_precision(unsigned short n, int width, int precision, int flags, int uppercase)
{
    return print_hex_with_width_precision((unsigned int)n, width, precision, flags, uppercase);
}

/* Binary with width */
int print_binary_with_width(unsigned int n, int width, int flags)
{
    int count = 0;
    int len = calculate_unsigned_length(n, 2);
    char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';

    if (flags & FLAG_MINUS)
    {
        /* Left alignment: binary first, then padding */
        count += print_binary(n);
        
        if (width > len)
            print_padding(width - len, ' ', &count);
    }
    else
    {
        /* Right alignment: padding first, then binary */
        if (width > len)
            print_padding(width - len, pad_char, &count);
            
        count += print_binary(n);
    }

    return count;
}

/* Pointer with width and precision */
int print_pointer_with_width_precision(void *ptr, int width, int precision, int flags)
{
    int count = 0;
    
    (void)precision;
    
    if (ptr == NULL)
    {
        int len = 5;
        char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';
        
        if (flags & FLAG_MINUS)
        {
            count += _printf("(nil)");
            if (width > len)
                print_padding(width - len, ' ', &count);
        }
        else
        {
            if (width > len)
                print_padding(width - len, pad_char, &count);
            count += _printf("(nil)");
        }
    }
    else
    {
        unsigned long address = (unsigned long)ptr;
        int len = 2;
        unsigned long temp = address;
        char pad_char = (flags & FLAG_ZERO && !(flags & FLAG_MINUS)) ? '0' : ' ';
        
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
        
        if (flags & FLAG_MINUS)
        {
            count += _putchar('0');
            count += _putchar('x');
            count += print_hex_long(address, 0);
            if (width > len)
                print_padding(width - len, ' ', &count);
        }
        else
        {
            if (width > len)
                print_padding(width - len, pad_char, &count);
            count += _putchar('0');
            count += _putchar('x');
            count += print_hex_long(address, 0);
        }
    }
    
    return count;
}

/* Main _printf function */
int _printf(const char *format, ...)
{
	va_list args;
	int i = 0, count = 0;
	int flags, length, width, precision;

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
			precision = get_precision(format, &i, args);
			length = get_length(format, &i);
			
			if (format[i] == '\0')
			{
				flush_buffer();
				va_end(args);
				return (-1);
			}
			
			/* Handle all conversion specifiers with width, precision and flags */
			if (format[i] == 'c')
			{
				count += print_char_with_width_precision(va_arg(args, int), width, precision, flags);
			}
			else if (format[i] == 's')
			{
				char *str = va_arg(args, char *);
				count += print_string_with_width_precision(str, width, precision, flags);
			}
			else if (format[i] == 'S')
			{
				count += print_custom_string_with_width_precision(args, width, precision, flags);
			}
			else if (format[i] == 'r')  /* Reversed string conversion specifier */
			{
				count += print_reversed_string_arg(args, width, precision, flags);
			}
			else if (format[i] == 'R')  /* ROT13 string conversion specifier */
			{
				count += print_rot13_string_arg(args, width, precision, flags);
			}
			else if (format[i] == 'd' || format[i] == 'i')
			{
				if (length == LENGTH_L)
					print_long_number_with_width_precision(va_arg(args, long), &count, width, precision, flags);
				else if (length == LENGTH_H)
					print_short_number_with_width_precision((short)va_arg(args, int), &count, width, precision, flags);
				else
					print_number_with_width_precision(va_arg(args, int), &count, width, precision, flags);
			}
			else if (format[i] == 'u')
			{
				if (length == LENGTH_L)
					count += print_long_unsigned_with_width_precision(va_arg(args, unsigned long), width, precision, flags);
				else if (length == LENGTH_H)
					count += print_short_unsigned_with_width_precision((unsigned short)va_arg(args, unsigned int), width, precision, flags);
				else
					count += print_unsigned_with_width_precision(va_arg(args, unsigned int), width, precision, flags);
			}
			else if (format[i] == 'o')
			{
				if (length == LENGTH_L)
					count += print_long_octal_with_width_precision(va_arg(args, unsigned long), width, precision, flags);
				else if (length == LENGTH_H)
					count += print_short_octal_with_width_precision((unsigned short)va_arg(args, unsigned int), width, precision, flags);
				else
					count += print_octal_with_width_precision(va_arg(args, unsigned int), width, precision, flags);
			}
			else if (format[i] == 'x')
			{
				if (length == LENGTH_L)
					count += print_long_hex_with_width_precision(va_arg(args, unsigned long), width, precision, flags, 0);
				else if (length == LENGTH_H)
					count += print_short_hex_with_width_precision((unsigned short)va_arg(args, unsigned int), width, precision, flags, 0);
				else
					count += print_hex_with_width_precision(va_arg(args, unsigned int), width, precision, flags, 0);
			}
			else if (format[i] == 'X')
			{
				if (length == LENGTH_L)
					count += print_long_hex_with_width_precision(va_arg(args, unsigned long), width, precision, flags, 1);
				else if (length == LENGTH_H)
					count += print_short_hex_with_width_precision((unsigned short)va_arg(args, unsigned int), width, precision, flags, 1);
				else
					count += print_hex_with_width_precision(va_arg(args, unsigned int), width, precision, flags, 1);
			}
			else if (format[i] == 'b')
			{
				count += print_binary_with_width(va_arg(args, unsigned int), width, flags);
			}
			else if (format[i] == 'p')
			{
				count += print_pointer_with_width_precision(va_arg(args, void*), width, precision, flags);
			}
			else if (format[i] == '%')
			{
				count += print_percent_with_width(width, flags);
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
