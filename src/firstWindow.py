import typing
from typing import Any
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt, QRect, QTimer
from PyQt5.QtGui import QPalette
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QPixmap, QImage
import sys
import os
import time
import subprocess

from rendererModule import Renderer




class Timer(object):
    def __init__(self) -> None:
        self.last_time_called = time.time()
    def __call__(self) -> Any:
        elapsed_time = self.last_time_called - time.time()
        self.last_time_called = time.time()
        return elapsed_time

class Window(QMainWindow):
    def __init__(self,) -> None:
        super().__init__()

        self.setWindowTitle("RayTracer")

        #create main context
        self.central_widget = QWidget()
        self.central_widget.setStyleSheet("background-color: lightblue;")
        self.setCentralWidget(self.central_widget)
      
        #let the main_layout is horizontally
        self.main_layout = QHBoxLayout()
        self.central_widget.setLayout(self.main_layout)

        #add the image container
        self.imgContainer = QLabel(self)
        self.main_layout.addWidget(self.imgContainer)
        
        #add the right colmun (VLayout) for the handling and buttons
        self.right_layout = QVBoxLayout()
        right_widget = QWidget()  # Widget to hold the right column layout
        right_widget.setStyleSheet("background-color: lightgrey;")
        right_widget.setLayout(self.right_layout) 
        self.main_layout.addWidget(right_widget)

        #add buttons to it:
        self.render_button = QPushButton("Render")
        self.render_button.setFixedSize(100, 50)
        self.render_button.setStyleSheet("background-color: lightblue; border: 1px solid black; border-radius: 5px;")
        self.render_button.clicked.connect(self._render)
        self.right_layout.addWidget(self.render_button)

        self.render_time = QLabel("")
        self.right_layout.addWidget(self.render_time)



        #create Renderer and render first image
        self.renderer = Renderer( 1080, 800)
        self.img_array = self.renderer.Render()
        self._render()

        #frame calls 
        self.timer = QTimer(self)
        self.timer.timeout.connect(self._update)
        self.timer.start(16)


    def _update(self) -> None:
        time_to_render = self._render()
        self.render_time.setText(str("{:.2f}".format(time_to_render)) + "ms")

    def _set_image_to_screen(self):
        qimage = QImage(self.img_array.data, 1080, 800, QImage.Format_ARGB32)
        pixmap = QPixmap.fromImage(qimage)
        self.imgContainer.setPixmap(pixmap)
        self.imgContainer.resize(pixmap.width(), pixmap.height())
        #self.resize(max(self.width(), pixmap.width()), max(self.height(), pixmap.height()))

    def _render(self) -> float:
        begin = time.time()
        self.img_array = self.renderer.Render()
        self._set_image_to_screen()
        return (time.time() - begin)*1000


if __name__ == "__main__":
    app = QApplication([])
    window = Window()
    window.show()
    sys.exit(app.exec())

