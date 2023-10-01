import pygame
import time
import socket
# from pygame.locals import *
# import sys

class Robot(object):
    """
    a class to do a job
    """

    def __init__(self):
        """
        Runs once when object is initialised
        """
        self.arduino_ip = "192.168.0.15"  # Replace this with the IP address of your Arduino
        self.arduino_port = 333  # Use any available port number (greater than 1023)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # self.sock.connect((self.arduino_ip, self.arduino_port))
        self.speeds = [90,90,90,90]
        self.messageNumber = 0
        self.lasttime = time.time()

    def keysToServos(self, keys):
        self.speeds = [0,0,0,0]
        if keys[0]==1:
            self.speeds[0]+=255
            self.speeds[1]+=255
        if keys[2]==1:
            self.speeds[0]-=255
            self.speeds[1]-=255
        if keys[1]==1:
            self.speeds[0]+=255
            self.speeds[1]-=255
        if keys[3]==1:
            self.speeds[0]-=255
            self.speeds[1]+=255
        # for speed in self.speeds:
        #     if speed > 180:
        #         speed = 180
        #     if speed < 0:
        #         speed = 0
        print(self.speeds)
        self.set_servo_speeds(self.speeds[0], self.speeds[1], self.speeds[2], self.speeds[3])

    def set_servo_speeds(self, speed0, speed1, speed2, speed3):
        command = f"S0{speed0};S1{speed1*1.0};S2{speed2};S3{speed3}\n"
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
    background_colour = (255,255,255)
    screen = pygame.display.set_mode((640, 480))
    screen.fill(background_colour)
    clock = pygame.time.Clock()
    pygame.display.set_caption("title")
    pygame.init()
    pygame.display.flip()
    # time.sleep(1)
    running = 1
    keys=[0,0,0,0]
    while running:
        # print("loop")
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = 0
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_w:
                    keys[0] = 1
                if event.key == pygame.K_a:
                    keys[1] = 1
                if event.key == pygame.K_s:
                    keys[2] = 1
                if event.key == pygame.K_d:
                    keys[3] = 1
            if event.type == pygame.KEYUP:
                if event.key == pygame.K_w:
                    keys[0] = 0
                if event.key == pygame.K_a:
                    keys[1] = 0
                if event.key == pygame.K_s:
                    keys[2] = 0
                if event.key == pygame.K_d:
                    keys[3] = 0
        robot.keysToServos(keys)
        screen.fill(background_colour)
        pygame.display.update()
        pygame.display.flip()
        time.sleep(0.02)

    pygame.quit()

if __name__ == "__main__":
    main()