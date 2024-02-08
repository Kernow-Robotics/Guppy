from joystick_controller import ControlAssignment
import time

def main():
    
    # Create a configurations dictionary, where a set of buttons, d-pad and analog joysticks
    # are mapped to a given variable.
    button_assingment = {"surge": {"inputs":[
                                        {"type": "axis", "id": 1, "gain": -0.5, "offset": 0.0},
                                        {"type": "hat", "id": 0, "index": 1, "gain": 0.5, "offset": 0.0}],
                                    "integrate": False},

                        "depth": {"inputs":[
                                        {"type": "axis", "id": 2, "gain": -0.5, "offset": 1.0},
                                        {"type": "button", "id": 4, "gain": 0.5, "offset": 0.0}],
                                    "integrate": True,
                                    "lower_limit": -3,
                                    "upper_limit": 0}}


    # Create a joystick controller
    joystick_controller = ControlAssignment(button_assingment)

    # Method used to override the state
    override = False

    # Enter the infinite loop
    while(True):
        
        # Exemplify the use of the override method (set_desired_state)
        if override:
            joystick_controller.set_desired_state("depth", -2.0)

        # Get a dictionary with the output of the controller (keys will be "surge" and "depth")
        output = joystick_controller.check_events()

        # Print the output dictionary to the screen
        print(output)

        # If output depth reaches -1 override to -2.0
        if output["depth"] < -1.0:
            override=True

        # Sleep for a few milliseconds
        time.sleep(1.0 / 10.0)


if __name__ == '__main__':
    main()