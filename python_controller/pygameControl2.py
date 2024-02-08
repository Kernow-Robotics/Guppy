import pygame
import time
import socket

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

class Robot(object):
    """
    a class to do a job
    """

    def __init__(self):
        """
        Runs once when object is initialised
        """
        self.arduino_ip = "192.168.0.105"  # Replace this with the IP address of your Arduino
        self.arduino_port = 333  # Use any available port number (greater than 1023)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # self.sock.connect((self.arduino_ip, self.arduino_port))
        self.speeds = [90,90,90,90]
        self.messageNumber = 0
        self.lasttime = time.time()

    def keysToServos(self, keys):
        powerstep = 127
        self.speeds = [0,0,0,0]
        if keys[2]==1:
            self.speeds[0]+=powerstep
            self.speeds[1]+=powerstep
        if keys[0]==1:
            self.speeds[0]-=powerstep
            self.speeds[1]-=powerstep
        if keys[3]==1:
            self.speeds[0]+=powerstep
            self.speeds[1]-=powerstep
        if keys[1]==1:
            self.speeds[0]-=powerstep
            self.speeds[1]+=powerstep
        # for speed in self.speeds:
        #     if speed > 180:
        #         speed = 180
        #     if speed < 0:
        #         speed = 0
        print(self.speeds)
        self.set_servo_speeds(self.speeds[0], self.speeds[1], self.speeds[2], self.speeds[3])

    def set_servo_speeds(self, speed0, speed1, speed2, speed3):
        command = f"S0{speed0};S1{speed1};S2{speed2};S3{speed3}\n"
        # command = f"S0{speed0};S1{speed1+1};S2{180-(speed2-3)};S3{speed3};{self.messageNumber}\n"
        self.messageNumber+=1
        # self.sock.close()
        print(self.messageNumber)
        print(1/(time.time()-self.lasttime))
        self.lasttime = time.time()
        self.send_command_to_arduino(command)

    def send_command_to_arduino(self, command):
        print(command)
        try:
            # self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # self.sock.connect((self.arduino_ip, self.arduino_port))
            self.sock.sendto(command.encode(), (self.arduino_ip, self.arduino_port))
            # self.sock.close()

        except Exception as e:
            print(f"Error sending command to Arduino: {e}")

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