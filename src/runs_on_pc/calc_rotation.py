import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# pandas dataframe aus csv einlesen
gyro_df = pd.read_csv("data/imu_data_drift.csv")
# gyro_df = pd.read_csv("data/imu_data_with_distance.csv")

# Berechnung des Bias (Offsets) für jede Achse
# das hier ist im stream/live nicht möglich, weil es ein mittel über alle werte verwendet
bias_x = np.mean(gyro_df['gyro_x'])
bias_y = np.mean(gyro_df['gyro_y'])
bias_z = np.mean(gyro_df['gyro_z'])

# Korrigiere die Gyroskopdaten um den Bias
# das hier ist im stream/live nicht möglich, weil es ein mittel über alle werte verwendet
gyro_df['gyro_x'] = gyro_df['gyro_x'] - bias_x
gyro_df['gyro_y'] = gyro_df['gyro_y'] - bias_y
gyro_df['gyro_z'] = gyro_df['gyro_z'] - bias_z

# für Gyro überflüssige Spalten entfernen
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
# macht das sinn? 
gyro_df['total_rotation'] = np.sqrt(gyro_df['rotation_x']**2 + gyro_df['rotation_y']**2 + gyro_df['rotation_z']**2)

# Zeige die berechneten Werte an
# print(gyro_df.head)
# print(gyro_df.tail)

# Speichere die berechneten Werte in einer neuen CSV-Datei
gyro_df.to_csv("data/imu_data_with_rotation.csv", index=False)