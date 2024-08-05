import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# pandas dataframe aus csv einlesen
# accel_df = pd.read_csv("data/imu_data.csv")
# accel_df = pd.read_csv("data/imu_data_drift.csv")
accel_df = pd.read_csv("data/imu_data_drift.csv")
accel_df = accel_df.drop(columns=['gyro_x', 'gyro_y', 'gyro_z'])

# Ziehe Erdanziehungskraft von den Beschleunigungswerten accel_z ab
accel_df['accel_z'] = accel_df['accel_z'] - 9.81

# Zeitdifferenzen in Sekunden berechnen
time_diff = accel_df['time_micros'].diff().fillna(0) / 1e6
# print(time_diff.head())

# Initialisiere Geschwindigkeits- und Distanzlisten für x, y und z
velocity_x = [0]
distance_x = [0]
velocity_y = [0]
distance_y = [0]
velocity_z = [0]
distance_z = [0]

# Berechnung der Geschwindigkeit und Strecke
for i in range(1, len(accel_df)):
    # Neue Geschwindigkeit und Strecke für die x-Achse
    new_velocity_x = velocity_x[-1] + accel_df.loc[i, 'accel_x'] * time_diff.iloc[i]
    velocity_x.append(new_velocity_x)
    new_distance_x = distance_x[-1] + new_velocity_x * time_diff.iloc[i]
    distance_x.append(new_distance_x)
    
    # Neue Geschwindigkeit und Strecke für die y-Achse
    new_velocity_y = velocity_y[-1] + accel_df.loc[i, 'accel_y'] * time_diff.iloc[i]
    velocity_y.append(new_velocity_y)
    new_distance_y = distance_y[-1] + new_velocity_y * time_diff.iloc[i]
    distance_y.append(new_distance_y)
    
    # Neue Geschwindigkeit und Strecke für die z-Achse
    new_velocity_z = velocity_z[-1] + accel_df.loc[i, 'accel_z'] * time_diff.iloc[i]
    velocity_z.append(new_velocity_z)
    new_distance_z = distance_z[-1] + new_velocity_z * time_diff.iloc[i]
    distance_z.append(new_distance_z)

# Füge die berechneten Geschwindigkeits- und Distanzwerte zum DataFrame hinzu
accel_df['velocity_x'] = velocity_x
accel_df['velocity_y'] = velocity_y
accel_df['velocity_z'] = velocity_z
accel_df['distance_x'] = distance_x
accel_df['distance_y'] = distance_y
accel_df['distance_z'] = distance_z

# gesamte Distanz (x-Achse, y-Achse, z-Achse) berechnen
accel_df['distance_total'] = (accel_df['distance_x'] ** 2 + accel_df['distance_y'] ** 2 + accel_df['distance_z'] ** 2) ** 0.5

# Zeige die berechneten Werte an
# print(accel_df.head())
# print(accel_df.tail())

# plotten
plt.figure()

# Distanz
plt.plot(accel_df['time_micros'] / 1e6, accel_df['distance_x'], label='Distance X')
plt.plot(accel_df['time_micros'] / 1e6, accel_df['distance_y'], label='Distance Y')
plt.plot(accel_df['time_micros'] / 1e6, accel_df['distance_z'], label='Distance Z')
plt.plot(accel_df['time_micros'] / 1e6, accel_df['distance_total'], label='Distance')
plt.ylabel('Distance [m]')

# Geschwindigkeit
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['velocity_x'], label='Velocity X')
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['velocity_y'], label='Velocity Y')
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['velocity_z'], label='Velocity Z')
# plt.ylabel('Velocity [m/s]')

# Beschleunigung
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['accel_x'], label='Accel X')
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['accel_y'], label='Accel Y')
# plt.plot(accel_df['time_micros'] / 1e6, accel_df['accel_z'], label='Accel Z')
# plt.ylabel('Accel [m/s^2]')

plt.xlabel('Time [s]')

plt.legend()
plt.show()

# test
print(accel_df['velocity_z'])
print(np.diff(accel_df['velocity_z']))
print(np.std(np.diff(accel_df['velocity_z'])))
print(np.mean(np.diff(accel_df['velocity_z'])))


# Speichere den DataFrame in einer neuen CSV-Datei
# accel_df.to_csv("data/imu_data_with_distance.csv", index=False)
accel_df.to_csv("data/imu_data_with_distance_backup.csv")