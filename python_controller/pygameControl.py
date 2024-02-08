from kernowRobotics import Guppy
import pygame
import time

class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 25)

    def tprint(self, screen, text):
        text_bitmap = self.font.render(text, True, (0, 0, 0))
        screen.blit(text_bitmap, (self.x, self.y))
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def main():
    robot = Guppy()
    screen = pygame.display.set_mode((640, 480))
    screen.fill((255,255,255))
    clock = pygame.time.Clock()
    joysticks = {}
    # joystick_count = pygame.joystick.get_count()
    pygame.display.set_caption("Kernow Robotics Controller")
    pygame.init()
    text_print = TextPrint()
    keys=[0,0,0,0]
    running = 1
    while running:
        # print("loop")
        screen.fill((255,255,255))
        text_print.reset()
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
        joystick_count = pygame.joystick.get_count()
        text_print.tprint(screen, f"Number of joysticks: {joystick_count}")
        text_print.tprint(screen, f"{keys}")
        # robot.keysToServos(keys)
        # screen.fill(background_colour)
        # pygame.display.update()
        clock.tick(60)
        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()