import socket
import time

class Guppy(object):
    """
    a class for creating instances of robots for comms using radio
    """

    def __init__(self):
        """
        Runs once when object is initialised
        """
        self.arduino_ip = "192.168.0.115"  # Replace this with the IP address of your Arduino
        # self.arduino_ip = "172.20.10.4"  # Replace this with the IP address of your Arduino
        self.arduino_port = 333  # Use any available port number (greater than 1023)
        # self.arduino_port = 333  # Use any available port number (greater than 1023)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2048)
        # self.sock.connect((self.arduino_ip, self.arduino_port))
        self.speeds = [0,0,0,0,0,0]
        self.messageNumber = 0
        self.lasttime = time.time()

    def keysToServos(self, keys):
        powerstep = 64
        self.speeds = [0,0,0,0,0,0]
        if keys[0]==1:
            self.speeds[0]+=powerstep
            self.speeds[1]+=powerstep
        if keys[2]==1:
            self.speeds[0]-=powerstep
            self.speeds[1]-=powerstep
        if keys[1]==1:
            self.speeds[0]+=powerstep
            self.speeds[1]-=powerstep
        if keys[3]==1:
            self.speeds[0]-=powerstep
            self.speeds[1]+=powerstep
        # for speed in self.speeds:
        #     if speed > 180:
        #         speed = 180
        #     if speed < 0:
        #         speed = 0
        print(self.speeds)
        self.set_servo_speeds(self.speeds[0], self.speeds[1], self.speeds[2], self.speeds[3], self.speeds[4], self.speeds[5])

    def set_servo_speeds(self, speed0, speed1, speed2, speed3, speed4, speed5):
        command = f"S0{speed0};S1{speed1};S2{speed2};S3{speed3};S4{speed4};S5{speed5}\n"
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
            print(command)
            self.sock.sendto(command.encode(), (self.arduino_ip, self.arduino_port))
            # self.sock.close()

        except Exception as e:
            print(f"Error sending command to Arduino: {e}")

