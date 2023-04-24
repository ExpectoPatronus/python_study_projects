"""
    This module provides a Calculator class for performing
    arithmetic calculations.
"""
import os
import ctypes
import ctypes.util


class Calculator:  # pylint: disable=too-few-public-methods
  """ A simple calculator that can evaluate arithmetic expressions
        containing numbers, operators, and functions. A class
        for performing arithmetic calculations using a shared library.
   """

  def __init__(self):
    """Initializes a new instance of the Calculator class."""
    lib_path = os.path.abspath(
      os.path.join(os.path.dirname(__file__), '..', 'model',
                   'libcalculator.dylib'))
    if not os.path.exists(lib_path):
      raise RuntimeError(f'Shared library not found: {lib_path}')

    # Load library
    self.lib = ctypes.cdll.LoadLibrary(lib_path)
    # We specify that the function takes two arguments char*
    self.lib.CalculateWrapper.argtypes = [ctypes.POINTER(ctypes.c_char),
                                          ctypes.POINTER(ctypes.c_char)]

  def calculate(self, expression):
    """
    Evaluates an arithmetic expression and returns the result.

    Args:
        expression (str): The arithmetic expression to evaluate.

    Returns:
        str: The result of the arithmetic expression as a string.
    """
    # The string must be converted to an array of bytes.
    # Then a b'\0' is appended to the end of the byte sequence
    # to indicate the end of the string
    expression_str = expression.encode('ascii') + b'\0'
    # A 256 byte buffer is created in the method.
    result_buf = ctypes.create_string_buffer(256)
    self.lib.CalculateWrapper(result_buf, expression_str)
    # The resulting byte array is reduced to a string.
    result = result_buf.value.decode('ascii')
    return result
