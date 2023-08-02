import pygame
import time
import socket
# from pygame.locals import *
# import sys

def send_command_to_arduino(command):
    print(command)
    try:
        arduino_ip = "192.168.0.13"  # Replace this with the IP address of your Arduino
        arduino_port = 333  # Use any available port number (greater than 1023)

        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((arduino_ip, arduino_port))
        sock.sendall(command.encode())
        sock.close()

    except Exception as e:
        print(f"Error sending command to Arduino: {e}")

def set_servo_speeds(speed0, speed1, speed2, speed3):
    command = f"S0{speed0};S1{speed1+1};S2{180-(speed2-3)};S3{speed3}\n"
    send_command_to_arduino(command)

def keysToServos(keys):
    speeds = [90,90,90,90]
    if keys[0]==1:
        speeds[1]+=90
        speeds[2]+=90
    if keys[2]==1:
        speeds[1]-=90
        speeds[2]-=90
    if keys[1]==1:
        speeds[1]-=70
        speeds[2]+=70
    if keys[3]==1:
        speeds[1]+=70
        speeds[2]-=70
    for speed in speeds:
        if speed > 180:
            speed = 180
        if speed < 0:
            speed = 0
    print(speeds)
    set_servo_speeds(speeds[0], speeds[1], speeds[2], speeds[3])


def main():
    background_colour = (255,255,255)
    screen = pygame.display.set_mode((640, 480))
    screen.fill(background_colour)
    clock = pygame.time.Clock()
    pygame.display.set_caption("title")

    pygame.init()
    pygame.display.flip()
    time.sleep(1)
    running = 1
    keys=[0,0,0,0]
    while running:
        # print("loop")
        for event in pygame.event.get():
            # if (event.type == KEYUP) or (event.type == KEYDOWN):
            #     print("key pressed")
            #     time.sleep(0.1)
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
        keysToServos(keys)
        screen.fill(background_colour)
        clock.tick(60)
        pygame.display.update()
        pygame.display.flip()
        # pygame.time.wait(1)
    pygame.quit()

if __name__ == "__main__":
    main()