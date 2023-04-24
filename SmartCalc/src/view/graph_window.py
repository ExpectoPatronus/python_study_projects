""" This module defines the GraphWindow class that plots
    a given mathematical expression using matplotlib library.
"""

import numpy as np
# pylint: disable=no-name-in-module
from PyQt6.QtWidgets import QMainWindow, QApplication, QStatusBar, QLabel
from matplotlib.backends.backend_qtagg import NavigationToolbar2QT, \
  FigureCanvasQTAgg
from matplotlib.figure import Figure
from matplotlib.ticker import AutoMinorLocator, AutoLocator

from presenter.presenter import Presenter  # pylint: disable=import-error

# The range of definition and the range of value of the functions are limited
MAX_VALUE_AREA = 1e6


class GraphWindow(QMainWindow):
  """The main window of the application that displays the graph."""

  def __init__(self, expression, parent=None):
    """Initializes the GraphWindow object.

    Args:
        expression (str): The mathematical expression to be plotted.
        parent (QWidget, optional): The parent widget of the window.
                                    Defaults to None.
    """
    super().__init__(parent=parent)
    self.presenter = Presenter(self)
    self.expression = expression
    self.create_window()
    self.setup_graph_window()
    self.add_arrows()
    self.add_grid()
    self.update_scale_label()
    self.data['x_lim'] = self.data['axis'].get_xlim()
    self.data['y_lim'] = self.data['axis'].get_ylim()
    self.canvas.mpl_connect('draw_event', self.update_plot)

  def create_window(self):
    """Creates the main window and sets up the graph canvas."""
    self.setWindowTitle(f'Graph of {self.expression}')
    self.setGeometry(200, 200, 800, 600)
    fig = Figure(figsize=(5, 4), dpi=100)
    self.canvas = FigureCanvasQTAgg(fig)
    self.setCentralWidget(self.canvas)

    self.data = {
      'axis': fig.add_subplot(111),
      'x_lim': (None, None),
      'y_lim': (None, None),
      'arrows': [],
      'status_bar': QStatusBar(),
      'scale_label': QLabel(),
    }
    self.data['axis'].set_xlim(-10, 10)
    x_coord = np.linspace(-10, 10, 1000)
    y_raw = np.array([self.calculate_function(i) for i in x_coord])
    y_coord = np.array(
      [float(val) if val != 'Error' else np.nan for val in y_raw], dtype=float)
    self.line, = self.data['axis'].plot(x_coord, y_coord)

  def setup_graph_window(self):
    """Sets up the graph window with status bar, toolbar and axes."""

    self.setStatusBar(self.data['status_bar'])
    self.data['status_bar'].addWidget(self.data['scale_label'])

    toolbar = NavigationToolbar2QT(self.canvas, self)
    self.addToolBar(toolbar)

    self.data['axis'].axhline(y=0, color='black', linewidth=1.5)
    self.data['axis'].axvline(x=0, color='black', linewidth=1.5)

  def add_arrows(self):
    """Adds arrows to the end of the x and y axes."""
    self.data['arrows'] = []
    self.data['arrows'].append(
      self.data['axis'].annotate('', xy=(self.data['axis'].get_xlim()[1], 0),
                                 xytext=(self.data['axis'].get_xlim()[0], 0),
                                 arrowprops={'arrowstyle': '->'}))
    self.data['arrows'].append(
      self.data['axis'].annotate('', xy=(0, self.data['axis'].get_ylim()[1]),
                                 xytext=(0, self.data['axis'].get_ylim()[0]),
                                 arrowprops={'arrowstyle': '->'}))

  def add_grid(self):
    """Adds major and minor gridlines to the graph."""
    self.data['axis'].xaxis.set_major_locator(AutoLocator())
    self.data['axis'].yaxis.set_major_locator(AutoLocator())
    self.data['axis'].xaxis.set_minor_locator(AutoMinorLocator())
    self.data['axis'].yaxis.set_minor_locator(AutoMinorLocator())
    self.data['axis'].grid(True, which='major', linestyle='-', linewidth=0.5)
    self.data['axis'].grid(True, which='minor', linestyle='--', linewidth=0.25)

  def update_scale_label(self):
    """Updates the scale label with the current limits of the x and y axes."""
    x_min, x_max = self.data['axis'].get_xlim()
    y_min, y_max = self.data['axis'].get_ylim()
    scale_str = f'Scale: x[{x_min:.2f}, {x_max:.2f}],' \
                f' y[{y_min:.2f}, {y_max:.2f}]'
    self.data['scale_label'].setText(scale_str)
    self.data['status_bar'].setStyleSheet('color: blue')

  def update_plot(self, _=None):
    """ Updates the graph whenever there is a change in the limits
        of the x and y axes."""
    x_min, x_max = self.data['axis'].get_xlim()
    y_min, y_max = self.data['axis'].get_ylim()
    if x_min != self.data['x_lim'][0] or x_max != self.data['x_lim'][
      1] or y_min != \
        self.data['y_lim'][0] or y_max != self.data['y_lim'][1]:
      self.check_limits()
      self.data['x_lim'] = (x_min, x_max)
      self.data['y_lim'] = (y_min, y_max)
      self.line.set_xdata(np.linspace(x_min, x_max, 1000))
      x_coord = np.linspace(x_min, x_max, 1000)
      y_raw = np.array([self.calculate_function(i) for i in x_coord])
      y_coord = np.array(
        [float(val) if val != 'Error' else np.nan for val in y_raw],
        dtype=float)
      self.line.set_ydata(y_coord)
      self.data['axis'].relim()
      self.data['axis'].autoscale_view()
      self.data['axis'].figure.canvas.draw()
      self.update_scale_label()
      for arrow in self.data['arrows']:
        arrow.remove()
      self.add_arrows()

  def calculate_function(self, x_value):
    """ Calculates the value of the function for a given x value.

    Args:
        x_value (float): the x value for which to calculate the function.
    Returns:
        The value of the function for the given x value, or np.nan
        if an error occurs.
    """
    try:
      result = self.presenter.calculate_result(
        self.expression.replace('x', '(' + str(x_value) + ')'))
    except (ValueError, TypeError, IndexError):
      result = np.nan
    return result

  def check_limits(self):
    """ Checks if the limits of the x and y axes are within the maximum
        value area, and adjusts them if necessary."""
    x_min, x_max = self.data['axis'].get_xlim()
    y_min, y_max = self.data['axis'].get_ylim()
    if x_min < -MAX_VALUE_AREA:
      self.data['axis'].set_xlim(left=-MAX_VALUE_AREA)
    if x_max > MAX_VALUE_AREA:
      self.data['axis'].set_xlim(right=MAX_VALUE_AREA)
    if y_min < -MAX_VALUE_AREA:
      self.data['axis'].set_ylim(bottom=-MAX_VALUE_AREA)
    if y_max > MAX_VALUE_AREA:
      self.data['axis'].set_ylim(top=MAX_VALUE_AREA)


if __name__ == '__main__':
  app = QApplication([])
  window = GraphWindow('x')
  window.show()
  app.exec()
