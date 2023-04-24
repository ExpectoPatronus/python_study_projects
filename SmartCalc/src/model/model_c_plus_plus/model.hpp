#ifndef SRC_MODEL_MODEL_HPP_
#define SRC_MODEL_MODEL_HPP_

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <regex>
#include <set>
#include <sstream>
#include <stack>

namespace s21 {

class SmartCalculator {
 private:
  const double Pi = acos(-1);
  struct Lexsema {
    char type;
    double number;
  };

 public:
  SmartCalculator() = default;
  ~SmartCalculator() = default;
  void Calculate(std::string &res, const std::string &str);

 private:
  void AddNumToStack(const std::string &str, size_t &i, Lexsema &leks,
                     std::stack<Lexsema> &numlist);
  double GetNum(const std::string &str, size_t &i);
  bool AddSignToStack(const std::string &str, size_t &i, Lexsema &leks,
                      std::stack<Lexsema> &operatorlist,
                      std::stack<Lexsema> &numlist);
  int GetRang(char Ch);
  bool CallMathFun(std::stack<Lexsema> &operatorlist,
                   std::stack<Lexsema> &numlist);
  bool Maths(std::stack<Lexsema> &stack_n, std::stack<Lexsema> &stack_o);
  void PushNumberPopOperator(std::stack<Lexsema> &stack_n,
                             std::stack<Lexsema> &stack_o, double num,
                             Lexsema &item);
  void FixString(std::string &buf, double res_number);
  void AddPiToStack(size_t &i, Lexsema &leks, std::stack<Lexsema> &numlist);
  void AddOpeningBracketToStack(const std::string &str, size_t &i,
                                Lexsema &leks,
                                std::stack<Lexsema> &operatorlist);
  bool AddClosingBracketToStack(size_t &i, std::stack<Lexsema> &operatorlist,
                                std::stack<Lexsema> &numlist);
  void AddAnotherOperatorToStack(const std::string &str, size_t &i,
                                 Lexsema &leks,
                                 std::stack<Lexsema> &operatorlist);
  void AddOperatorToStack(const std::string &str, size_t &i, Lexsema &leks,
                          std::stack<Lexsema> &operatorlist);
  void CheckInput(std::string &str);
  int CountBrack(const std::string &str, char bracket) const;
  void ChangeSign(std::string &str);
  double Parser(std::string &result_str);
  bool IsFunction(const std::string &token);
  bool IsValidExpression(const std::string &str);
  bool IsValidCombOperator(const std::string &str);
};
}  // namespace s21

#endif  // SRC_MODEL_HPP_
