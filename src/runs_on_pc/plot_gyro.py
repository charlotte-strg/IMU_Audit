import pandas as pd
import matplotlib.pyplot as plt

# Originalsignal plotten
# imu_df = pd.read_csv("data/imu_data_drift.csv")

# Rotation plotten
imu_df = pd.read_csv("data/imu_data_with_rotation.csv")
imu_fourier_df = pd.read_csv("data/backup_imu_data_with_rotation_fourier.csv")

# Hauptfiguren erstellen
fig = plt.figure(figsize=(12, 10))

# Erste Subfigur (Zeitbasiert)
subfigs = fig.subfigures(2, 1, height_ratios=[1, 1])

# Subfig 1: Gyro und Rotation
axs = subfigs[0].subplots(2, 1, sharex=True)

# Gyro
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_x'], label='Gyro X')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_y'], label='Gyro Y')
axs[0].plot(imu_df['time_micros'] / 1e6, imu_df['gyro_z'], label='Gyro Z')
axs[0].set_ylabel('Gyro [Rad/s]')
axs[0].legend(loc='upper right')
axs[0].grid(True)

# Rotation
# TODO total rotation berechnung einfügen
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_x'], label='Rotation X')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_y'], label='Rotation Y')
axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['rotation_z'], label='Rotation Z')
# axs[1].plot(imu_df['time_micros'] / 1e6, imu_df['total_rotation'], label='Total Rotation')
axs[1].set_ylabel('Rotation [°]')
axs[1].set_xlabel('Time [s]')
axs[1].legend(loc='upper right')
axs[1].grid(True)

# Subfig 1: Automatische Skalierung der x-Achse
for ax in axs:
    ax.autoscale(enable=True, axis='x', tight=True)

# Subfig 2: FFT-Daten
ax_fft = subfigs[1].subplots(1, 1)

# FFT
ax_fft.plot(imu_fourier_df['gyro_freq'], imu_fourier_df['gyro_fourier_x'], label='FFT Gyro X')
ax_fft.plot(imu_fourier_df['gyro_freq'], imu_fourier_df['gyro_fourier_y'], label='FFT Gyro Y')
ax_fft.plot(imu_fourier_df['gyro_freq'], imu_fourier_df['gyro_fourier_z'], label='FFT Gyro Z')
ax_fft.set_ylabel('Amplitude')
ax_fft.set_xlabel('Frequency [Hz]')
ax_fft.legend(loc='upper right')
ax_fft.grid(True)

# FFT Subplot automatisch skalieren (Frequenz-Achse)
ax_fft.autoscale(enable=True, axis='x', tight=True)
ax_fft.set_yscale('log')

# Anzeige
plt.show()

# tests
# print(imu_df['velocity_z'])
# print(np.diff(imu_df['velocity_z']))
# print(np.std(np.diff(imu_df['velocity_z'])))
# print(np.mean(np.diff(imu_df['velocity_z'])))