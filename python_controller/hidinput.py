import pyglet

# Function to handle joystick axis motion
def on_joyaxis_motion(joystick, axis, value):
    print(f"Joystick {joystick.device.name}: Axis {axis} moved. Value: {value}")

# Function to handle joystick button press
def on_joybutton_press(joystick, button):
    print(f"Joystick {joystick.device.name}: Button {button} pressed.")

# Function to handle joystick button release
def on_joybutton_release(joystick, button):
    print(f"Joystick {joystick.device.name}: Button {button} released.")

# Check for available joysticks
controllers = pyglet.input.get_controllers()

if controllers:
    # Use the first detected joystick
    controller = controllers[0]
    controller.open()
    # controller.rumble_play_weak(strength, duration=0.5)

    # Set event handlers
    controller.push_handlers(
        on_joyaxis_motion=on_joyaxis_motion,
        on_joybutton_press=on_joybutton_press,
        on_joybutton_release=on_joybutton_release
    )

    # Start the event loop
    pyglet.app.run()
else:
    print("No joystick found.")
