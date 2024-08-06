import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Originalsignal plotten
imu_df = pd.read_csv("data/imu_data_drift.csv")

# Rotation plotten
# imu_df = pd.read_csv("data/imu_data_with_rotation_backup.csv")
# imu_df = pd.read_csv("data/imu_data_with_rotation.csv")

# Distanz/Geschwindigkeit plotten
# imu_df = pd.read_csv("data/imu_data_drift_with_distance.csv")
# imu_df = pd.read_csv("data/backup_imu_data_with_distance.csv")

# plotten
plt.figure()

# Distanz
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['distance_x'], label='Distance X')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['distance_y'], label='Distance Y')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['distance_z'], label='Distance Z')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['distance_total'], label='Total Distance')
# plt.ylabel('Distance [m]')

# Geschwindigkeit
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['velocity_x'], label='Velocity X')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['velocity_y'], label='Velocity Y')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['velocity_z'], label='Velocity Z')
# plt.ylabel('Velocity [m/s]')

# Beschleunigung/Accel
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['accel_x'], label='Accel X')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['accel_y'], label='Accel Y')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['accel_z'], label='Accel Z')
# plt.ylabel('Accel [m/s^2]')

# Gyro
plt.plot(imu_df['time_micros'] / 1e6, imu_df['gyro_z'], label='Gyro Z')
plt.plot(imu_df['time_micros'] / 1e6, imu_df['gyro_x'], label='Gyro X')
plt.plot(imu_df['time_micros'] / 1e6, imu_df['gyro_y'], label='Gyro Y')
plt.ylabel('Gyro [Rad]')

# Rotation
# TODO: noch nicht sicher die richtigen zahlen
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['rotation_x'], label='Rotation X')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['rotation_y'], label='Rotation Y')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['rotation_z'], label='Rotation Z')
# plt.plot(imu_df['time_micros'] / 1e6, imu_df['total_rotation'], label='Total Rotation')
# plt.ylabel('Rotation [°]')

plt.xlabel('Time [s]')

plt.legend()
plt.show()

# tests
# print(imu_df['velocity_z'])
# print(np.diff(imu_df['velocity_z']))
# print(np.std(np.diff(imu_df['velocity_z'])))
# print(np.mean(np.diff(imu_df['velocity_z'])))