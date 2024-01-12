import typing
from typing import Any
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt, QRect
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
    def __init__(self, coupler) -> None:
        super().__init__()
        self.renderer = Renderer("resources/image.png", 1080, 800)
        self.img_array = self.renderer.Render()

        self.coupler = coupler
        self.img_path = self.coupler.img_path
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
        self.load_image()

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
        self.render_button.clicked.connect(self.render)
        self.right_layout.addWidget(self.render_button)

        self.render_time = QLabel("")
        self.right_layout.addWidget(self.render_time)

        self.render()


    def load_image(self):
        qimage = QImage(self.img_array.data, 1080, 800, QImage.Format_ARGB32)
        pixmap = QPixmap.fromImage(qimage)
        self.imgContainer.setPixmap(pixmap)
        self.imgContainer.resize(pixmap.width(), pixmap.height())
        #self.resize(max(self.width(), pixmap.width()), max(self.height(), pixmap.height()))

    def render(self):
        begin = time.time()
        self.img_array = self.renderer.Render()
        self.load_image()
        elapsed_in_ms = (time.time() - begin)*1000
        print(elapsed_in_ms, "ms elapsed for rendering to screen")
    
class Coupler:
    def __init__(self, render_executable_path, img_path) -> None:
       self.exc = render_executable_path
       self.img_path = img_path 
    
    def render(self):
        subprocess.call([self.exc, self.img_path])
        return 0.0





if __name__ == "__main__":
    def main():
        app = QApplication([])
        coupler = Coupler("./build/render", "resources/image.png")
        window = Window(coupler)
        window.show()
        sys.exit(app.exec())
    main()
