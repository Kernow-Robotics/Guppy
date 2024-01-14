from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QVBoxLayout, QPushButton
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGamepad import QGamepad, QGamepadManager

class XboxControllerReader(QWidget):
    def __init__(self, parent=None):
        super(XboxControllerReader, self).__init__(parent)

        self.gamepad_manager = QGamepadManager()
        self.controller = None

        self.setup_ui()
        self.setup_gamepad()

    def setup_ui(self):
        self.setWindowTitle("Xbox Controller Reader")
        self.setGeometry(100, 100, 300, 200)

        self.label = QLabel("Xbox Controller Input", self)
        layout = QVBoxLayout(self)
        layout.addWidget(self.label)

        self.quit_button = QPushButton("Quit", self)
        self.quit_button.clicked.connect(self.close)
        layout.addWidget(self.quit_button)

    def setup_gamepad(self):
        available_gamepads = self.gamepad_manager.connectedGamepads()
        if available_gamepads:
            self.controller = QGamepad(available_gamepads[0])
            self.timer = QTimer(self)
            self.timer.timeout.connect(self.read_controller_input)
            self.timer.start(16)  # Update every 16 milliseconds (about 60 FPS)
        else:
            self.label.setText("No Xbox Controller found.")

    def read_controller_input(self):
        if self.controller:
            # Read input from the connected gamepad
            button_states = [self.controller.buttonValue(i) for i in range(QGamepadButton.CountButtons)]
            axis_states = [self.controller.axisValue(i) for i in range(QGamepadAxis.CountAxes)]

            input_data = f"Buttons: {button_states}, Axes: {axis_states}"
            self.label.setText(f"Controller Input: {input_data}")

if __name__ == "__main__":
    app = QApplication([])
    window = XboxControllerReader()
    window.show()
    app.exec_()