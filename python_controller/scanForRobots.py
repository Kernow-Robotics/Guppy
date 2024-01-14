import socket

udp_host = "0.0.0.0"
udp_port = 12345

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((udp_host, udp_port))

print(f"Listening for announcements on {udp_host}:{udp_port}")

while True:
    data, addr = sock.recvfrom(1024)
    print(f"Received announcement from {addr}: {data.decode()}")
    
    # # Extracting the IP address from the announcement
    # ip_address = data.decode().split()[-1]
    # print(f"Arduino IP address: {ip_address}")