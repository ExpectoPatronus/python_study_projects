#include <gtest/gtest.h>

#include "model.hpp"

TEST(CalculateTest, SimpleAddition) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "1+2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "3");
}

TEST(CalculateTest, SimpleSubtraction) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "10-5";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "5");
}

TEST(CalculateTest, SimpleMultiplication) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "2*3";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "6");
}

TEST(CalculateTest, SimpleDivision) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "10/2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "5");
}

TEST(CalculateTest, SimpleExponentiation) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "10^2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "100");
}

TEST(CalculateTest, ComplexExpression) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "(2+3)*(4-1)";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "15");
}

TEST(CalculateTest, DivisionByZero) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "10/0";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, InvalidExpression) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "1 + 2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, NegativeNumbers) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "-2*3+5";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "-1");
}

TEST(CalculateTest, TrigonometricFunctions) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression =
      "sin(0.5)+cos(0.5)+tan(0.5)+asin(0.5)+acos(0.5)+atan(0.5)";
  calculator.Calculate(result, expression);
  ASSERT_NEAR(std::stod(result), 3.9377545, 0.0000001);
}

TEST(CalculateTest, LogarithmicFunctions) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "ln(10)+log(10)";
  calculator.Calculate(result, expression);
  ASSERT_NEAR(std::stod(result), 3.30258509299, 0.0000001);
}

TEST(CalculateTest, ModulusOperator) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "7mod3";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "1");
}

TEST(CalculateTest, ComplexExpressions) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression =
      "sin(0.5)^2+cos(0.5)^2-tan(0.5)^2+log(10)*(3+2)-7mod3";
  calculator.Calculate(result, expression);
  ASSERT_NEAR(std::stod(result), 4.70155358959, 0.000001);
}

TEST(CalculateTest, MissingOperand) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "3+";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, InvalidCharacters) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "2x+3";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, InvalidLogFunction) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "2 + log(0)";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, ModuloByZero) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "5mod0";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, UnbalancedParentheses) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "2 * (3 + 4))";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, AnotherTest1) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "asin(sqrt(-1))";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest2) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "asin(13)/0";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest3) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "1+2+3+4+5+6+7+8+9+10";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "55");
}

TEST(CalculateTest, AnotherTest4) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "-1-2-3-4-5-6-7-8-9-10";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "-55");
}

TEST(CalculateTest, AnotherTest5) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "--(1++2+-3-+4*5/6+7^8+9mod10)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "5764806.6666667");
}

TEST(CalculateTest, AnotherTest6) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch =
      "1------------------------------------------------"
      "-------------------------------------------------------------------"
      "--------------------------------------------------------------------"
      "----------------------------------------------------------------------"
      "----------------------------------------2";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "-1");
}

TEST(CalculateTest, AnotherTest7) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch =
      "1-----------------------------------------------------"
      "------------------------------------------------------------------------"
      "------------------------------------------------------------------------"
      "-"
      "------------------------------------------------------------------------"
      "------------------------2";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "3");
}

TEST(CalculateTest, AnotherTest8) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "cos(-1.5)^2";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "0.0050038");
}

TEST(CalculateTest, AnotherTest9) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "cos(1.5)^2*sin(1.5)^2";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "0.0049787");
}

TEST(CalculateTest, AnotherTest10) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "sqrt(cos(Pi)+sin(Pi)+tan(Pi))";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest11) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "acos(1)+asin(1)+atan(1)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "2.3561945");
}

TEST(CalculateTest, AnotherTest12) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "ln(110)/log(16)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "3.9036644");
}

TEST(CalculateTest, AnotherTest13) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "asin(13)/";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest14) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "ln(-1)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest15) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "log(-1)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest16) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "(-Pi*log(0.6))*ln(sin(0.5";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "-0.5123806");
}

TEST(CalculateTest, AnotherTest17) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "(-Pi*log(0.6))*ln(sin(0.5))))))";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest18) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "(1E5/10-1E(-3))";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "9999.999");
}

TEST(CalculateTest, AnotherTest19) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "-1";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "-1");
}

TEST(CalculateTest, AnotherTest20) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "+10";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "10");
}

TEST(CalculateTest, AnotherTest21) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "--1+2";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "3");
}

TEST(CalculateTest, AnotherTest22) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "sqrt(-1)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest23) {
  s21::SmartCalculator m;
  std::string f;
  const std::string ch = "log(-1)";
  m.Calculate(f, ch);
  ASSERT_EQ(f, "Error");
}

TEST(CalculateTest, AnotherTest24) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "1*+2";  // this like 1*(+2)
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "2");
}

TEST(CalculateTest, AnotherTest25) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "si(1)";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}
TEST(CalculateTest, AnotherTest26) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "1#2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

TEST(CalculateTest, AnotherTest27) {
  s21::SmartCalculator calculator;
  std::string result;
  std::string expression = "1/*2";
  calculator.Calculate(result, expression);
  ASSERT_EQ(result, "Error");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
