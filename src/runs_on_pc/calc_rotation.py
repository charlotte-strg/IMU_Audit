import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# pandas dataframe aus csv einlesen
gyro_df = pd.read_csv("data/imu_data.csv")
# gyro_df = pd.read_csv("data/imu_data_with_distance.csv")

gyro_df = gyro_df.drop(columns=['accel_x', 'accel_y', 'accel_z'])

# Zeitdifferenzen in Sekunden berechnen
time_diff = gyro_df['time_micros'].diff().fillna(0) / 1e6

# Initialisiere Rotationslisten für x, y und z
rotation_x = [0]  # Anfangswinkel für X
rotation_y = [0]  # Anfangswinkel für Y
rotation_z = [0]  # Anfangswinkel für Z

# Berechnung der Rotationen in Grad
for i in range(1, len(gyro_df)):
    # Neue Rotation für die x-Achse
    new_rotation_x = rotation_x[-1] + gyro_df.loc[i, 'gyro_x'] * time_diff.iloc[i] * 180 / np.pi
    rotation_x.append(new_rotation_x)
    
    # Neue Rotation für die y-Achse
    new_rotation_y = rotation_y[-1] + gyro_df.loc[i, 'gyro_y'] * time_diff.iloc[i] * 180 / np.pi
    rotation_y.append(new_rotation_y)
    
    # Neue Rotation für die z-Achse
    new_rotation_z = rotation_z[-1] + gyro_df.loc[i, 'gyro_z'] * time_diff.iloc[i] * 180 / np.pi
    rotation_z.append(new_rotation_z)

# Füge die berechneten Rotationswerte zum DataFrame hinzu
gyro_df['rotation_x'] = rotation_x
gyro_df['rotation_y'] = rotation_y
gyro_df['rotation_z'] = rotation_z

# Berechnung der Gesamtrotation (Magnitude der Rotationsvektoren)
gyro_df['total_rotation'] = np.sqrt(gyro_df['rotation_x']**2 + gyro_df['rotation_y']**2 + gyro_df['rotation_z']**2)

# Zeige die berechneten Werte an
print(gyro_df.head)
print(gyro_df.tail)

# Gesamtrotation plotten
plt.figure()
plt.plot(gyro_df['time_micros'], gyro_df['rotation_x'], label='Rotation X')
plt.plot(gyro_df['time_micros'], gyro_df['rotation_y'], label='Rotation Y')
plt.plot(gyro_df['time_micros'], gyro_df['rotation_z'], label='Rotation Z')
plt.plot(gyro_df['time_micros'], gyro_df['total_rotation'], label='Total Rotation')
plt.xlabel('Time [µs]')
plt.ylabel('Rotation [°]')
plt.legend()
plt.show()

# Speichere die berechneten Werte in einer neuen CSV-Datei
gyro_df.to_csv("data/imu_data_with_rotation.csv", index=False)