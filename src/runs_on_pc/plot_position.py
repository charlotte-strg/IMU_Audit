import pandas as pd
import matplotlib.pyplot as plt

### read in files

# ##  Quadrat, runde Ecken, IMU, Integral
# last_row = 8043
# # last_row = 752
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n durch Integration der Gyroskopdaten"
# file_path = "MA_files/Recherche/Ergebnisse/IMU_Integral_Rounded/2025-01-22_16-54-27.dat"

#  Quadrat, runde Ecken, IMU, Madgwick
# last_row = 8259
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit Madgwick-Filter"
# file_path = "MA_files/Recherche/Ergebnisse/IMU_Madgwick_Rounded/2025-01-22_15-06-50.dat"

##  Quadrat, scharfe Ecken, IMU, Madgwick
# last_row = 5997
# # last_row = 571
# title = "Fahren eines Quadrats mit scharfen Ecken:\n Bewegungspfad des Roboters in der Arena \n mit Madgwick-Filter"
# file_path = "MA_files/Recherche/Ergebnisse/IMU_Madgwick_Square/2025-01-22_16-03-09.dat"

# #  Quadrat, abgerundete Ecken, Motors, None
# last_row = 7931
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit motorbasierter Fahrweise"
# file_path = "MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.dat"

##  Quadrat, abgerundete Ecken, IMU, PID
# problem: weiß nicht, wie ich start/stopp für ecken definieren soll
# lösung: immer, wenn bot einen target-wert mit modulo pi (0.0, 1.5707, 2...., 3.1425, etc.) erreicht, ist die ecke fertig gefahren oder anders:
# wenn target-wert nicht modulo pi ist, wird aktuell eine kurve gefahren
last_row = 1700
title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit PID-Regler"
file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_13-58-11.dat"

##  Gerade, IMU, PID
# title = "Fahren einer Gerade:\n Bewegungspfad des Roboters in der Arena \n mit PID-Regler"
# file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_14-05-57.dat"

##  Gerade, Motors, None
# title = "Fahren einer Gerade:\n Bewegungspfad des Roboters in der Arena \n ohne PID-Regler" # aber mit welcher Einstellung???
# file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_14-09-34.dat"

### read in data
# read first 3000 rows
# data = pd.read_csv(file_path, sep='\s+', nrows=3000)
# read all rows except the last 20
# data = pd.read_csv(file_path, sep='\s+', skipfooter=20, engine='python')
# read all rows
data = pd.read_csv(file_path, sep='\s+')
# read all rows until last_row
data = data[data["frame"]<=last_row]



### plot
# plot for x and y
x = data['x']
y = data['y']
plt.figure(figsize=(8, 6))
# cud_colors = ["#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7"]
plt.plot(x, y, marker="o", markersize=1, label="Pfad Roboter 16", color="#0072B2")
plt.annotate("Start", (x.iloc[0], y.iloc[0]), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=9)
plt.annotate("Ende", (x.iloc[-1], y.iloc[-1]), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=9)

# Markierungen für Kurvenbeginn und Kurvenende
frames_to_mark = {799: "B1", 861: "E1", 
 1022: "B2", 1179: "E2",
 1342: "B3", 1425: "E3",
 1589: "B4", 1651: "E4",}
# frames_to_mark = {i:i for i in range(0, 1700, 50)}
for frame, label in frames_to_mark.items():
    if frame in data["frame"].values:
        x_mark = data.loc[data["frame"] == frame, "x"].values[0]
        y_mark = data.loc[data["frame"] == frame, "y"].values[0]
        plt.scatter(x_mark, y_mark, color='red', s=20, label=label)
        plt.annotate(label, (x_mark, y_mark), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=9)


plt.xlabel("x (in mm)")
plt.ylabel("y (in mm)")
# plt.title(title)
plt.axis('equal')  # Skalierung für x und y gleich
plt.grid(True)
plt.legend()
plt.show()

# # plot for px and py
# px = data['px']
# py = data['py']

# plt.figure(figsize=(8, 6))
# plt.plot(px, py, marker="o", markersize=1, label="Robot Path (x, y)", color="green")

# plt.annotate("Start", (px.iloc[0], py.iloc[0]), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=9)
# plt.annotate("End", (px.iloc[-1], py.iloc[-1]), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=9)

# plt.xlabel("px-Koordinate (m)")
# plt.ylabel("py-Koordinate (m)")
# plt.title("Bewegungspfad des Roboters in der lokalen Ebene")
# plt.axis('equal')  # Equal scaling for x and y
# plt.grid(True)
# plt.legend()
# plt.show()
