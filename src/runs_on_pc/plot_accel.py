import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Originalsignal plotten
# imu_df = pd.read_csv("data/imu_data_drift.csv")

# Distanz/Geschwindigkeit plotten
# imu_df = pd.read_csv("data/imu_data_drift_with_distance.csv")
imu_df = pd.read_csv("data/backup_imu_data_with_distance.csv")

# Subplots erstellen
fig, axs = plt.subplots(3, 1, figsize=(10, 15), sharex=True)

# Beschleunigung/Accel
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['accel_x'], label='Accel X')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['accel_y'], label='Accel Y')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['accel_z'], label='Accel Z')
axs[0].set_ylabel('Accel [m/s^2]')
axs[0].legend(loc='upper right')
axs[0].grid(True)

# Geschwindigkeit
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['velocity_x'], label='Velocity X')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['velocity_y'], label='Velocity Y')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['velocity_z'], label='Velocity Z')
axs[1].set_ylabel('Velocity [m/s]')
axs[1].legend(loc='upper right')
axs[1].grid(True)

# Distanz
axs[2].plot(imu_df['time_micros'] / 1e6, imu_df['distance_x'], label='Distance X')
axs[2].plot(imu_df['time_micros'] / 1e6, imu_df['distance_y'], label='Distance Y')
axs[2].plot(imu_df['time_micros'] / 1e6, imu_df['distance_z'], label='Distance Z')
axs[2].plot(imu_df['time_micros'] / 1e6, imu_df['distance_total'], label='Total Distance')
axs[2].set_ylabel('Distance [m]')
axs[2].set_xlabel('Time [s]')
axs[2].legend(loc='upper right')
axs[2].grid(True)

# FFT
# axs[3].plot(imu_df['freqs'], np.abs(accel_x_fft), label='FFT Accel X')
# axs[3].plot(imu_df['freqs'], np.abs(accel_y_fft), label='FFT Accel Y')
# axs[3].plot(imu_df['freqs'], np.abs(accel_z_fft), label='FFT Accel Z')
# axs[3].set_ylabel('Amplitude')
# axs[3].set_xlabel('Frequency [Hz]')
# axs[3].legend(loc='upper right')
# axs[3].grid(True)

# Anzeige
plt.show()

# tests
# print(imu_df['velocity_z'])
# print(np.diff(imu_df['velocity_z']))
# print(np.std(np.diff(imu_df['velocity_z'])))
# print(np.mean(np.diff(imu_df['velocity_z'])))