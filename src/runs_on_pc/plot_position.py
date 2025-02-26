import pandas as pd
import matplotlib.pyplot as plt

### read in files

##  Quadrat, runde Ecken, IMU, Integral
# #problem: aufnahme startet zwar schon vorher aber kurve 1 ist eigentlich schon kurve 3 (bei frame 752)
# last_row = 8043
# #last_row = 752
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n durch Integration der Gyroskopdaten"
# file_path = "MA_files/Recherche/Ergebnisse/IMU_Integral_Rounded/2025-01-22_16-54-27.dat"

##  Quadrat, runde Ecken, IMU, Madgwick
# eine ecke zu wenig
last_row = 8259
title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit Madgwick-Filter"
file_path = "MA_files/Recherche/Ergebnisse/IMU_Madgwick_Rounded/2025-01-22_15-06-50.dat"

##  Quadrat, scharfe Ecken, IMU, Madgwick
# #problem: aufnahme startet zwar schon vorher aber kurve 1 ist eigentlich schon kurve 3 (bei frame 571)
# last_row = 5997
# last_row = 571
# title = "Fahren eines Quadrats mit scharfen Ecken:\n Bewegungspfad des Roboters in der Arena \n mit Madgwick-Filter"
# file_path = "MA_files/Recherche/Ergebnisse/IMU_Madgwick_Square/2025-01-22_16-03-09.dat"

##  Quadrat, abgerundete Ecken, Motors, None
# # problem: zwar start (reihe 4801) aufgezeichnet, aber nicht stopp
# last_row = 8057 
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit motorbasierter Fahrweise"
# file_path = "MA_files/Recherche/Ergebnisse/Motors_None_Rounded/2025-01-22_17-03-38.dat"

##  Quadrat, abgerundete Ecken, IMU, PID
# # problem: weiß nicht, wie ich start/stopp für ecken definieren soll
# last_row = ?
# title = "Fahren eines Quadrats mit abgerundeten Ecken:\n Bewegungspfad des Roboters in der Arena \n mit PID-Regler"
# file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_13-58-11.dat"

##  Gerade, IMU, PID
# funktioniert
# title = "Fahren einer Gerade:\n Bewegungspfad des Roboters in der Arena \n mit PID-Regler"
# file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_14-05-57.dat"

##  Gerade, Motors, None
# funktioniert
# title = "Fahren einer Gerade:\n Bewegungspfad des Roboters in der Arena \n ohne PID-Regler" # aber mit welcher Einstellung???
# file_path = "MA_files/Recherche/Ergebnisse/PID_tests/2025-02-06_14-09-34.dat"

### read in data
# read first 3000 rows
# data = pd.read_csv(file_path, sep='\s+', nrows=3000)
# read all rows except the last 20
# data = pd.read_csv(file_path, sep='\s+', skipfooter=20, engine='python')
# read all rows
# data = pd.read_csv(file_path, sep='\s+')
# read all rows until last_row
data = pd.read_csv(file_path, sep='\s+', nrows=last_row)

### plot
# plot for x and y
x = data['x']
y = data['y']
plt.figure(figsize=(8, 6))
# cud_colors = ["#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7"]
plt.plot(x, y, marker="o", markersize=1, label="Pfad Roboter 16", color="#0072B2")
plt.annotate("Start", (x.iloc[0], y.iloc[0]), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=9)
plt.annotate("Ende", (x.iloc[-1], y.iloc[-1]), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=9)
plt.xlabel("x (in mm)")
plt.ylabel("y (in mm)")
plt.title(title)
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
