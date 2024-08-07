import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Originalsignal plotten
# imu_df = pd.read_csv("data/imu_data_drift.csv")

# Rotation plotten
# imu_df = pd.read_csv("data/imu_data_with_rotation_backup.csv")
imu_df = pd.read_csv("data/imu_data_with_rotation.csv")

# Subplots erstellen
fig, axs = plt.subplots(2, 1, figsize=(10, 10), sharex=True)

# Gyro
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_x'], label='Gyro X')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_y'], label='Gyro Y')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_z'], label='Gyro Z')
axs[0].set_ylabel('Gyro [Rad/s]')
axs[0].legend(loc='upper right')
axs[0].grid(True)

# Rotation
# TODO macht die total rotation berechnung sinn?
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_x'], label='Rotation X')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_y'], label='Rotation Y')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_z'], label='Rotation Z')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['total_rotation'], label='Total Rotation')
axs[1].set_ylabel('Rotation [°]')
axs[1].set_xlabel('Time [s]')
axs[1].legend(loc='upper right')
axs[1].grid(True)

# Anzeige
plt.show()

# tests
# print(imu_df['velocity_z'])
# print(np.diff(imu_df['velocity_z']))
# print(np.std(np.diff(imu_df['velocity_z'])))
# print(np.mean(np.diff(imu_df['velocity_z'])))