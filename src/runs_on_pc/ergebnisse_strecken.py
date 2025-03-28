import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import ergebnisse_utils as utils

# file paths
path_pid_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/PID_tests_neu/2025-03-11_15-10-26.sensors.dat"
path_pid_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/PID_tests_neu/2025-03-11_15-10-26.dat"

path_motors_none_rounded_imu = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.sensors.dat"
path_motors_none_rounded_cam = "c:/Users/charl/Desktop/Uni_Leipzig/MA/Code/PlatformIO/IMU_Audit/MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.dat"

# Motorbasierte Fahrweise

column_names_motors = ["frame", "start_stopp", "empty0", "empty1", "empty2", "empty3", "empty4"]

df_path_motors_none_rounded_imu = pd.read_csv(path_motors_none_rounded_imu, sep=' ', header=None, names=column_names_motors)
df_path_motors_none_rounded_cam = pd.read_csv(path_motors_none_rounded_cam, sep=' ', header=0)

df_imu_motors = df_path_motors_none_rounded_imu
df_imu_motors['start_stopp'] = df_imu_motors['start_stopp'].apply(lambda x: 1 if x == 0 else 0) 
df_cam_motors = df_path_motors_none_rounded_cam


# PID

column_names_pid = ["frame", "event"]

df_path_imu_pid_rounded_imu = pd.read_csv(path_pid_rounded_imu, sep=' ', header=0)
df_path_imu_pid_rounded_imu.rename(columns={"event": "start_stopp"}, inplace=True)
df_path_imu_pid_rounded_cam = pd.read_csv(path_pid_rounded_cam, sep=' ', header=0)

df_imu_pid = df_path_imu_pid_rounded_imu
df_cam_pid = df_path_imu_pid_rounded_cam
# df_imu_pid filtern nach nicht start_stopp == 2
df_imu_pid = df_imu_pid[df_imu_pid['start_stopp'] != 2]

# get results_df
results_df_motors = utils.calc_before_after_angles_seg_motors(df_imu_motors, df_cam_motors)
results_df_pid = utils.calc_before_after_angles_seg_pid(df_imu_pid, df_cam_pid)

# print(results_df_motors)

# get segments_df
segments_df_motors = utils.find_segments(results_df_motors)
segments_df_pid = utils.find_segments(results_df_pid)

# Beispiel-Daten (in Radian)
motorbasiert = segments_df_motors['angle_diff'].to_list()
pid = segments_df_pid['angle_diff'].to_list()


datasets = [motorbasiert, pid]
titles = ["Motorbasiert\nabgerundete Ecken", "IMU-basiert\nPID-Regler\nabgerundete Ecken"]


#print mean of each dataset
for dataset, title in zip(datasets, titles):
    print(title)
    mean = np.mean(dataset)
    mean = utils.rad_to_deg(mean)
    print(mean)

# utils.plot_curves_boxplot(datasets, titles)
utils.plot_segments_boxplot(datasets, titles)

