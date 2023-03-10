#pragma once

#include <iostream>

void Print(const char* format) {
  std::cout << format;
}

template <typename T, typename... ArgsT>
void Print(const char* format, T arg, ArgsT... args) {
  while (*format != '%') {
    std::putchar(*format++);
  }

  if (*format == '%') {
    std::cout << arg;
    sizeof...(args);

    format++;
    Print(format, args...);
  }
}
