import numpy as np
import pandas as pd


def get_fourier(df_column: pd.Series, df_sample_time_delta: float) -> "tuple[np.ndarray, np.ndarray]":
    
    """
    Berechnet die Fourier-Transformation einer gegebenen Spalte von Daten.

    ## Parameter:
    - df_column (pd.Series): Eine Spalte mit numerischen Daten, z.B. eine Achse von einem Gyroskop.
    - sample_time_delta (float): Der Zeitabstand zwischen erstem und letztem Messpunkt (Zeile) in Mikrosekunden.

    ## Rückgabe:
    tuple: Ein Tupel bestehend aus zwei Arrays:
        - x-Achse: Frequenzen
        - y-Achse: Fourier-Transformation der Eingabedaten
    """
    signal = df_column.values

    # x und y entsprechen achsen in der fouriertransformation
    x_frequencies = np.fft.fftfreq(len(signal), df_sample_time_delta)
    y_fouriertransform = np.fft.fft(signal)

    # get rid of negative frequencies
    mask_no_neg = x_frequencies >= 0
    x_frequencies_no_neg = x_frequencies[mask_no_neg]
    y_fouriertransform_no_neg = y_fouriertransform[mask_no_neg]

    # return (y_fouriertransform_no_neg, x_frequencies_no_neg)
    return x_frequencies_no_neg, y_fouriertransform_no_neg
    