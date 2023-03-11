#pragma once

#include <iostream>

void Print(const char* format);

template <typename T, typename... ArgsT>
void Print(const char* format, T arg, ArgsT... args) {
  while ((*format != '%') && (*format != '\0')) {
    std::putchar(*format++);
  }

  if (*format == '%') {
    std::cout << arg;

    format++;
    Print(format, args...);
  }
}
