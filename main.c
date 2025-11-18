#include <stdio.h>
#include "main.h"

/**
 * main - test _printf function
 * Return: Always 0
 */
int main(void)
{
	int len1, len2;

	_printf("Simple sentence.\n");
	printf("Simple sentence.\n");

	_printf("Character: [%c]\n", 'A');
	printf("Character: [%c]\n", 'A');

	_printf("String: [%s]\n", "Hello World");
	printf("String: [%s]\n", "Hello World");

	_printf("Percent: [%%]\n");
	printf("Percent: [%%]\n");

	_printf("Unknown: [%r]\n");
	printf("Unknown: [%r]\n");

	len1 = _printf("Custom: %c %s %%\n", 'X', "Test");
	len2 = printf("Custom: %c %s %%\n", 'X', "Test");
	
	_printf("Lengths: [%d, %d]\n", len1, len2);
	printf("Lengths: [%d, %d]\n", len1, len2);

	return (0);
}
