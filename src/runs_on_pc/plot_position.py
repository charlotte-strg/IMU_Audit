# plot position of bot using data MA_files/2025-01-15_12-33-01.dat

import pandas as pd
import matplotlib.pyplot as plt

file_path = "MA_files/2025-01-15_12-33-01.dat"
# read first 3000 rows
# data = pd.read_csv(file_path, sep='\s+', nrows=3000)
# read all rows
data = pd.read_csv(file_path, sep='\s+')

# plot for x and y
x = data['x']
y = data['y']
plt.figure(figsize=(8, 6))
plt.plot(x, y, marker="o", markersize=1, label="Pfad Roboter 22", color="green")
plt.annotate("Start", (x.iloc[0], y.iloc[0]), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=9)
plt.annotate("Ende", (x.iloc[-1], y.iloc[-1]), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=9)
plt.xlabel("x (in mm)")
plt.ylabel("y (in mm)")
plt.title("Bewegungspfad des Roboters in der Arena")
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
