"""
    This module contains the implementation of a simple calculator
    GUI using PyQt6.
"""

# pylint: disable=no-name-in-module
import os
from PyQt6.QtWidgets import QMainWindow, QMessageBox, QInputDialog
from PyQt6 import uic
from PyQt6.QtCore import QSettings
from PyQt6.QtGui import QAction

# pylint: disable=import-error
from view.graph_window import GraphWindow
from presenter.presenter import Presenter

MAX_VALUE_X = 1e7

path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
ui_file = path + '/view/ui_interface/main_window.ui'
Ui_MainWindow, QtBaseClass = uic.loadUiType(ui_file)


class MainWindow(QMainWindow, Ui_MainWindow):
  """ The main window of the calculator application.

  This class is responsible for handling user input and displaying output.

  Attributes:
      presenter (Presenter): The presenter object that handles user input.
  """

  def __init__(self):
    """ Initializes the main window."""
    super().__init__()
    self.setupUi(self)
    self.presenter = Presenter(self)
    self.add_functions()
    self.load_history()
    self.add_help_menu()

  def add_functions(self):
    """ Connects button signals to the presenter methods."""
    buttons = [self.pushButton_0, self.pushButton_1, self.pushButton_2,
               self.pushButton_3, self.pushButton_4, self.pushButton_5,
               self.pushButton_6, self.pushButton_7, self.pushButton_8,
               self.pushButton_9, self.pushButton_add, self.pushButton_sub,
               self.pushButton_mul, self.pushButton_div, self.pushButton_dot,
               self.pushButton_open_br, self.pushButton_close_br,
               self.pushButton_exp,
               self.pushButton_x, self.pushButton_mod]

    buttons_function = [self.pushButton_sin, self.pushButton_cos,
                        self.pushButton_tan,
                        self.pushButton_asin, self.pushButton_acos,
                        self.pushButton_atan,
                        self.pushButton_ln, self.pushButton_log,
                        self.pushButton_sqrt]

    for button in buttons:
      button.clicked.connect(
        lambda checked, b=button: self.presenter.write_number(b.text()))

    for button in buttons_function:
      button.clicked.connect(
        lambda checked, b=button: self.presenter.write_number(b.text() + '('))

    self.pushButton_equal.clicked.connect(
      lambda: self.presenter.get_result(self.text_edit.toPlainText()))
    self.pushButton_pi.clicked.connect(
      lambda checked: self.presenter.write_number('Pi'))
    self.pushButton_exponentiation.clicked.connect(
      lambda checked: self.presenter.write_number('^'))
    self.pushButton_ac.clicked.connect(self.presenter.clear_result)
    self.pushButton_c.clicked.connect(self.presenter.clear_entry)
    self.pushButton_graph.clicked.connect(self.presenter.open_graphic)
    self.pushButton_clean_histoty.clicked.connect(self.presenter.clear_history)
    self.pushButton_load_expres.clicked.connect(
      self.presenter.load_string_from_history)

  def add_help_menu(self):
    """ Adds a 'Help' menu to the menu bar."""
    help_action = QAction('Help', self)
    help_action.triggered.connect(self.show_help_dialog)
    menu_bar = self.menuBar()
    help_menu = menu_bar.addMenu('Help')
    help_menu.addAction(help_action)

  def show_help_dialog(self):
    """ Shows a dialog box with information about the application."""
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    view_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..',
                             'view')
    info_path = os.path.join(view_dir, 'info.txt')
    with open(info_path, 'r', encoding='UTF-8') as file:
      help_text = file.read()
    message = '<font size = 5 color = gray > Information about the' \
            ' Calculator app </font> <br/><br/>In the Calculator' \
            ' application, you can perform simple or complex calculations,' \
            ' as well as draw graphs of functions. <br/><br/>'
    msg = QMessageBox()
    msg.setIcon(QMessageBox.Icon.Information)
    msg.setInformativeText(message)
    msg.setDetailedText(f'{help_text}')
    msg.addButton(QMessageBox.StandardButton.Ok)
    msg.exec()

  def clear_input_field(self):
    """ Clears the input field."""
    self.text_edit.setPlainText('0')

  def get_text_edit_text(self):
    """ Returns the text in the input field."""
    return self.text_edit.toPlainText()

  def set_text_edit_text(self, text):
    """ Sets the text in the input field.

    Args:
        text (str): The text to set in the input field.
    """
    self.text_edit.setPlainText(text)

  def add_to_history(self, expression):
    """ Adds an expression to the history list.

    Args:
        expression (str): The expression to add to the history list.
    """
    self.list_history.addItem(expression)

  def clear_history(self):
    """ Clears the history list."""
    self.list_history.clear()

  def load_string_from_history(self):
    """ Loads the selected expression from the history list into
        the input field."""
    selected_item = self.list_history.currentItem()
    if selected_item is not None:
      self.text_edit.setPlainText(selected_item.text().split(' = ')[0])

  def save_history(self):
    """ Saves the history list to the application settings."""
    settings = QSettings('MySoft', 'MyCalculator')
    history = []
    for i in range(self.list_history.count()):
      history.append(self.list_history.item(i).text())
    settings.setValue('history', history)

  def load_history(self):
    """ Loads the history list from the application settings."""
    settings = QSettings('MySoft', 'MyCalculator')
    history = settings.value('history')
    if history is not None:
      for item in history:
        self.add_to_history(item)

  def clear_entry(self):
    """ Clears the last character in the input field."""
    self.text_edit.setPlainText(
      self.text_edit.toPlainText()[
      :len(self.text_edit.toPlainText()) - self.backspace()])

  def backspace(self):
    """ Returns the number of characters to remove when
        the backspace button is clicked."""
    res = 1
    expression = self.text_edit.toPlainText()
    ends = ['asin(', 'atan(', 'acos(', 'cos(', 'sin(', 'tan(', 'Pi', 'mod',
            'sqrt(', 'log(',
            'ln(', 'Error']
    for end in ends:
      if expression.endswith(end):
        res = len(end)
        break
    return res

  def show_x_dialog(self):
    """ Shows a dialog box for the user to enter a value for X."""
    x_value, ok_button = QInputDialog.getDouble(self, 'Ввод X',
                                                'Введите значение X:',
                                                decimals=7,
                                                min=-MAX_VALUE_X,
                                                max=MAX_VALUE_X)
    if ok_button:
      return x_value
    return None

  def open_graphic(self):
    """ Opens a new window to display a graph of the input expression."""
    window = GraphWindow(self.text_edit.toPlainText(), parent=self)
    window.show()

  def closeEvent(self, event):  # pylint: disable=invalid-name
    """ Handles the window close event."""
    self.save_history()
    event.accept()
