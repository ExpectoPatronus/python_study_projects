#include "model.hpp"

#include <iostream>
#include <locale>

namespace s21 {
void SmartCalculator::Calculate(std::string &result_str,
                                const std::string &str) {
  std::locale::global(std::locale("C"));
  result_str = str;
  try {
    CheckInput(result_str);
  } catch (std::invalid_argument const &ex) {
    result_str = "Error";
    return;
  }
  try {
    double result = Parser(result_str);
    FixString(result_str, result);
  } catch (...) {
    result_str = "Error";
    return;
  }
}

double SmartCalculator::Parser(std::string &result_str) {
  size_t i = 0;
  bool flag = false;
  std::stack<Lexsema> numlist;
  std::stack<Lexsema> operatorlist;
  Lexsema leks = {};
  while (!flag) {
    if ((result_str[i] >= '0' && result_str[i] <= '9')) {  //  If number
      AddNumToStack(result_str, i, leks, numlist);
    } else if (result_str[i] == 'P') {  //  If PI
      AddPiToStack(i, leks, numlist);
    } else if (strchr("sctlm", result_str[i]) &&
               (result_str[i] != '\0')) {  //  If cos/sin/tan/log/mod
      AddOperatorToStack(result_str, i, leks, operatorlist);
    } else if (result_str[i] == 'a') {  //  if acos/asin/atan
      AddAnotherOperatorToStack(result_str, i, leks, operatorlist);
    } else if (result_str[i] == '(') {  //  If bracket (
      AddOpeningBracketToStack(result_str, i, leks, operatorlist);
    } else if (result_str[i] == ')') {  //  If bracket )
      if (AddClosingBracketToStack(i, operatorlist, numlist)) {
        throw std::invalid_argument("Invalid input: too many closing brackets");
      }
    } else if ((result_str[i] == '+' || result_str[i] == '-' ||
                result_str[i] == '*' || result_str[i] == '/' ||
                result_str[i] == '^')) {
      if (AddSignToStack(result_str, i, leks, operatorlist, numlist)) {
        throw std::invalid_argument(
            "Invalid input: there is incorrect math action");
      }
    } else if (result_str[i] == '\0') {
      flag = true;
    }
    // if someone try to calculate Error, nan, inf
    else if ((result_str[i] == 'E' || result_str[i] == 'n' ||
              result_str[i] == 'i' || result_str[i] == 'x')) {
      throw std::invalid_argument(
          "Invalid input: cannot calculate error, NaN, or inf");
    } else {
      throw std::invalid_argument("Invalid input: unrecognized character");
    }
  }
  //  Вызываем матем. функцию до тех пор, пока в стеке с операциями не будет 0
  //  элементов
  while (operatorlist.size() != 0) {
    if (CallMathFun(operatorlist, numlist)) {
      throw std::invalid_argument(
          "Invalid input: there is incorrect math action");
    }
  }
  if (numlist.size() != 1) {
    throw std::invalid_argument(
        "Invalid input: operation not equal to numbers");
  }
  return numlist.top().number;
}

// Делаем красивую строку на выходе (удаляем нули, обрубаем до 7 знаков)
void SmartCalculator::FixString(std::string &buf, double res_number) {
  if (std::isnan(res_number) || std::isinf(res_number)) {
    buf = "Error";
  } else {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(7) << res_number;
    buf = oss.str();
    size_t dot_pos = buf.find('.');
    if (dot_pos != std::string::npos) {
      size_t last_nonzero_pos = buf.find_last_not_of('0');
      if (last_nonzero_pos ==
          dot_pos) {  // If all digits after the decimal point are zero
        buf.erase(last_nonzero_pos);  // Remove the decimal point as well
      } else {
        buf.erase(last_nonzero_pos + 1,
                  std::string::npos);  // Remove the trailing zeroes after the
                                       // decimal point
      }
    }
  }
}

void SmartCalculator::PushNumberPopOperator(std::stack<Lexsema> &stack_n,
                                            std::stack<Lexsema> &stack_o,
                                            double num, Lexsema &item) {
  item.type = '0';
  item.number = num;
  stack_n.push(item);
  if (!stack_o.empty()) {
    stack_o.pop();
  }
}

bool SmartCalculator::Maths(std::stack<Lexsema> &stack_n,
                            std::stack<Lexsema> &stack_o) {
  constexpr double kEpsilon = 0.0000001;
  Lexsema item;
  bool error = false;
  double first_number, second_number, result_number;
  first_number =
      stack_n.top().number;  //  Берется верхнее число из стека с числами
  stack_n.pop();  //  Удаляется верхнее число из стека с числами
  switch (
      stack_o.top()
          .type) {  //  Проверяется тип верхней операции из стека с операциями
    case '+':  //  Если тип верхней операции из стека с операциями сложение
      second_number = stack_n.top().number;
      stack_n.pop();
      result_number = first_number + second_number;
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case '-':
      second_number = stack_n.top().number;
      stack_n.pop();
      result_number = second_number - first_number;
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case '^':
      second_number = stack_n.top().number;
      stack_n.pop();
      result_number = pow(second_number, first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;

    case '*':
      second_number = stack_n.top().number;
      stack_n.pop();
      result_number = first_number * second_number;
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;

    case '/':
      second_number = stack_n.top().number;
      stack_n.pop();
      if (first_number == 0) {
        error = true;
      } else {
        result_number = (second_number / first_number);
        PushNumberPopOperator(stack_n, stack_o, result_number, item);
      }
      break;
    case 's':
      result_number = std::sin(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'c':
      result_number = std::cos(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 't':
      if (fabs(cos(first_number)) <= kEpsilon) {
        error = true;
      } else {
        result_number = std::tan(first_number);
        if (fabs(result_number) <= kEpsilon) result_number = 0;
        PushNumberPopOperator(stack_n, stack_o, result_number, item);
      }
      break;
    case 'q':
      result_number = std::sqrt(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'l':
      result_number = std::log10(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'n':
      result_number = std::log(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'm':
      second_number = stack_n.top().number;
      stack_n.pop();
      result_number = fmod(second_number, first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'a':
      result_number = std::asin(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'x':
      result_number = std::acos(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    case 'z':
      result_number = std::atan(first_number);
      PushNumberPopOperator(stack_n, stack_o, result_number, item);
      break;
    default:
      error = true;
      break;
  }
  return error;
}

// Опредение приоритета операций
int SmartCalculator::GetRang(char Ch) {
  return Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'q' || Ch == 'n' ||
                 Ch == 'm' || Ch == 'l' || Ch == 'a' || Ch == 'x' || Ch == 'z'
             ? 4
         : Ch == '^'              ? 3
         : Ch == '+' || Ch == '-' ? 1
         : Ch == '*' || Ch == '/' ? 2
                                  : 0;  // unrecognized operator
}

//  Добавляет знак в стек
bool SmartCalculator::AddSignToStack(const std::string &str, size_t &i,
                                     Lexsema &leks,
                                     std::stack<Lexsema> &operatorlist,
                                     std::stack<Lexsema> &numlist) {
  bool res = false;
  if (!operatorlist.empty() &&
      GetRang(str[i]) <= GetRang(operatorlist.top().type)) {
    res = CallMathFun(operatorlist, numlist);
  } else {
    leks.type = str[i];
    leks.number = 0;
    operatorlist.push(leks);
    i++;
    if (str[i] == '+' || str[i] == '-') {
      i++;
    }
  }
  return res;
}

bool SmartCalculator::CallMathFun(std::stack<Lexsema> &operatorlist,
                                  std::stack<Lexsema> &numlist) {
  bool error = false;
  try {
    error = Maths(numlist, operatorlist);
  } catch (...) {
    error = true;
  }
  return error;
}

//  Добавляет число в стек
void SmartCalculator::AddNumToStack(const std::string &str, size_t &i,
                                    Lexsema &leks,
                                    std::stack<Lexsema> &numlist) {
  double num = GetNum(str, i);
  leks.type = 0;
  leks.number = num;
  numlist.push(leks);
}

//  Добавляет число Пи в стек
void SmartCalculator::AddPiToStack(size_t &i, Lexsema &leks,
                                   std::stack<Lexsema> &numlist) {
  leks.number = Pi;
  leks.type = 0;
  numlist.push(leks);
  i += 2;
}

//  Добавляет скобку в стек
void SmartCalculator::AddOpeningBracketToStack(
    const std::string &str, size_t &i, Lexsema &leks,
    std::stack<Lexsema> &operatorlist) {
  leks.type = str[i];
  leks.number = 0;
  operatorlist.push(leks);
  i++;
  if (str[i] == '+') {
    i++;
  }
}

//  Обрабатывает действия в скобках
bool SmartCalculator::AddClosingBracketToStack(
    size_t &i, std::stack<Lexsema> &operatorlist,
    std::stack<Lexsema> &numlist) {
  bool res = false;
  while (!operatorlist.empty() && operatorlist.top().type != '(' &&
         res == false) {
    if (Maths(numlist, operatorlist)) {  //  Если функция вернет "false", то
                                         //  прекращаем работу
      res = true;
    }
  }
  if (res == false) {
    operatorlist.pop();
  }
  i++;
  return res;
  ;
}

//  Добавляет оператор в стек (cos/sin/tan/log/mod)
void SmartCalculator::AddOperatorToStack(const std::string &str, size_t &i,
                                         Lexsema &leks,
                                         std::stack<Lexsema> &operatorlist) {
  switch (str[i + 1]) {
    case 'q':
      leks.type = 'q';
      i += 1;
      break;
    case 'n':
      leks.type = 'n';
      i -= 1;
      break;
    default:
      leks.type = str[i];
      break;
  }
  leks.number = 0;
  operatorlist.push(leks);
  i += 3;
}

//  Добавляет оставшиеся операторы(функции) в стек (acos/asin/atan)
void SmartCalculator::AddAnotherOperatorToStack(
    const std::string &str, size_t &i, Lexsema &leks,
    std::stack<Lexsema> &operatorlist) {
  switch (str[i + 1]) {
    case 's':
      leks.type = str[i];  // asin
      break;
    case 'c':
      leks.type = 'x';  // acos
      break;
    default:
      leks.type = 'z';  // atan
      break;
  }
  leks.number = 0;
  operatorlist.push(leks);
  i += 4;
}

// Получение числа из строки
double SmartCalculator::GetNum(const std::string &str, size_t &i) {
  double res;
  bool flag = true;
  std::string lexsema;
  // Skip leading whitespace
  while (i < str.length() && std::isspace(str[i])) {
    i++;
  }
  for (size_t j = i; j <= str.length() && flag; j++) {
    if (j != str.length() &&
        ((str[j] >= '0' && str[j] <= '9') || str[j] == '.')) {
      lexsema.push_back(str[j]);
    } else {
      flag = false;
    }
    i = j;
  }
  // Skip trailing whitespace
  while (i < str.length() && std::isspace(str[i])) {
    i++;
  }
  try {
    res = std::stod(lexsema);
  } catch (const std::invalid_argument &) {
    // Handle invalid input (e.g. return 0.0)
    res = 0.0;
  } catch (const std::out_of_range &) {
    // Handle out-of-range input (e.g. return infinity)
    res = std::numeric_limits<double>::infinity();
  }
  return res;
}

// Проверяем строку на корректность и добавляем скобки ')' (если нужно)
void SmartCalculator::CheckInput(std::string &str) {
  if (str.length() > 255) {
    throw std::invalid_argument("Invalid input");
  }
  if (!IsFunction(str) || !IsValidExpression(str)) {
    throw std::invalid_argument("Invalid input");
  }
  str = regex_replace(str, std::regex("E"), "*(10)^");
  int temp = CountBrack(str, '(') - CountBrack(str, ')');
  if (str.find_last_of("+-*/^d(E") == str.length() - 1 ||
      str.find_first_of("mE*/") == 0 || temp < 0) {
    throw std::invalid_argument("Invalid input");
  }
  while (temp != 0) {
    str += ")";
    temp--;
  }
  ChangeSign(str);
  if (!IsValidCombOperator(str)) {
    throw std::invalid_argument("Invalid input");
  }
}

// Вспомогательная функция к CheckInput, считает кол-во элементов
int SmartCalculator::CountBrack(const std::string &str, char bracket) const {
  return std::count(str.begin(), str.end(), bracket);
}

// Обработка нескольких знаков
void SmartCalculator::ChangeSign(std::string &str) {
  // Replace multiple '-' signs with '+'
  std::regex minus_regex("\\--");
  str = std::regex_replace(str, minus_regex, "+");

  // Replace multiple '+' signs with '+'
  std::regex plus_regex("\\++");
  str = std::regex_replace(str, plus_regex, "+");

  // Replace '+-' with '-'
  std::regex plus_minus_regex("\\+-");
  str = std::regex_replace(str, plus_minus_regex, "-");

  // Replace '-+' with '-'
  std::regex minus_plus_regex("\\-\\+");
  str = std::regex_replace(str, minus_plus_regex, "-");

  // Insert '0' after '(' followed by a '-' sign
  std::regex opening_parenthesis_regex("\\(-");
  str = std::regex_replace(str, opening_parenthesis_regex, "(0-");

  // Insert '0' at the beginning if the string starts with a sign
  if (str.size() > 1 && (str[0] == '-' || str[0] == '+')) {
    str.insert(0, "0");
  }
}

// Проверка корректности написания функций
bool SmartCalculator::IsFunction(const std::string &str) {
  std::set<std::string> validFunctions{"sin",  "cos",  "tan", "asin",
                                       "acos", "atan", "log", "mod",
                                       "ln",   "Pi",   "E",   "sqrt"};
  std::string currentFunction;
  for (size_t i = 0; i < str.size(); i++) {
    if (std::isalpha(str[i])) {
      currentFunction += str[i];
    } else if (currentFunction.size() > 0) {
      if (validFunctions.count(currentFunction) == 1) {
        currentFunction = "";
      } else {
        return false;
      }
    }
  }
  return true;
}

// Проверка, что нет недопустимых символов в строке
bool SmartCalculator::IsValidExpression(const std::string &str) {
  std::string validSymbols = "+-*/^().0123456789";
  std::string currentFunction;
  for (const char &c : str) {
    if (!std::isalpha(c) && (validSymbols.find(c) == std::string::npos)) {
      return false;
    }
  }
  return true;
}

// Проверка на недопустимые комбинации операторов
bool SmartCalculator::IsValidCombOperator(const std::string &str) {
  std::string operators = "*/^+-";
  for (size_t i = 0; i < str.length(); i++) {
    if (operators.find(str[i]) != std::string::npos) {
      if (i == 0 || i == str.length() - 1) {
        // Operator at beginning or end of string
        return false;
      } else if (operators.find(str[i - 1]) != std::string::npos ||
                 operators.find(str[i + 1]) != std::string::npos) {
        // Two operators in a row
        return false;
      }
    }
  }
  return true;
}

}  // namespace s21
