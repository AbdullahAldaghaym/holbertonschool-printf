#include "main.h"
#include <stdarg.h>
#include <unistd.h>

/**
	* flush_buffer - writes buffer to stdout
	* @buffer: the buffer to flush
	* @buf_ind: pointer to buffer index
	*/
void flush_buffer(char buffer[], int *buf_ind)
{
	if (*buf_ind > 0)
	{
	write(1, buffer, *buf_ind);
	*buf_ind = 0;
	}
}

/**
	* add_to_buffer - adds character to buffer
	* @c: character to add
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	* @count: pointer to character counter
	*/
void add_to_buffer(char c, char buffer[], int *buf_ind, int *count)
{
	if (*buf_ind >= BUFFER_SIZE)
	flush_buffer(buffer, buf_ind);

	buffer[(*buf_ind)++] = c;
	(*count)++;
}

/**
	* _putchar - writes the character c to stdout using buffer
	* @c: The character to print
	* Return: On success 1.
	*/
int _putchar(char c)
{
	/* للحفاظ على التوافق مع الدوال الأخرى */
	static char local_buffer[BUFFER_SIZE];
	static int local_buf_ind = 0;

	if (local_buf_ind >= BUFFER_SIZE)
	flush_buffer(local_buffer, &local_buf_ind);

	local_buffer[local_buf_ind++] = c;

	if (c == '\n' || local_buf_ind >= BUFFER_SIZE)
	flush_buffer(local_buffer, &local_buf_ind);

	return (1);
}

/**
	* print_number - prints an integer number using buffer
	* @n: number to print
	* @count: pointer to character counter
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	*/
void print_number(int n, int *count, char buffer[], int *buf_ind)
{
	unsigned int num;

	if (n == 0)
	{
	add_to_buffer('0', buffer, buf_ind, count);
	return;
	}

	if (n < 0)
	{
	add_to_buffer('-', buffer, buf_ind, count);
	num = -n;
	}
	else
	{
	num = n;
	}

	if (num / 10)
	print_number(num / 10, count, buffer, buf_ind);

	add_to_buffer((num % 10) + '0', buffer, buf_ind, count);
}

/**
	* print_binary - prints a number in binary using buffer
	* @n: number to print
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	* Return: number of characters printed
	*/
int print_binary(unsigned int n, char buffer[], int *buf_ind)
{
	int count = 0;

	if (n == 0)
	{
	add_to_buffer('0', buffer, buf_ind, &count);
	return count;
	}

	if (n / 2)
	count += print_binary(n / 2, buffer, buf_ind);

	add_to_buffer((n % 2) + '0', buffer, buf_ind, &count);
	return count;
}

/**
	* print_unsigned - prints an unsigned integer using buffer
	* @n: number to print
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	* Return: number of characters printed
	*/
int print_unsigned(unsigned int n, char buffer[], int *buf_ind)
{
	int count = 0;

	if (n / 10)
	count += print_unsigned(n / 10, buffer, buf_ind);

	add_to_buffer((n % 10) + '0', buffer, buf_ind, &count);
	return count;
}

/**
	* print_octal - prints a number in octal using buffer
	* @n: number to print
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	* Return: number of characters printed
	*/
int print_octal(unsigned int n, char buffer[], int *buf_ind)
{
	int count = 0;

	if (n / 8)
	count += print_octal(n / 8, buffer, buf_ind);

	add_to_buffer((n % 8) + '0', buffer, buf_ind, &count);
	return count;
}

/**
	* print_hex - prints a number in hexadecimal using buffer
	* @n: number to print
	* @buffer: the buffer
	* @buf_ind: pointer to buffer index
	* @uppercase: 1 for uppercase, 0 for lowercase
	* Return: number of characters printed
	*/
int print_hex(unsigned int n, char buffer[], int *buf_ind, int uppercase)
{
	int count = 0;
	char *digits;

	if (uppercase)
	digits = "0123456789ABCDEF";
	else
	digits = "0123456789abcdef";

	if (n / 16)
	count += print_hex(n / 16, buffer, buf_ind, uppercase);

	add_to_buffer(digits[n % 16], buffer, buf_ind, &count);
	return count;
}

/**
	* _printf - produces output according to a format using buffer
	* @format: character string containing directives
	* Return: number of characters printed
	*/
int _printf(const char *format, ...)
{
	va_list args;
	int i = 0, count = 0, buf_ind = 0;
	char buffer[BUFFER_SIZE];
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
	{
	flush_buffer(buffer, &buf_ind);
	return (-1);
	}

	if (format[i] == 'c')
	{
	add_to_buffer(va_arg(args, int), buffer, &buf_ind, &count);
	}
	else if (format[i] == 's')
	{
	str = va_arg(args, char *);
	if (str == NULL)
	str = "(null)";
	while (str[0] != '\0')
	{
	add_to_buffer(str[0], buffer, &buf_ind, &count);
	str++;
	}
	}
	else if (format[i] == 'd' || format[i] == 'i')
	{
	print_number(va_arg(args, int), &count, buffer, &buf_ind);
	}
	else if (format[i] == 'b')
	{
	count += print_binary(va_arg(args, unsigned int), buffer, &buf_ind);
	}
	else if (format[i] == 'u')
	{
	count += print_unsigned(va_arg(args, unsigned int), buffer, &buf_ind);
	}
	else if (format[i] == 'o')
	{
	count += print_octal(va_arg(args, unsigned int), buffer, &buf_ind);
	}
	else if (format[i] == 'x')
	{
	count += print_hex(va_arg(args, unsigned int), buffer, &buf_ind, 0);
	}
	else if (format[i] == 'X')
	{
	count += print_hex(va_arg(args, unsigned int), buffer, &buf_ind, 1);
	}
	else if (format[i] == '%')
	{
	add_to_buffer('%', buffer, &buf_ind, &count);
	}
	else
	{
	add_to_buffer('%', buffer, &buf_ind, &count);
	add_to_buffer(format[i], buffer, &buf_ind, &count);
	}
	}
	else
	{
	add_to_buffer(format[i], buffer, &buf_ind, &count);
	}
	i++;
	}

	flush_buffer(buffer, &buf_ind);
	va_end(args);
	return (count);
}
