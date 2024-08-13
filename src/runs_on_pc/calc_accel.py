import pandas as pd
import analysis_utils as aua

# pandas dataframe aus csv einlesen
# accel_df = pd.read_csv("data/imu_data.csv")
accel_df = pd.read_csv("data/imu_data_drift.csv")

# für Accel überflüssige Spalten entfernen
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

# FFT für alle accel Spalten berechnen und in accel_df einfügen
fourier_cols = [
    accel_df['accel_x'],
    accel_df['accel_y'],
    accel_df['accel_z']
]
# hier vielleicht noch Fehlerausgabe, 
# wenn len(accel_df['time_micros']) != len(fourier_cols) ? 
fourier = [aua.get_fourier(col, accel_df['time_micros']) for col in fourier_cols]
# warum nochmal [:1] ???
print(len(fourier))
print(len(accel_df))
for x_freq, y_fourier in fourier[:1]:
    accel_df['x_freq'] = x_freq
    accel_df['y_fourier'] = y_fourier
print(fourier)


# Zeige die berechneten Werte an
# print(accel_df.head())
# print(accel_df.tail())

# Speichere den DataFrame in einer neuen CSV-Datei
# accel_df.to_csv("data/imu_data_with_distance.csv", index=False)
accel_df.to_csv("data/backup_imu_data_with_distance.csv")