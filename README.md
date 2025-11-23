# Custom _printf() Implementation

<p align="center">
  <img src="https://apply.holbertonschool.com/holberton-logo.png" alt="Holberton School Logo" width="300">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C-Programming-blue.svg" alt="C Language">
  <img src="https://img.shields.io/badge/Holberton-School-red.svg" alt="Holberton School">
  <img src="https://img.shields.io/badge/License-MIT-green.svg" alt="License">
  <img src="https://img.shields.io/badge/Version-1.0-orange.svg" alt="Version">
</p>

<p align="center">
  <strong>A comprehensive custom implementation of the printf function in C</strong><br>
  Developed as part of the Holberton School curriculum
</p>

## 📋 Table of Contents

- [Features](#features)
- [Supported Format Specifiers](#supported-format-specifiers)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Examples](#examples)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- **Full printf Compatibility**: Implements most standard printf functionality with high accuracy
- **Advanced Buffer Management**: 1024-character output buffer for optimal performance
- **Comprehensive Formatting**: Supports flags, width, precision, and length modifiers
- **Custom Specifiers**: Extended functionality beyond standard printf
- **Memory Efficient**: Minimal memory footprint with optimized buffer handling
- **Error Resilient**: Robust error handling and edge case management
- **Standards Compliant**: Follows C89 standards and Betty coding style

## 🔧 Supported Format Specifiers

### Standard Specifiers

| Specifier | Description | Example Output |
|-----------|-------------|----------------|
| `%c` | Character | `_printf("%c", 'A')` → `A` |
| `%s` | String | `_printf("%s", "Hello")` → `Hello` |
| `%d` / `%i` | Signed integer | `_printf("%d", -42)` → `-42` |
| `%u` | Unsigned integer | `_printf("%u", 255)` → `255` |
| `%o` | Octal | `_printf("%o", 64)` → `100` |
| `%x` | Hexadecimal (lowercase) | `_printf("%x", 255)` → `ff` |
| `%X` | Hexadecimal (uppercase) | `_printf("%X", 255)` → `FF` |
| `%p` | Pointer address | `_printf("%p", &var)` → `0x7ffe637541f0` |
| `%%` | Percent sign | `_printf("%%")` → `%` |

### Custom Specifiers

| Specifier | Description | Example Output |
|-----------|-------------|----------------|
| `%b` | Binary | `_printf("%b", 5)` → `101` |
| `%S` | Custom string (non-printable as \\xXX) | `_printf("%S", "Test\\n")` → `Test\\x0A` |
| `%r` | Reversed string | `_printf("%r", "hello")` → `olleh` |
| `%R` | ROT13 string | `_printf("%R", "Hello")` → `Uryyb` |

### Format Flags

| Flag | Description | Example |
|------|-------------|---------|
| `+` | Always show sign | `%+d` → `+42` |
| ` ` (space) | Prefix positive with space | `% d` → ` 42` |
| `#` | Alternate form | `%#x` → `0xff`, `%#o` → `0100` |
| `0` | Zero padding | `%05d` → `00042` |
| `-` | Left alignment | `%-10s` → `Hello     ` |

### Length Modifiers

| Modifier | Description | Supported Types |
|----------|-------------|-----------------|
| `l` | Long | `%ld`, `%lu`, `%lx` |
| `h` | Short | `%hd`, `%hu`, `%hx` |

## 📥 Installation

### Prerequisites
- GCC compiler
- Ubuntu 20.04 LTS (recommended)
- Make (optional)

### Compilation

```bash
# Clone the repository
git clone https://github.com/AbdullahAldaghaym/holbertonschool-printf.git
cd printf

# Compile all source files
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-format *.c -o printf

# Or compile with specific files
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 _printf.c main.c -o printf_test

```
## 🚀 Usage

### Basic Example 
```c
#include "main.h"

int main(void)
{
    int count;
    
    count = _printf("Hello, %s!\\n", "World");
    _printf("Printed %d characters\\n", count);
    
    return (0);
}
```
### Advanced Formatting Examples
```c 
#include "main.h"

int main(void)
{
    int num = 42;
    char *str = "Test";
    void *ptr = &num;
    
    /* Basic specifiers */
    _printf("Character: %c\\n", 'X');
    _printf("String: %s\\n", "Hello World");
    _printf("Integer: %d\\n", -123);
    _printf("Percent: %%\\n");
    
    /* Number formats */
    _printf("Unsigned: %u\\n", 255);
    _printf("Octal: %o\\n", 64);
    _printf("Hex: %x, %X\\n", 255, 255);
    
    /* Custom specifiers */
    _printf("Binary: %b\\n", 10);
    _printf("Reversed: %r\\n", "hello");
    _printf("ROT13: %R\\n", "Hello World");
    _printf("Custom string: %S\\n", "Test\\nLine");
    
    /* Pointers */
    _printf("Pointer: %p\\n", ptr);
    _printf("NULL Pointer: %p\\n", NULL);
    
    /* Flags */
    _printf("Plus: %+d\\n", 42);
    _printf("Space: % d\\n", 42);
    _printf("Hash: %#x, %#o\\n", 255, 64);
    
    return 0;
}
```

##📁 Project Structure

```
printf/
├── main.h              # Header file with all prototypes and macros
├── _printf.c           # Main _printf implementation
├── main.c              # Example test file
├── man_3_printf        # Manual page
├── README.md           # Project documentation
└── test_files/         # Additional test cases (optional)

```

## 🔬 Technical Details

### Buffer System 
#### The implementation uses a 1024-character static buffer to minimize system calls:

```c
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
```
### Format Parsing Pipeline

- Flag Detection: `+`, `#`, `0`, `-`
- Width Specification: Fixed numbers or `*` for variable width
- Precision Handling: `.` precision with optional number or `*`
- Length Modifiers: `l`, `h` for type size
- Specifier Execution: Appropriate conversion function

  
## 🧪 Testing
```c
#include <limits.h>
#include <stdio.h>
#include "main.h"

/**
 * main - Comprehensive test function
 * Return: Always 0
 */
int main(void)
{
    int len, len2;
    unsigned int ui;
    void *addr;

    /* Basic functionality */
    len = _printf("Let's try to printf a simple sentence.\\n");
    len2 = printf("Let's try to printf a simple sentence.\\n");
    
    /* Edge cases */
    ui = (unsigned int)INT_MAX + 1024;
    addr = (void *)0x7ffe637541f0;
    
    /* Comparison tests */
    _printf("Length:[%d, %i]\\n", len, len);
    printf("Length:[%d, %i]\\n", len2, len2);
    _printf("Negative:[%d]\\n", -762534);
    printf("Negative:[%d]\\n", -762534);
    _printf("Unsigned:[%u]\\n", ui);
    printf("Unsigned:[%u]\\n", ui);
    _printf("Unsigned octal:[%o]\\n", ui);
    printf("Unsigned octal:[%o]\\n", ui);
    _printf("Unsigned hexadecimal:[%x, %X]\\n", ui, ui);
    printf("Unsigned hexadecimal:[%x, %X]\\n", ui, ui);
    _printf("Character:[%c]\\n", 'H');
    printf("Character:[%c]\\n", 'H');
    _printf("String:[%s]\\n", "I am a string !");
    printf("String:[%s]\\n", "I am a string !");
    _printf("Address:[%p]\\n", addr);
    printf("Address:[%p]\\n", addr);
    _printf("Percent:[%%]\\n");
    printf("Percent:[%%]\\n");
    
    /* Custom specifiers */
    _printf("Binary:[%b]\\n", 98);
    _printf("Custom string:[%S]\\n", "Best\\nSchool");
    _printf("Reversed:[%r]\\n", "Hello");
    _printf("ROT13:[%R]\\n", "Hello World");
    
    return (0);
}
```

## Compilation and Execution 
```sh
# Compile test suite
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-format *.c -o printf_test

# Run tests
./printf_test

# Compare with standard printf
./printf_test | diff - expected_output.txt
```


## 🛠️ Function Reference

#### Core Functions
```c
int _printf(const char *format, ...);
int _putchar(char c);
void flush_buffer(void);
```
#### Conversion Handlers
```c
/* Number printing */
void print_number(int n, int *count, int flags);
int print_unsigned(unsigned int n);
int print_octal(unsigned int n, int flags);
int print_hex(unsigned int n, int uppercase, int flags);

/* Custom specifiers */
int print_binary(unsigned int n);
int print_custom_string(va_list args);
int print_reversed_string(char *str, int width, int precision, int flags);
int print_rot13_string(char *str, int width, int precision, int flags);
int print_pointer(va_list args);
```
#### Format Parsing
```c
int get_flags(const char *format, int *i);
int get_width(const char *format, int *i, va_list args);
int get_precision(const char *format, int *i, va_list args);
int get_length(const char *format, int *i);
```



## 👥 Authors

<div align="center">

### **Project Developers**

| **Solaf Alessa** | **Abdullah Aldaghaym** |
|:----------------:|:----------------------:|
| [![GitHub](https://img.shields.io/badge/📁_GitHub-Profile-black?style=for-the-badge&logo=github)](https://github.com/lilsouy) | [![GitHub](https://img.shields.io/badge/📁_GitHub-Profile-black?style=for-the-badge&logo=github)](https://github.com/abdullahaldaghaym) |
| **C Developer** | **C Developer** |

</div>
