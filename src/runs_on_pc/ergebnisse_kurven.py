import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import ergebnisse_utils as utils


# file paths
path_motors_none_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.sensors.dat"
path_motors_none_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.dat"
path_imu_integral_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Integral_Rounded/2025-01-22_16-54-27.sensors.dat"
path_imu_integral_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Integral_Rounded/2025-01-22_16-54-27.dat"
path_imu_madgwick_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Madgwick_Rounded/2025-01-22_15-06-50.sensors.dat"
path_imu_madgwick_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Madgwick_Rounded/2025-01-22_15-06-50.dat"
path_imu_madgwick_square_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Madgwick_Square/2025-01-22_16-03-09.sensors.dat"
path_imu_madgwick_square_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/IMU_Madgwick_Square/2025-01-22_16-03-09.dat"
path_pid_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/PID_tests_neu/2025-03-11_15-10-26.sensors.dat"
path_pid_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/PID_tests_neu/2025-03-11_15-10-26.dat"

# header für imu daten
column_names_motors = ["frame", "start_stopp", "empty0", "empty1", "empty2", "empty3", "empty4"]
column_names_madgwick = ["frame", "start_stopp", "angle", "o_w", "o_x", "o_y", "o_z"]
column_names_integral = ["frame", "start_stopp", "angle", "NaN", "gyro_x", "gyro_y", "gyro_z"]
column_names_pid_alt = ["frame", "angle", "target", "pid_out", "vl", "vr"]
column_names_pid = ["frame", "event"]

# dateien einlesen in pd dataframes
df_path_motors_none_rounded_imu = pd.read_csv(path_motors_none_rounded_imu, sep=' ', header=None, names=column_names_motors)
df_path_motors_none_rounded_cam = pd.read_csv(path_motors_none_rounded_cam, sep=' ', header=0)
df_path_imu_integral_rounded_imu = pd.read_csv(path_imu_integral_rounded_imu, sep=' ', header=None, names=column_names_integral)
df_path_imu_integral_rounded_cam = pd.read_csv(path_imu_integral_rounded_cam, sep=' ', header=0)
df_path_imu_madgwick_rounded_imu = pd.read_csv(path_imu_madgwick_rounded_imu, sep=' ', header=None, names=column_names_madgwick)
df_path_imu_madgwick_rounded_cam = pd.read_csv(path_imu_madgwick_rounded_cam, sep=' ', header=0)
df_path_imu_madgwick_square_imu = pd.read_csv(path_imu_madgwick_square_imu, sep=' ', header=None, names=column_names_madgwick)
df_path_imu_madgwick_square_cam = pd.read_csv(path_imu_madgwick_square_cam, sep=' ', header=0)
df_path_imu_pid_rounded_imu = pd.read_csv(path_pid_rounded_imu, sep=' ', header=0)
df_path_imu_pid_rounded_cam = pd.read_csv(path_pid_rounded_cam, sep=' ', header=0)


# # nur für PID
# Daten einlesen, ersten Header entfernen, falls doppelt vorhanden, event in start_stopp umbenennen
df_path_imu_pid_rounded_imu = pd.read_csv(path_pid_rounded_imu, sep=' ', header=None, names=column_names_pid, skiprows=1)
df_path_imu_pid_rounded_imu.rename(columns={"event": "start_stopp"}, inplace=True)

# erstellen aller results dataframes mit den Spalten "frame", "start_stopp", "avg_angle_last_20", "avg_angle_next_20"
results_df_motors = utils.calc_before_after_angles(df_path_motors_none_rounded_imu, df_path_motors_none_rounded_cam)
results_df_integral = utils.calc_before_after_angles(df_path_imu_integral_rounded_imu, df_path_imu_integral_rounded_cam)
results_df_madgwick_rounded = utils.calc_before_after_angles(df_path_imu_madgwick_rounded_imu, df_path_imu_madgwick_rounded_cam)
results_df_madgwick_square = utils.calc_before_after_angles(df_path_imu_madgwick_square_imu, df_path_imu_madgwick_square_cam)
results_df_pid = utils.calc_before_after_angles(df_path_imu_pid_rounded_imu, df_path_imu_pid_rounded_cam)

# erstellen aller curves dataframes mit den Spalten "frame", "start_stopp", "avg_angle_last_20", "avg_angle_next_20"
curves_df_motors = utils.find_curves(results_df_motors)
curves_df_integral = utils.find_curves(results_df_integral)
curves_df_madgwick_rounded = utils.find_curves(results_df_madgwick_rounded)
curves_df_madgwick_square = utils.find_curves(results_df_madgwick_square)
curves_df_pid = utils.find_curves_pid(results_df_pid)

# utils.plot_curves(curves_df_motors)

# Beispiel-Daten (in Radian)
motorbasiert = curves_df_motors['angle_diff'].to_list()
imu_integration = curves_df_integral['angle_diff'].to_list()
madgwick_rounded = curves_df_madgwick_rounded['angle_diff'].to_list()
madgwick_square = curves_df_madgwick_square['angle_diff'].to_list()
pid = curves_df_pid['angle_diff'].to_list()

datasets = [motorbasiert, imu_integration]
titles = ["Motorbasierte\nFahrweise\nabgerundete Ecken", "IMU-basiert\nNumerische\nIntegration\nabgerundete Ecken"]
# datasets = [motorbasiert, imu_integration, madgwick_rounded, madgwick_square]
# titles = ["Motorbasierte\nFahrweise\nabgerundete Ecken", "IMU-basiert\nNumerische\nIntegration\nabgerundete Ecken", "IMU-basiert\nMadgwick\nabgerundete Ecken", "IMU-basiert\nMadgwick\nscharfe Ecken"]
# datasets = [motorbasiert, imu_integration, madgwick_rounded, madgwick_square, pid]
# titles = ["Motorbasierte\nFahrweise\nabgerundete Ecken", "IMU-basiert\nNumerische\nIntegration\nabgerundete Ecken", "IMU-basiert\nMadgwick\nabgerundete Ecken", "IMU-basiert\nMadgwick\nscharfe Ecken", "IMU-basiert\nPID-Regler\nabgerundete Ecken"]


#print mean of each dataset
for dataset, title in zip(datasets, titles):
    print(title)
    mean = np.mean(dataset)
    mean = utils.rad_to_deg(mean)
    print(mean)

utils.plot_curves_boxplot(datasets, titles)


