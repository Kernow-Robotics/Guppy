import pygame
import time
import socket

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def main():
    robot = Robot()
    pygame.init()
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
    if joystick_count == 0:
        print("No joystick detected")
    else:
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        print(f"Joystick Name: {joystick.get_name()}")
        print(f"Number of Axes: {joystick.get_numaxes()}")
        axes = [0,0,0,0,0,0]
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            axes_values = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
            # print(f"All Axis Values: {axes_values}")
            # print(axes_values[5]-axes_values[4])
            throttle = map_range(axes_values[5]-axes_values[4], -2.0, 2.0, -1.0, 1.0)
            steering = map_range(axes_values[0], -1.0, 1.0, -0.5, 0.5)
            power = throttle*255
            leftWheel = power - power*steering
            rightWheel = power + power*steering
            # print(leftWheel, rightWheel)
            robot.set_servo_speeds(int(leftWheel), int(rightWheel),0,0)
            # print()
            # print(int(map_range(axes_values[5]-axes_values[4], -2.0, 2.0, -255, 255)))
            # print(throttle, steering)
            time.sleep(0.05)
        pygame.quit()

if __name__ == "__main__":
    main()