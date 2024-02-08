import pyglet
# devices = pyglet.input.get_devices()
# controllers = pyglet.input.get_controllers()
# joysticks = pyglet.input.get_joysticks()
# print(joysticks)

# manager = pyglet.input.ControllerManager()

if controllers:
    controller = controllers[0]
    controller.open()
    print()
    print("openned controller")
    while True:
        if controller.a == True:
            print("howdy")