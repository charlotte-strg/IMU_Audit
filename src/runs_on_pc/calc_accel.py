import pandas as pd
import analysis_utils as aua
import numpy as np

# pandas dataframe aus csv einlesen
# accel_df = pd.read_csv("data/imu_data.csv")
accel_df = pd.read_csv("data/imu_data_drift.csv")

# für Accel überflüssige Spalten entfernen
accel_df = accel_df.drop(columns=['gyro_x', 'gyro_y', 'gyro_z'])

# Ziehe Erdanziehungskraft von den Beschleunigungswerten accel_z ab
# accel_df['accel_z'] = accel_df['accel_z'] - 9.81
accel_df['accel_z'] = accel_df['accel_z']

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

# FFT für alle accel Spalten berechnen und in neues df einfügen
accel_x_x_freq, accel_x_y_fourier = aua.get_fourier(accel_df['accel_x'], accel_df['time_micros'])
accel_y_x_freq, accel_y_y_fourier = aua.get_fourier(accel_df['accel_y'], accel_df['time_micros'])
accel_z_x_freq, accel_z_y_fourier = aua.get_fourier(accel_df['accel_z'], accel_df['time_micros'])

# da alle frequenzen gleich für accel_x, accel_y und accel_z nur eine behalten. vergleichbar zu der einen zeitspalte in accel_df
accel_freq = accel_x_x_freq

# abs berechnen um aus imaginären werten reele zu machen, mathe shit halt
# und direkt umbenennung da es jetzt nur noch accel_freq statt accel_x_x_freq gibt, können "accel_x_y_fourier" namen vereinfacht werden
accel_fourier_x = np.abs(accel_x_y_fourier)
accel_fourier_y = np.abs(accel_y_y_fourier)
accel_fourier_z = np.abs(accel_z_y_fourier)

accel_freq_fourier_df = pd.DataFrame({
    'accel_freq':       accel_freq,
    'accel_fourier_x':  accel_fourier_x,
    'accel_fourier_y':  accel_fourier_y,
    'accel_fourier_z':  accel_fourier_z,
})
print(f"{accel_freq_fourier_df=}")


# Zeige die berechneten Werte an
# print(accel_df.head())
# print(accel_df.tail())

# Speichere den DataFrame in einer neuen CSV-Datei
# accel_df.to_csv("data/imu_data_with_distance.csv", index=False)
accel_df.to_csv("data/backup_imu_data_with_distance.csv")
accel_freq_fourier_df.to_csv("data/backup_imu_data_with_distance_fourier.csv")