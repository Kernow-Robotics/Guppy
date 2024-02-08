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
    motors=[0,0,0,0,0,0]
    running = 1
    while running:
        screen.fill((255,255,255))
        text_print.reset()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = 0
            if event.type == pygame.JOYDEVICEADDED:
                # This event will be generated when the program starts for every
                # joystick, filling up the list without needing to create them manually.
                joy = pygame.joystick.Joystick(event.device_index)
                joysticks[joy.get_instance_id()] = joy
                print(f"Joystick {joy.get_instance_id()} connencted")

            if event.type == pygame.JOYDEVICEREMOVED:
                del joysticks[event.instance_id]
                print(f"Joystick {event.instance_id} disconnected")
        # ---- GET KEY STATES ---- #
        keyspressed = pygame.key.get_pressed()
        keys[0] = keyspressed[pygame.K_w]*1
        keys[1] = keyspressed[pygame.K_a]*1
        keys[2] = keyspressed[pygame.K_s]*1
        keys[3] = keyspressed[pygame.K_d]*1

        # ---- GET JOYSTICK AXES ---- #
        axes=[0,0,0,0,0,0,]
        joystick_count = pygame.joystick.get_count()
        if joystick_count > 0:
            print("howdy")
            for joystick in joysticks.values():
                axes = []
                numaxes = joystick.get_numaxes()
                print(numaxes)
                for i in range(numaxes):
                    axes.append(joystick.get_axis(i))
                    print(joystick.get_axis(i))

        # ---- SELECT CONTROL SUPPLY ---- #
        selector = 0
        if any(keys):
            selector=1
        elif any(axes):
            selector=2
        else:
            selector=0
        
        # ---- CALCULATE MOTOR COMMANDS ---- #
        if selector==0:
            robot.set_servo_speeds(0,0,0,0,0,0)
        elif selector==1:
            robot.set_servo_speeds(0,0,0,0,0,0)
        elif selector==2:
            robot.set_servo_speeds(0,0,0,0,0,0)


        text_print.tprint(screen, f"Number of joysticks: {joystick_count}")
        text_print.tprint(screen, f"{keys}")
        text_print.tprint(screen, f"{axes}")
        text_print.tprint(screen, f"{selector}")
        # robot.keysToServos(keys)
        # screen.fill(background_colour)
        # pygame.display.update()
        clock.tick(60)
        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()