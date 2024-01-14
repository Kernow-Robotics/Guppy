# main.py

from time import sleep

from pygamepad.gamepads import Gamepad


def main():
    gamepad = Gamepad()
    gamepad.listen()  # And that's it

    # Now you can read gamepad values in the main thread
    try:
        while True:
            print("START button value:", gamepad.buttons.ABS_RX.value)
            sleep(0.01)
    except (KeyboardInterrupt, SystemExit):
        # Kill gamepad's listening thread
        gamepad.stop_listening()
        # And exit from the program
        exit()


if __name__ == "__main__":
    main()