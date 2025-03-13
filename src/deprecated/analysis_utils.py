import numpy as np
import pandas as pd
from scipy.signal import butter, lfilter, freqz
import socket
import struct
import csv
import time


def get_fourier(df_value_column: pd.Series, df_time_column: pd.Series, time_unit: str="micros") -> "tuple[np.ndarray, np.ndarray]":
    
    """
    Berechnet die Fourier-Transformation einer gegebenen Spalte von Daten.

    ## Parameter:
    - df_value_column (pd.Series): Eine Spalte mit numerischen Daten, z.B. eine Achse von einem Gyroskop.
    - df_time_column (pd.Series): Eine Spalte mit Zeitpunkten in Mikrosekunden/Sekunden.

    ## Rückgabe:
    tuple: Ein Tupel bestehend aus zwei Arrays:
        - x-Achse: Frequenzen
        - y-Achse: Fourier-Transformation der Eingabedaten
    """
    if time_unit == "micros":
        time_delta = (df_time_column.iloc[-1] - df_time_column.iloc[0]) / 1e6
    elif time_unit == "s":
        time_delta = df_time_column.iloc[-1] - df_time_column.iloc[0]
    else:
        raise ValueError("time_unit must be 'micros' or 's'.")
    
    signal = df_value_column.values
    time_step = time_delta / len(signal)

    # x und y entsprechen achsen in der fouriertransformation
    x_frequencies = np.fft.fftfreq(len(signal), d=time_step)
    y_fouriertransform = np.fft.fft(signal)

    # get rid of negative frequencies
    mask_no_neg = x_frequencies >= 0
    x_frequencies_no_neg = x_frequencies[mask_no_neg]
    y_fouriertransform_no_neg = y_fouriertransform[mask_no_neg]

    # return (y_fouriertransform_no_neg, x_frequencies_no_neg)
    return x_frequencies_no_neg, y_fouriertransform_no_neg
    
def butter_lowpass(cutoff, fs, order=5):
    """
    Erstellt einen Butterworth-Tiefpassfilter.
    """

    nyq = 0.5 * fs  # Nyquist-Frequenz
    normal_cutoff = cutoff / nyq
    # print(f"{normal_cutoff=}")
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a


def butter_lowpass_filter(data, cutoff, fs, order=5):
    """
    Wendet einen Butterworth-Tiefpassfilter auf ein Signal an.
    """

    first_sample_offset = data[0]
    data_centered = data - first_sample_offset

    b, a = butter_lowpass(cutoff, fs, order=order)
    y_centered = lfilter(b, a, data_centered)

    y = y_centered + first_sample_offset

    return y


def receive_data(rec_time_sek: float) -> pd.DataFrame:
    '''
    abwandlung von receive_data_to_csv.py
    änderung: aufnahmezeit in sekunden statt anzahl an schritten
    '''
    
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('10.0.2.11', 4444))

    BUFFER_SIZE = 1024
    
    df_sensor_data = pd.DataFrame(columns=['time_micros', 'accel_x', 'accel_y', 'accel_z', 'gyro_x', 'gyro_y', 'gyro_z'])
    i=0
    start_time = time.time()
    while True:
        i += 1

        data, addr = sock.recvfrom(BUFFER_SIZE)
        time, ax,ay,az,gx,gy,gz = struct.unpack('<Qffffff', data)
        df_sensor_data = df_sensor_data.append({'time_micros': time, 'accel_x': ax, 'accel_y': ay, 'accel_z': az, 'gyro_x': gx, 'gyro_y': gy, 'gyro_z': gz}, ignore_index=True)

        delta_time = time - start_time
        if delta_time > rec_time_sek:
            # check if i ist ein vielfaches von BUFFER_SIZE
            if i % BUFFER_SIZE == 0:
                break

    return df_sensor_data


# testszenarien
#     kommen daten über wifi an?
#     low pass gegen hardware lowpass testen (C++)
#     berechnung weg/rotation für vergleich mit wahrheitswerten aus arena
#     madgwick/mahony/kalman vergleich performance/genauigkeit 


# def master(
#         parameter_sets: list[TestCalcParams]  # type: ignore
#     ):
#     # daten sammeln
#     # accel/gyro daten über wifi? live senden oder speichern?
#     df_sensor_data = receive_data(10)

#     # for parameter_set in parameter_sets:
#         # berechnungen
#         # berechnung lowpass? hard/soft lp? mit welchem cutoff?
#         # berechnung fourier transformation?
#         # berechnung weg/rotation?
#         # berechnung madgwick/mahony/kalman?
    
#     # visualisierung
#     # visualisierung gewünscht? wovon?

#     # speichern der ...
#         # sensordaten
#         # params
#         # visualisierung

#     # ausgabe einer empfehlung
#         # eingabe
#         # bestes parameterset
#     pass


# enum mit werten: madgwick, mahony, kalman, undefined
class SensorFusionAlgo:
    MADGWICK = "madgwick"
    MAHONY = "mahony"
    KALMAN = "kalman"
    UNDEFINED = "undefined"

# klasse mit start, stop und step feldern
# soll einen generator bereitstellen, der bei start startet und bei stop aufhört und in steps iteriert
# start, stop, step sind floats
# start < stop
# step > 0 (größe der schritte)
# deshalb neu definiert, weil range nur für ints funktioniert
class FloatRange:
    def __init__(self, start: float, stop: float, step: float):
        self.start = start
        self.stop = stop
        self.step = step

    # wird aufgerufen, wenn eine for-schleife über ein Range objekt iteriert
    def __iter__(self):
        current = self.start
        while current < self.stop:
            yield current
            current += self.step

    def __str__(self):
        return f"FloatRange({self.start}, {self.stop}, {self.step})"
    def __repr__(self):
        return str(self)

# TODO: testen
class TestCalcParams:
    def __init__(
            self,
            sample_len_sec: float,
            cutoff_accel: float,
            cutoff_gyro: float,
            order_accel: int,
            order_gyro: int,
            sensor_fusion_algo: SensorFusionAlgo
            ):
        self.sample_len_sec = sample_len_sec
        self.cutoff_accel = cutoff_accel
        self.cutoff_gyro = cutoff_gyro
        self.order_accel = order_accel
        self.order_gyro = order_gyro
        self.sensor_fusion_algo = sensor_fusion_algo
        # if not isinstance(sample_len_sec, (int, float)):
        #     raise ValueError("sample_len_sec must be an int or a float.")
        # self.sample_len_sec = sample_len_sec

        # if isinstance(cutoff_accel, (int, float)):
        #     # falls cutoff_accel ein float oder int ist, Range objekt mit einem wert erzeugen, um permutation zu vereinfachen
        #     self.cutoff_accel = FloatRange(cutoff_accel, cutoff_accel, 1)
        # elif isinstance(cutoff_accel, FloatRange):
        #     self.cutoff_accel = cutoff_accel
        # else:
        #     raise ValueError("cutoff_accel must be a float or a FloatRange object.")
        
        # if isinstance(cutoff_gyro, (int, float)):
        #     # falls cutoff_gyro ein float oder int ist, FloatRange objekt mit einem wert erzeugen, um permutation zu vereinfachen
        #     self.cutoff_gyro = FloatRange(cutoff_gyro, cutoff_gyro, 1)
        # elif isinstance(cutoff_gyro, FloatRange):
        #     self.cutoff_gyro = cutoff_gyro
        # else:
        #     raise ValueError("cutoff_gyro must be a float or a FloatRange object.")
        
        # if isinstance(order_accel, (int, float)):
        #     # falls order_accel ein float oder int ist, FloatRange objekt mit einem wert erzeugen, um permutation zu vereinfachen
        #     self.order_accel = FloatRange(order_accel, order_accel, 1)
        # elif isinstance(order_accel, FloatRange):
        #     self.order_accel = order_accel
        # else:
        #     raise ValueError("order_accel must be a float or a FloatRange object.")
        
        # if isinstance(order_gyro, (int, float)):
        #     # falls order_gyro ein float oder int ist, FloatRange objekt mit einem wert erzeugen, um permutation zu vereinfachen
        #     self.order_gyro = FloatRange(order_gyro, order_gyro, 1)
        # elif isinstance(order_gyro, FloatRange):
        #     self.order_gyro = order_gyro
        # else:
        #     raise ValueError("order_gyro must be a float or a FloatRange object.")
        
        # self.sensor_fusion_algo = sensor_fusion_algo

    # # will aus meiner TestCalcParams mit zu optimierenden, 
    # # undefinierten werten eine reihe an TestCalcParams erzeugen, 
    # # für die jedes attribut definiert ist.
    # # 
    # # iterator, welcher parameter sets zurück gibt. 
    # # falls einer der parameter SensorFusionAlgo.UNDEFINED ist, oder ein Range Objekt ist, 
    # # sollen mehrere parameter sets erzeugt werden, die über diesen parameter permutieren
    # # 
    # # falls ein parameter ein Range Objekt ist, sollen die parameter sets so erzeugt werden, dass alle möglichen kombinationen der parameterwerte aus den Range Objekten erzeugt werden
    # # falls ein parameter SensorFusionAlgo.UNDEFINED ist, sollen alle möglichen SensorFusionAlgo werte durchprobiert werden
    # # falls ein parameter ein float ist, soll dieser wert für alle parameter sets gleich sein
    # # falls ein parameter ein int ist, soll dieser wert für alle parameter sets gleich sein
    # def __iter__(self):
    #     for cutoff_accel in self.cutoff_accel:
    #         for cutoff_gyro in self.cutoff_gyro:
    #             for order_accel in self.order_accel:
    #                 for order_gyro in self.order_gyro:
    #                     if self.sensor_fusion_algo == SensorFusionAlgo.UNDEFINED:
    #                         for sensor_fusion_algo in [SensorFusionAlgo.MADGWICK, SensorFusionAlgo.MAHONY, SensorFusionAlgo.KALMAN]:
    #                             yield TestCalcParams(self.sample_len_sec, cutoff_accel, cutoff_gyro, order_accel, order_gyro, sensor_fusion_algo)
    #                     else:
    #                         # frage: ist self.sensor_fusion_algo hier nicht UNDEFINED?
    #                         yield TestCalcParams(self.sample_len_sec, cutoff_accel, cutoff_gyro, order_accel, order_gyro, self.sensor_fusion_algo)

    # gibt eine string repräsentation des objekts zurück
    def __str__(self):
        return f"TestCalcParams(sample_len_sec={self.sample_len_sec}, cutoff_accel={self.cutoff_accel}, cutoff_gyro={self.cutoff_gyro}, order_accel={self.order_accel}, order_gyro={self.order_gyro}, sensor_fusion_algo={self.sensor_fusion_algo})"
    # gibt eine string repräsentation des objekts zurück
    def __repr__(self):
        return str(self)
