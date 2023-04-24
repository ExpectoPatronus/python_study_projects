""" The presenter acts upon the model and the view.
    It retrieves data from repositories (the model),
    and formats it for display in the view.
"""

from model.calculator import Calculator  # pylint: disable=import-error


class Presenter:
  """ A presenter class that serves as a bridge between view object
    and model (calculator).
  """

  def __init__(self, view):
    """ Constructs a new Presenter object.

      Args:
          view (object): The view object to interact with.
    """
    self.view = view
    self.calculator = Calculator()

  def calculate_result(self, expression):
    """ Calculates the result of the given expression using calculator object.

        Args:
            expression (str): The mathematical expression to evaluate.

        Returns:
            str: The result of the calculation, or 'Error' if an exception
                occurred.
    """
    try:
      result = self.calculator.calculate(expression)
      return str(result)
    except (ValueError, TypeError, IndexError):
      return "Error"

  def get_result(self, expression):
    """ Calculates the result of the given expression and displays it in the
        view object's history. If the expression contains the variable 'x',
        prompts the user to enter a value for 'x' and replaces all occurrences
        of 'x' with the entered value before calculating the result.

        Args:
            expression (str): The mathematical expression to evaluate.

    """
    if "x" in expression:
      x_value = self.view.show_x_dialog()
      if x_value is not None:
        result = self.calculator.calculate(
          expression.replace("x", str(x_value)))
        self.view.add_to_history(f"{expression} = {result}; x = {x_value}")
        self.view.set_text_edit_text(str(result))
    else:
      result = self.calculator.calculate(expression)
      self.view.add_to_history(f"{expression} = {result}")
      self.view.set_text_edit_text(str(result))

  def clear_result(self):
    """ Clears the view object's input field. """
    self.view.clear_input_field()

  def write_number(self, number):
    """ Appends the given number to the view object's input field.
        If the input field currently contains '0' or 'Error', replaces
        it with the given number instead.

        Args:
            number (str): The number to append to the input field.
    """
    current_text = self.view.get_text_edit_text()
    if current_text == "Error" or (
        current_text == "0" and number not in (
        "-", "+", "*", "/", ".", "^", "E")):
      self.view.set_text_edit_text(number)
    else:
      self.view.set_text_edit_text(current_text + number)

  def clear_history(self):
    """ Clears the view object's history display. """
    self.view.clear_history()

  def load_string_from_history(self):
    """ Loads the currently selected item from the view object's history display
        into the input field.
    """
    self.view.load_string_from_history()

  def clear_entry(self):
    """ Clears the last entered character from the view object's input field.
    """
    self.view.clear_entry()

  def open_graphic(self):
    """ Opens a graphic display.
    """
    self.view.open_graphic()
