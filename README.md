# _printf() - Custom Printf Implementation

A custom implementation of the `printf` function in C, developed as part of the Holberton School curriculum. This project replicates the core functionality of the standard `printf` function, handling various format specifiers and providing a robust output formatting system.

## 🚀 Features

### Currently Supported Format Specifiers

| Specifier | Description | Example |
|-----------|-------------|---------|
| `%c` | Prints a single character | `_printf("%c", 'A')` → `A` |
| `%s` | Prints a string of characters | `_printf("%s", "Hello")` → `Hello` |
| `%d` | Prints a signed decimal integer | `_printf("%d", 42)` → `42` |
| `%i` | Prints a signed integer | `_printf("%i", -42)` → `-42` |
| `%%` | Prints a percent sign | `_printf("%%")` → `%` |

### Coming Soon
- `%u` - Unsigned integers
- `%o` - Octal format
- `%x` - Hexadecimal format
- `%b` - Binary format

## 📁 Project Structure
