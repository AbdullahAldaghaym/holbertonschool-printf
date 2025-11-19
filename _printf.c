#include "main.h"
#include <stdarg.h>
#include <unistd.h>

/**
	* _putchar - writes the character c to stdout
	* @c: The character to print
	* Return: On success 1.
	*/
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
	* print_number - prints an integer number
	* @n: number to print
	* @count: pointer to character counter
	*/
void print_number(int n, int *count)
{
	unsigned int num;

	if (n == 0)
	{
		*count += _putchar('0');
		return;
	}

	if (n < 0)
	{
		*count += _putchar('-');
		num = -n;
	}
	else
	{
		num = n;
	}

	if (num / 10)
		print_number(num / 10, count);

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

	if (n / 10)
		count += print_unsigned(n / 10);

	count += _putchar((n % 10) + '0');

	return count;
}

/**
	* print_octal - prints a number in octal
	* @n: number to print
	* Return: number of characters printed
	*/
int print_octal(unsigned int n)
{
	int count = 0;

	if (n / 8)
		count += print_octal(n / 8);

	count += _putchar((n % 8) + '0');

	return count;
}

/**
	* print_hex - prints a number in hexadecimal
	* @n: number to print
	* @uppercase: 1 for uppercase, 0 for lowercase
	* Return: number of characters printed
	*/
int print_hex(unsigned int n, int uppercase)
{
	int count = 0;
	char *digits;

	if (uppercase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";

	if (n / 16)
		count += print_hex(n / 16, uppercase);

	count += _putchar(digits[n % 16]);

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

	if (format == NULL)
		return (-1);

	va_start(args, format);

	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == '\0')
				return (-1);

			if (format[i] == 'c')
				count += _putchar(va_arg(args, int));
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
			else if (format[i] == 'd' || format[i] == 'i')
				print_number(va_arg(args, int), &count);
			else if (format[i] == 'b')
				count += print_binary(va_arg(args, unsigned int));
			else if (format[i] == 'u')
				count += print_unsigned(va_arg(args, unsigned int));
			else if (format[i] == 'o')
				count += print_octal(va_arg(args, unsigned int));
			else if (format[i] == 'x')
				count += print_hex(va_arg(args, unsigned int), 0);
			else if (format[i] == 'X')
				count += print_hex(va_arg(args, unsigned int), 1);
			else if (format[i] == '%')
				count += _putchar('%');
			else
			{
				count += _putchar('%');
				count += _putchar(format[i]);
			}
		}
		else
			count += _putchar(format[i]);
		i++;
	}

	va_end(args);
	return (count);
}
