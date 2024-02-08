from kernowRobotics import Guppy
import pygame
import time


def main():
    robot = Guppy()
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
        # screen.fill(background_colour)
        # pygame.display.update()
        # pygame.display.flip()
        time.sleep(0.01)

    pygame.quit()

if __name__ == "__main__":
    main()