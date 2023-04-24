"""
This module contains unit tests for the Calculator class in model.calculator.

It tests various arithmetic operations such as addition, subtraction,
multiplication, division, exponentiation, and modulus operator.
It also tests trigonometric functions (sin, cos, tan, asin, acos, atan),
logarithmic functions (natural logarithm and base-10 logarithm),
and negative numbers. Additionally, it tests error cases such as division
by zero, invalid expressions, missing operands, invalid characters, invalid
logarithmic functions, and unbalanced parentheses.
"""

import unittest
from model.calculator import Calculator  # pylint: disable=import-error


class TestSmartCalculator(unittest.TestCase):  # pylint: disable=R0904
  """ A test case for the `Calculator` class. """

  def test_simple_addition(self):
    """ Test that the `calculate` method can perform simple addition. """
    calculator = Calculator()
    expression = "1+2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "3")

  def test_simple_subtraction(self):
    """ Test that the `calculate` method can perform simple subtraction. """
    calculator = Calculator()
    expression = "10-5"
    result = calculator.calculate(expression)
    self.assertEqual(result, "5")

  def test_simple_multiplication(self):
    """ Test that the `calculate` method can perform simple multiplication. """
    calculator = Calculator()
    expression = "2*3"
    result = calculator.calculate(expression)
    self.assertEqual(result, "6")

  def test_simple_division(self):
    """ Test that the `calculate` method can perform simple division. """
    calculator = Calculator()
    expression = "10/2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "5")

  def test_simple_exponentiation(self):
    """ Test that the `calculate` method can perform simple exponentiation. """
    calculator = Calculator()
    expression = "10^2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "100")

  def test_complex_expression(self):
    """ Test that the `calculate` method can evaluate complex expressions. """
    calculator = Calculator()
    expression = "(2+3)*(4-1)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "15")

  def test_division_by_zero(self):
    """ Test that the `calculate` method returns "Error" when division
      by zero is attempted. """
    calculator = Calculator()
    expression = "10/0"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_expression(self):
    """ Test that the `calculate` method returns "Error" when given
      an invalid expression. """
    calculator = Calculator()
    expression = "1 + 2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_negative_numbers(self):
    """ Test that the `calculate` method can handle negative numbers. """
    calculator = Calculator()
    expression = "-2*3+5"
    result = calculator.calculate(expression)
    self.assertEqual(result, "-1")

  def test_trigonometric_functions(self):
    """  Test that the `calculate` method can handle trigonometric
      functions. """
    calculator = Calculator()
    expression = "sin(0.5)+cos(0.5)+tan(0.5)+asin(0.5)+acos(0.5)+atan(0.5)"
    result = calculator.calculate(expression)
    self.assertAlmostEqual(float(result), 3.9377545, places=7)

  def test_logarithmic_functions(self):
    """ Test that the `calculate` method can handle logarithmic functions. """
    calculator = Calculator()
    expression = "ln(10)+log(10)"
    result = calculator.calculate(expression)
    self.assertAlmostEqual(float(result), 3.30258509299, places=7)

  def test_modulus_operator(self):
    """ Test that the `calculate` method can handle the modulus operator. """
    calculator = Calculator()
    expression = "7mod3"
    result = calculator.calculate(expression)
    self.assertEqual(result, "1")

  def test_complex_expressions(self):
    """ Test that the calculator can evaluate a complex expression. """
    calculator = Calculator()
    expression = "sin(0.5)^2+cos(0.5)^2-tan(0.5)^2+log(10)*(3+2)-7mod3"
    result = calculator.calculate(expression)
    self.assertAlmostEqual(float(result), 4.70155358959, places=7)

  def test_missing_operand(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains a missing operand.
    """
    calculator = Calculator()
    expression = "3+"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_characters(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains invalid characters.
    """
    calculator = Calculator()
    expression = "2x+3"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_log_function(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains an invalid logarithm function.
    """
    calculator = Calculator()
    expression = "2 + log(0)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_modulo_by_zero(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains a modulus operation with a divisor of zero.
    """
    calculator = Calculator()
    expression = "5mod0"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_unbalanced_parentheses(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains unbalanced parentheses.
    """
    calculator = Calculator()
    expression = "2 * (3 + 4))"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_asin(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains an arcsine function without a complete argument.
    """
    calculator = Calculator()
    expression = "asin(13)/"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_ln(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains a natural logarithm function with a negative argument.
    """
    calculator = Calculator()
    expression = "ln(-1)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_log(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains a logarithm function with a negative argument.
    """
    calculator = Calculator()
    expression = "log(-1)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_complex_expression1(self):
    """ Test that the calculator can evaluate a complex expression. """
    calculator = Calculator()
    expression = "(-3.14159*log(0.6))*ln(sin(0.5"
    result = calculator.calculate(expression)
    self.assertAlmostEqual(float(result), -0.5123802)

  def test_complex_expression2(self):
    """ Test that the calculator returns an "Error" message when the expression
        contains too many closing parentheses.
    """
    calculator = Calculator()
    expression = "(-3.14159*log(0.6))*ln(sin(0.5))))))"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_complex_expression3(self):
    """ Test that the calculator can evaluate a complex expression containing
        exponential notation.
    """
    calculator = Calculator()
    expression = "(1E5/10-1E(-3))"
    result = calculator.calculate(expression)
    self.assertAlmostEqual(float(result), 9999.999)

  def test_minus_one(self):
    """ Test that the calculator can handle a negative integer input. """
    calculator = Calculator()
    expression = "-1"
    result = calculator.calculate(expression)
    self.assertEqual(result, "-1")

  def test_plus_ten(self):
    """ Test that the calculator can handle a positive integer input
        with a plus sign. """
    calculator = Calculator()
    expression = "+10"
    result = calculator.calculate(expression)
    self.assertEqual(result, "10")

  def test_multiple_plus_minus(self):
    """ Test that the calculator can handle multiple consecutive plus
        and minus signs. """
    calculator = Calculator()
    expression = "--1+2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "3")

  def test_sqrt(self):
    """ Test that the calculator returns an error message when trying to
        take the square root of a negative number.
    """
    calculator = Calculator()
    expression = "sqrt(-1)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_expression1(self):
    """ Test that the calculator returns an error message when encountering
        an invalid expression.
    """
    calculator = Calculator()
    expression = "1*+2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_expression2(self):
    """ Test that the calculator returns an error message when encountering
        an invalid expression containing an unknown function.
    """
    calculator = Calculator()
    expression = "si(1)"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_expression3(self):
    """ Test that the calculator returns an error message when encountering
        an invalid expression containing an unexpected symbol.
    """
    calculator = Calculator()
    expression = "1#2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")

  def test_invalid_expression4(self):
    """ Test that the calculator returns an error message when encountering
        an invalid expression.
    """
    calculator = Calculator()
    expression = "1/*2"
    result = calculator.calculate(expression)
    self.assertEqual(result, "Error")


if __name__ == "__main__":
  unittest.main()
