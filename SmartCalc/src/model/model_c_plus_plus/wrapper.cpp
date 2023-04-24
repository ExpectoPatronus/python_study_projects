#include <string>

#include "model.hpp"

extern "C" {

void CalculateWrapper(char* result, const char* expression) {
  std::string result_str;
  std::string expression_str(expression);
  s21::SmartCalculator calculator;
  calculator.Calculate(result_str, expression_str);
  strncpy(result, result_str.c_str(), 256);
}
}
