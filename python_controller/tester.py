import socket
from pynput import keyboard

def send_command_to_arduino(command):
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
    # Construct the command to set the servo speeds
    print("sending")
    command = f"S0{speed0};S1{speed1+1};S2{180-(speed2-3)};S3{speed3}\n"
    send_command_to_arduino(command)

def on_press(key):
    global servo_speeds, speed_step
    try:
        if key.char == 'w':  # Increase speed for servos 1 and 2
            servo_speeds[1] += speed_step
            servo_speeds[2] += speed_step
        elif key.char == 's':  # Decrease speed for servos 1 and 2
            servo_speeds[1] -= speed_step
            servo_speeds[2] -= speed_step
        elif key.char == 'a':  # Increase speed for servo 0
            servo_speeds[0] += speed_step
        elif key.char == 'd':  # Decrease speed for servo 0
            servo_speeds[0] -= speed_step

        # Limit the speed values to valid ranges (0 to 180 degrees)
        for i in range(4):
            servo_speeds[i] = max(0, min(180, servo_speeds[i]))

        # Set the updated servo speeds
        set_servo_speeds(*servo_speeds)

        # Print the current servo speeds
        print(f"Servo 0 Speed: {servo_speeds[0]} degrees")
        print(f"Servo 1 Speed: {servo_speeds[1]} degrees")
        print(f"Servo 2 Speed: {servo_speeds[2]} degrees")
        print(f"Servo 3 Speed: {servo_speeds[3]} degrees")

    except AttributeError:
        pass

def on_release(key):
    if key == keyboard.Key.esc:
        return False

if __name__ == "__main__":
    speed_step = 10  # Amount to change speed with each key press
    servo_speeds = [90, 90, 90, 90]  # Initial servo speeds (90 degrees)

    print("Press 'W' to increase speed for servos 1 and 2")
    print("Press 'S' to decrease speed for servos 1 and 2")
    print("Press 'A' to increase speed for servo 0")
    print("Press 'D' to decrease speed for servo 0")
    print("Press 'Esc' to exit")

    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()

    # Clear the console before exiting
    print("\033c", end="")
