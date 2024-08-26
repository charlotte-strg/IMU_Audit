import pandas as pd
import matplotlib.pyplot as plt

# Originalsignal plotten
# imu_df = pd.read_csv("data/imu_data_drift.csv")

# Distanz/Geschwindigkeit plotten
# imu_df = pd.read_csv("data/imu_data_drift_with_distance.csv")
imu_df = pd.read_csv("data/backup_imu_data_with_distance.csv")
imu_fourier_df = pd.read_csv("data/backup_imu_data_with_distance_fourier.csv")

# Hauptfiguren erstellen
fig = plt.figure(figsize=(16, 8))

# Erste Subfigur (Zeitbasiert)
subfigs = fig.subfigures(2, 1, height_ratios=[3, 1])

# Subfig 1: Beschleunigung, Geschwindigkeit und Distanz
axs = subfigs[0].subplots(3, 1, sharex=True)

# Beschleunigung/Accel
axs[0].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['accel_x'], label='Accel X')
axs[0].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['accel_y'], label='Accel Y')
axs[0].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['accel_z'], label='Accel Z')
axs[0].set_ylabel('Accel [m/s^2]')
axs[0].legend(loc='upper right')
axs[0].grid(True)

# Geschwindigkeit
axs[1].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['velocity_x'], label='Velocity X')
axs[1].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['velocity_y'], label='Velocity Y')
axs[1].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['velocity_z'], label='Velocity Z')
axs[1].set_ylabel('Velocity [m/s]')
axs[1].legend(loc='upper right')
axs[1].grid(True)

# Distanz
axs[2].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['distance_x'], label='Distance X')
axs[2].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['distance_y'], label='Distance Y')
axs[2].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['distance_z'], label='Distance Z')
axs[2].plot((imu_df['time_micros'] - imu_df['time_micros'].iloc[0]) / 1e6, imu_df['distance_total'], label='Total Distance')
axs[2].set_ylabel('Distance [m]')
axs[2].set_xlabel('Time [s]')
axs[2].legend(loc='upper right')
axs[2].grid(True)

# Subfig 1: Automatische Skalierung der x-Achse (Zeit)
for ax in axs:
    ax.autoscale(enable=True, axis='x', tight=True)

# Subfig 2: FFT-Daten
ax_fft = subfigs[1].subplots(1, 1)

# FFT
ax_fft.plot(imu_fourier_df['accel_freq'], imu_fourier_df['accel_fourier_x'], label='FFT Accel X')
ax_fft.plot(imu_fourier_df['accel_freq'], imu_fourier_df['accel_fourier_y'], label='FFT Accel Y')
ax_fft.plot(imu_fourier_df['accel_freq'], imu_fourier_df['accel_fourier_z'], label='FFT Accel Z')
ax_fft.set_ylabel('Amplitude')
ax_fft.set_xlabel('Frequency [Hz]')
ax_fft.legend(loc='upper right')
ax_fft.grid(True)

# FFT Subplot automatisch skalieren (Frequenz-Achse)
ax_fft.autoscale(enable=True, axis='x', tight=True)
ax_fft.set_yscale('log')
# ax_fft.set_xlim(-0.0001, 0.05)
# ax_fft.set_ylim(-100, 1000)
# ax_fft.autoscale(enable=True, axis='y', tight=True)

# Anzeige
plt.show()

# tests
# print(imu_df['velocity_z'])
# print(np.diff(imu_df['velocity_z']))
# print(np.std(np.diff(imu_df['velocity_z'])))
# print(np.mean(np.diff(imu_df['velocity_z'])))