import numpy as np
import pandas as pd
from scipy.signal import butter, lfilter, freqz


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