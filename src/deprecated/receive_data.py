import socket
import struct


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('10.0.2.11', 4444))
BUFFER_SIZE = 1024

while True:
    data, addr = sock.recvfrom(BUFFER_SIZE)
    time, ax,ay,az,gx,gy,gz= struct.unpack('<Qffffff', data)
    print(time, ax, ay, az, gx, gy, gz)