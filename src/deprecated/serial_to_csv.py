import serial
import csv

# Serielle Verbindung einrichten (COM-Port anpassen)
ser = serial.Serial('COM6', 115200)

data_output_path = "data/imu_data.csv"
listening_steps = 2048

# CSV-Datei öffnen
with open(data_output_path, 'w', newline='') as csvfile:
    header = [
        'id', 'time_micros',
        'accel_x', 'accel_y', 'accel_z',
        'gyro_x', 'gyro_y', 'gyro_z'
    ]
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(header)

    # Endlosschleife zum Lesen von Daten von der seriellen Schnittstelle
    for i in range(listening_steps):
        # Lies eine Zeile von der seriellen Schnittstelle
        line = ser.readline().decode('utf-8')#.strip()
        print(i, line) # Optional: Ausgabe der empfangenen Daten

        # Teile die Zeile anhand des Kommas auf und schreibe sie in die CSV-Datei
        data = line.replace('[', '').replace(']', '').strip().split(',')

        # only write data if line has the correct number of items
        if len(data) == len(header):
            csvwriter.writerow(data)