import socket
import struct
import csv

data_output_path = "data/imu_data.csv"
listening_steps = 4096

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('10.0.2.11', 4444))
BUFFER_SIZE = 1024

# CSV-Datei öffnen
with open(data_output_path, 'w', newline='') as csvfile:
    header = [
        'time_micros',
        'accel_x', 'accel_y', 'accel_z',
        'gyro_x', 'gyro_y', 'gyro_z'
    ]
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(header)

    i=0

    while True:
        i += 1

        data, addr = sock.recvfrom(BUFFER_SIZE)
        time, ax,ay,az,gx,gy,gz = struct.unpack('<Qffffff', data)
        line = [time, ax,ay,az,gx,gy,gz]
        # print(time, ax, ay, az, gx, gy, gz)

        if len(line) == len(header):
                csvwriter.writerow(line)

        if i > (listening_steps-1):
            break