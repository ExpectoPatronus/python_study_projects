"""
This module contains the main entry point for the SmartCalc application.
"""

from PyQt6 import QtWidgets
# pylint: disable=import-error
from view.main_window import MainWindow

if __name__ == '__main__':
  app = QtWidgets.QApplication([])
  window = MainWindow()
  window.show()
  app.exec()
