import typing
from typing import Any, Optional
from PyQt5.QtCore import Qt, QRect, QTimer, pyqtSlot
from PyQt5.QtGui import QKeyEvent, QMouseEvent, QPalette
from PyQt5.QtWidgets import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtGui import QPixmap, QImage
import sys
import os
import time
import subprocess
import numpy as np
from collections import defaultdict

from rendererModule import Renderer

def tracksTimePassage(func):
    def wrapped(*args, **kwargs):
        retval = func(*args, **kwargs)
        wrapped._lastCallTime = time.time()
        return retval
    wrapped._lastCallTime = None
    return wrapped

def returnsRuntime(func):
    """decorater that returns the time it took to run the function in seconds"""
    def wrapper(*args, **kwargs) -> float:
        begin = time.time()
        func(*args, **kwargs)
        return time.time() - begin
    return wrapper


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
        self.render_button.clicked.connect(lambda: self._update())
        
        self.right_layout.addWidget(self.render_button)

        self.render_time = QLabel("")
        self.right_layout.addWidget(self.render_time)

        #vars for keyboard processing
        self._keysBeingPressed = defaultdict(bool)
        self._cameraMovementSpeed: float = 8.0

        #create Renderer and render first image
        self.viewPortWidth = 1500
        self.viewPortHeight = 800
        self.renderer = Renderer( self.viewPortWidth, self.viewPortHeight)
        self.img_array = None
        self._render()

        #frame calls 
        self.timer = QTimer(self)
        self.timer.timeout.connect(lambda: self._update())
        self.timer.start(16)

    @tracksTimePassage
    def _update(self) -> None:
        ts = 0.0 if self._update._lastCallTime is None else time.time() - self._update._lastCallTime
        self._lastCalltime = time.time()
        time_to_render = self._render()
        self.renderer.moveCamera(self._determineCameraMovement()*ts*self._cameraMovementSpeed)
        self.render_time.setText(str("{:.2f}".format(time_to_render*1000)) + "ms")
    

    def _set_image_to_screen(self):
        qimage = QImage(self.img_array.data, self.viewPortWidth, self.viewPortHeight, QImage.Format_ARGB32)
        pixmap = QPixmap.fromImage(qimage)
        self.imgContainer.setPixmap(pixmap)
        self.imgContainer.resize(pixmap.width(), pixmap.height())
        #self.resize(max(self.width(), pixmap.width()), max(self.height(), pixmap.height()))

    @returnsRuntime
    def _render(self) -> None:
        self.img_array = self.renderer.render()
        self._set_image_to_screen()

    #controlls the movement of the camera by setting the _cameraMoveDirection depending on wsad. Movement is only working if the right mouse key is pressed
    def keyPressEvent(self, a0: QKeyEvent) -> None:
        self._keysBeingPressed[a0.key()] = True
    def keyReleaseEvent(self, a0: QKeyEvent) -> None:
        self._keysBeingPressed[a0.key()] = False
    def mousePressEvent(self, a0: QMouseEvent) -> None:
        self._keysBeingPressed[a0.button()] = True
    def mouseReleaseEvent(self, a0: QMouseEvent) -> None:
        self._keysBeingPressed[a0.button()] = False

    def _determineCameraMovement(self):
        retval = np.array([0,0,0], float)
        if self._keysBeingPressed[Qt.RightButton] is False:
            return retval
        if self._keysBeingPressed[Qt.Key_W]:
            retval[1] -= 1
        if self._keysBeingPressed[Qt.Key_S]:
            retval[1] +=1
        if self._keysBeingPressed[Qt.Key_D]:
            retval[0] += 1
        if self._keysBeingPressed[Qt.Key_A]:
            retval[0] -= 1
        return retval

         

if __name__ == "__main__":
    app = QApplication([])
    window = Window()
    window.show()
    sys.exit(app.exec())

