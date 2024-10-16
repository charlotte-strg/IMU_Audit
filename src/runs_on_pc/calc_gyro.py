import pandas as pd
import analysis_utils as aua
import numpy as np

# pandas dataframe aus csv einlesen
gyro_df = pd.read_csv("data/imu_data_drift.csv")

# Berechnung des Bias (Offsets) für jede Achse mit Mittelwert über alle Werte
# das hier ist im stream/live nicht möglich, weil es ein mittel über alle werte verwendet
# außerdem nicht möglich, weil noise auf daten vorher rausgerechnet werden muss
# bias_x = np.mean(gyro_df['gyro_x'])
# bias_y = np.mean(gyro_df['gyro_y'])
# bias_z = np.mean(gyro_df['gyro_z'])

# Berechnung des Bias (Offsets) für jede Achse mit Annahme, dass erster Messpunkt Stillstand ist
# außerdem nicht möglich, weil noise auf daten vorher rausgerechnet werden muss
# bias_x = gyro_df['gyro_x'].iloc[0]
# bias_y = gyro_df['gyro_y'].iloc[0]
# bias_z = gyro_df['gyro_z'].iloc[0]

# Korrigiere die Gyroskopdaten um den Bias
# das hier ist im stream/live nicht möglich, weil es ein mittel über alle werte verwendet
# außerdem nicht möglich, weil noise auf daten vorher rausgerechnet werden muss
# gyro_df['gyro_x'] = gyro_df['gyro_x'] - bias_x
# gyro_df['gyro_y'] = gyro_df['gyro_y'] - bias_y
# gyro_df['gyro_z'] = gyro_df['gyro_z'] - bias_z

# für Gyro überflüssige Spalten entfernen
gyro_df = gyro_df.drop(columns=['accel_x', 'accel_y', 'accel_z'])

# Zeitdifferenzen in Sekunden berechnen
time_diff = gyro_df['time_micros'].diff().fillna(0) / 1e6

#################################### LOWPASS FILTER ####################################
cutoff = 50  # cutoff frequency in Hz
# TODO: Problem: Frequenz, die hier genutzt wird,
# ist eine andere als für FFT (accel_freq) --> anpassen!
fs = 1/time_diff.mean()  # sampling frequency in Hz
gyro_df['gyro_x'] = aua.butter_lowpass_filter(gyro_df['gyro_x'], cutoff, fs, order=5)
gyro_df['gyro_y'] = aua.butter_lowpass_filter(gyro_df['gyro_y'], cutoff, fs, order=5)
gyro_df['gyro_z'] = aua.butter_lowpass_filter(gyro_df['gyro_z'], cutoff, fs, order=5)

#################################### MADGWICK FILTER ####################################
# from ahrs.filters import Madgwick
# madgwick = Madgwick(gyr=gyro_data, acc=acc_data)     # Using IMU
# type(madgwick.Q), madgwick.Q.shape

# ODER

# If we desire to estimate each sample independently, we call the corresponding update method.
# madgwick = Madgwick()
# Q = np.tile([1., 0., 0., 0.], (num_samples, 1)) # Allocate for quaternions
# for t in range(1, num_samples):
#     Q[t] = madgwick.updateIMU(Q[t-1], gyr=gyro_data[t], acc=acc_data[t])


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
# macht das sinn? --> in quaternionen ist 3d-rotation darstellbar
# gyro_df['total_rotation'] = np.sqrt(gyro_df['rotation_x']**2 + gyro_df['rotation_y']**2 + gyro_df['rotation_z']**2)

# FFT für alle Gyro-Spalten berechnen und in neues df einfügen
gyro_x_x_freq, gyro_x_y_fourier = aua.get_fourier(gyro_df['gyro_x'], gyro_df['time_micros'])
gyro_y_x_freq, gyro_y_y_fourier = aua.get_fourier(gyro_df['gyro_y'], gyro_df['time_micros'])
gyro_z_x_freq, gyro_z_y_fourier = aua.get_fourier(gyro_df['gyro_z'], gyro_df['time_micros'])

# Da alle Frequenzen gleich für gyro_x, gyro_y und gyro_z nur eine behalten
gyro_freq = gyro_x_x_freq

# Absolutwerte berechnen, um aus den komplexen Werten reelle zu machen
# und direkt umbenennung da es jetzt nur noch accel_freq statt gyro_x_x_freq gibt, können "gyro_x_y_fourier" namen vereinfacht werden
gyro_fourier_x = np.abs(gyro_x_y_fourier)
gyro_fourier_y = np.abs(gyro_y_y_fourier)
gyro_fourier_z = np.abs(gyro_z_y_fourier)

gyro_freq_fourier_df = pd.DataFrame({
    'gyro_freq':        gyro_freq,
    'gyro_fourier_x':   gyro_fourier_x,
    'gyro_fourier_y':   gyro_fourier_y,
    'gyro_fourier_z':   gyro_fourier_z,
})

# Zeige die berechneten Werte an
# print(gyro_df.head)
# print(gyro_df.tail)

# Speichere die berechneten Werte in einer neuen CSV-Datei
gyro_df.to_csv("data/imu_data_drift_with_rotation.csv", index=False)
gyro_freq_fourier_df.to_csv("data/imu_data_drift_with_rotation_fourier.csv")