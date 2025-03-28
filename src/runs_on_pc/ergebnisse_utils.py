import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def cyclic_mean(series, period=2 * np.pi):
    """
    Berechnet den Mittelwert einer zyklischen Serie, z. B. für Winkel oder Uhrzeiten.

    Args:
        series (pd.Series): Die Serie mit zyklischen Werten.
        period (float): Der Zyklus der Werte (z. B. 2π für Winkel, 24 für Uhrzeiten).
    
    Returns:
        float: Der zyklische Mittelwert.
    """
    # Umwandlung der Werte in Radien (wenn nicht bereits im passenden Bereich)
    radians = series * (2 * np.pi / period)  # Normierung auf [0, 2π]
    
    # Umrechnung in kartesische Koordinaten
    x = np.cos(radians)
    y = np.sin(radians)
    
    # Durchschnitt der kartesischen Koordinaten berechnen
    mean_x = np.mean(x)
    mean_y = np.mean(y)
    
    # Zurück in zyklischen Winkel umrechnen
    mean_angle = np.arctan2(mean_y, mean_x)
    
    # Ergebnis auf den ursprünglichen Zyklus zurückskalieren
    cyclic_mean_value = (mean_angle % (2 * np.pi)) * (period / (2 * np.pi))
    
    return cyclic_mean_value

def cyclic_difference(value1, value2, period=2 * np.pi):
    """
    Berechnet die zyklische Differenz zwischen zwei Werten.

    Args:
        value1 (float): Erster Wert.
        value2 (float): Zweiter Wert.
        period (float): Der Zyklus der Werte (Standard: 2π).
    
    Returns:
        float: Die zyklische Differenz im Bereich [-period/2, period/2].
    """
    # Berechne die rohe Differenz
    raw_diff = value1 - value2
    
    # Bringe die Differenz in den Bereich [-period/2, period/2]
    cyclic_diff = (raw_diff + period / 2) % period - period / 2
    
    return cyclic_diff

# Funktion, die zwei dataframes entgegennimmt ein neues dataframe results_df erstellt mit den Spalten "frame", "start_stopp", "avg_angle_last_20", "avg_angle_next_20"
def calc_before_after_angles(df_imu, df_cam):

    results = []

    # Iteration über jede Zeile von df_imu
    for _, row in df_imu.iterrows():
        frame_value = row['frame']
        start_stopp = row['start_stopp']
        
        # Finde die Zeile in df_cam, die diesem frame entspricht
        df_cam_before_frame = df_cam[df_cam['frame'] < frame_value]
        
        # Berechne den Durchschnitt der letzten 20 Werte in der 'angle'-Spalte
        if len(df_cam_before_frame) >= 20:
            last_20_values = df_cam_before_frame['phi'].tail(20)
            avg_angle_last_20 = cyclic_mean(last_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_last_20 = cyclic_mean(df_cam_before_frame['phi'], period=2 * np.pi)
        
        # Finde die Zeile in df_cam, die dem nächsten frame entspricht
        df_cam_after_frame = df_cam[df_cam['frame'] > frame_value]

        # Berechne den Durchschnitt der nächsten 20 Werte in der 'angle'-Spalte
        if len(df_cam_after_frame) >= 20:
            next_20_values = df_cam_after_frame['phi'].head(20)
            avg_angle_next_20 = cyclic_mean(next_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_next_20 = cyclic_mean(df_cam_after_frame['phi'], period=2 * np.pi)
        
        # Füge das Ergebnis in das neue DataFrame hinzu
        results.append({'frame': frame_value, 'start_stopp': start_stopp, 'avg_angle_last_20': avg_angle_last_20, 'avg_angle_next_20': avg_angle_next_20})
    
    # Konvertiere die Ergebnisse in ein DataFrame
    result_df = pd.DataFrame(results)

    return result_df

def calc_before_after_angles_seg_pid(df_imu_pid, df_cam_pid):

    results_pid = []

    # Iteration über jede Zeile von df_imu_pid
    for _, row in df_imu_pid.iterrows():
        frame_value = row['frame']
        start_stopp = row['start_stopp']
        
        # Finde die Zeile in df_cam, die diesem frame entspricht
        df_cam_before_frame = df_cam_pid[df_cam_pid['frame'] < frame_value]
        
        # Berechne den Durchschnitt der letzten 20 Werte in der 'angle'-Spalte
        if len(df_cam_before_frame) >= 20:
            last_20_values = df_cam_before_frame['phi'].tail(20)
            avg_angle_last_20 = cyclic_mean(last_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_last_20 = cyclic_mean(df_cam_before_frame['phi'], period=2 * np.pi)
        
        # Finde die Zeile in df_cam, die dem nächsten frame entspricht
        df_cam_after_frame = df_cam_pid[df_cam_pid['frame'] > frame_value]

        # Berechne den Durchschnitt der nächsten 20 Werte in der 'angle'-Spalte
        if len(df_cam_after_frame) >= 20:
            next_20_values = df_cam_after_frame['phi'].head(20)
            avg_angle_next_20 = cyclic_mean(next_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_next_20 = cyclic_mean(df_cam_after_frame['phi'], period=2 * np.pi)
        
        # Füge das Ergebnis in das neue DataFrame hinzu
        results_pid.append({'frame': frame_value, 'start_stopp': start_stopp, 'avg_angle_last_20': avg_angle_last_20, 'avg_angle_next_20': avg_angle_next_20})

    # Konvertiere die Ergebnisse in ein DataFrame
    result_df_pid = pd.DataFrame(results_pid)

    # für pid tests: result_df abschneiden nach 109 werten
    # result_df_pid = result_df_pid.head(109)

    return result_df_pid

def calc_before_after_angles_seg_motors(df_imu_motors, df_cam_motors):
    
    results_motors = []

    # Iteration über jede Zeile von df_imu_motors
    for _, row in df_imu_motors.iterrows():
        frame_value = row['frame']
        start_stopp = row['start_stopp']
        
        # Finde die Zeile in df_cam, die diesem frame entspricht
        df_cam_before_frame = df_cam_motors[df_cam_motors['frame'] < frame_value]
        
        # Berechne den Durchschnitt der letzten 20 Werte in der 'angle'-Spalte
        if len(df_cam_before_frame) >= 20:
            last_20_values = df_cam_before_frame['phi'].tail(20)
            avg_angle_last_20 = cyclic_mean(last_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_last_20 = cyclic_mean(df_cam_before_frame['phi'], period=2 * np.pi)
        
        # Finde die Zeile in df_cam, die dem nächsten frame entspricht
        df_cam_after_frame = df_cam_motors[df_cam_motors['frame'] > frame_value]

        # Berechne den Durchschnitt der nächsten 20 Werte in der 'angle'-Spalte
        if len(df_cam_after_frame) >= 20:
            next_20_values = df_cam_after_frame['phi'].head(20)
            avg_angle_next_20 = cyclic_mean(next_20_values, period=2 * np.pi)
        else:
            # Falls weniger als 20 Werte vorhanden sind, den Durchschnitt der verfügbaren berechnen
            avg_angle_next_20 = cyclic_mean(df_cam_after_frame['phi'], period=2 * np.pi)
        
        # Füge das Ergebnis in das neue DataFrame hinzu
        results_motors.append({'frame': frame_value, 'start_stopp': start_stopp, 'avg_angle_last_20': avg_angle_last_20, 'avg_angle_next_20': avg_angle_next_20})

    # Konvertiere die Ergebnisse in ein DataFrame
    result_df_motors = pd.DataFrame(results_motors)

    # für Motors-Tests: result_df abschneiden nach 109 Werten
    # result_df_motors = result_df_motors.head(109)

    return result_df_motors

# Funktion, die ein DataFrame mit den Ergebnissen entgegennimmt und ein neues DataFrame mit den Kurven erstellt
def find_curves(result_df):

    # Neues DataFrame für die Ergebnisse
    curves = []
    curve_number = 1

    # Iteration über result_df in Paaren
    for i in range(len(result_df) - 1):
        if result_df.iloc[i]['start_stopp'] == 1 and result_df.iloc[i + 1]['start_stopp'] == 0:
            before_angle = result_df.iloc[i]['avg_angle_last_20']
            after_angle = result_df.iloc[i + 1]['avg_angle_next_20']
            angle_diff = cyclic_difference(after_angle, before_angle)

            frame_diff = result_df.iloc[i + 1]['frame'] - result_df.iloc[i]['frame']

            # Prüfen, ob angle_diff > π und ggf. π abziehen
            # if angle_diff > math.pi:
            #     angle_diff -= 2*math.pi
            
            # Füge die Werte der aktuellen Kurve hinzu
            curves.append({
                'curve_number': curve_number,
                'before_frame': result_df.iloc[i]['frame'],
                'before_angle': before_angle,
                'after_frame': result_df.iloc[i + 1]['frame'],
                'after_angle': after_angle,
                'angle_diff': angle_diff,
                'frame_diff': frame_diff,
            })
            curve_number += 1

    # Konvertiere die Liste der Kurven in ein DataFrame
    curves_df = pd.DataFrame(curves)

    # dataframe abschneiden nach kurve 40
    curves_df = curves_df[curves_df['curve_number'] <= 37]

    return curves_df

def find_curves_pid(result_df):
    # Neues DataFrame für die Ergebnisse
    curves = []
    curve_number = 1

    # result_df filtern nach start_stopp = 2
    result_df = result_df[result_df['start_stopp'] == 2]

    # zwischen jeder zeile und ihrer jeweils darauffolgenden zeile

    # Iteration über result_df in Paaren
    for i in range(len(result_df) - 1):

        before_angle = result_df.iloc[i]['avg_angle_last_20']
        after_angle = result_df.iloc[i + 1]['avg_angle_next_20']

        angle_diff = cyclic_difference(after_angle, before_angle)

        frame_diff = result_df.iloc[i + 1]['frame'] - result_df.iloc[i]['frame']

        # Prüfen, ob angle_diff > π und ggf. π abziehen
        # if angle_diff > math.pi:
        #     angle_diff -= 2*math.pi
        
        # Füge die Werte der aktuellen Kurve hinzu
        curves.append({
            'curve_number': curve_number,
            'before_frame': result_df.iloc[i]['frame'],
            'before_angle': before_angle,
            'after_frame': result_df.iloc[i + 1]['frame'],
            'after_angle': after_angle,
            'angle_diff': angle_diff,
            'frame_diff': frame_diff,
        })
        curve_number += 1
    # Konvertiere die Liste der Kurven in ein DataFrame
    curves_df_pid = pd.DataFrame(curves)

    # dataframe abschneiden nach kurve 36
    curves_df_pid = curves_df_pid[curves_df_pid['curve_number'] <= 37]

    return curves_df_pid

def find_segments(result_df):
    # Initialisieren der Variablen
    segments = []
    segment_number = 1

    # Schleife über result_df: wenn aktuelle Zeile start_stopp = 1 und nächste Zeile start_stopp = 0, dann Segment hinzufügen
    for i in range(len(result_df) - 1):
        if result_df.iloc[i]['start_stopp'] == 1 and result_df.iloc[i + 1]['start_stopp'] == 0:

            before_angle = result_df.iloc[i]['avg_angle_next_20']
            after_angle = result_df.iloc[i + 1]['avg_angle_last_20']

            angle_diff = cyclic_difference(after_angle, before_angle)

            start_frame = result_df.iloc[i]['frame']
            end_frame = result_df.iloc[i + 1]['frame']

            frame_diff = result_df.iloc[i + 1]['frame'] - result_df.iloc[i]['frame']

            segments.append({
                'segment_number': segment_number, 
                'start_frame': start_frame, 
                'end_frame': end_frame,
                'frame_diff': frame_diff,
                'before_angle': before_angle,
                'after_angle': after_angle,
                'angle_diff': angle_diff,
            })
            segment_number += 1

    print(segments)
    # Konvertiere die Liste der Segmente in ein DataFrame
    segments_df = pd.DataFrame(segments)

    segments_df = segments_df[segments_df['segment_number'] <= 36]

    return segments_df

def plot_curves(curves_df):
    plt.plot(curves_df['curve_number'], curves_df['angle_diff'])
    plt.show()

def plot_curves_boxplot(datasets, titles):
    """
    Plottet einen Boxplot aus beliebig vielen Datasets.
    
    Parameters:
    - datasets: Liste von Listen/Arrays mit Winkeldaten in Radiant.
    - titles: Liste von Strings, Titel der einzelnen Boxen.
    """
    # Umrechnung in Grad
    data_deg = [np.degrees(data) for data in datasets]
    
    # Dynamische Breite: ca. 1.5 pro Box + extra Platz
    fig_width = max(4, len(datasets) * 2)  # Spielraum für Labels
    
    # Plot
    fig, ax = plt.subplots(figsize=(fig_width, 5))
    
    ax.boxplot(
        data_deg,
        vert=True,
        patch_artist=True,
        labels=titles,
        showmeans=True,
        widths=0.5
    )
    
    # Stil & Beschriftung
    ax.set_ylabel("Winkel (in °)", fontsize=14)
    ax.axhline(y=90, color='r', linestyle='--', label=r"90° ($\pi$/2)")
    ax.legend()
    
    plt.xticks()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.show()

def plot_segments_boxplot(datasets, titles):
    """
    Plottet einen Boxplot aus beliebig vielen Datasets.
    
    Parameters:
    - datasets: Liste von Listen/Arrays mit Winkeldaten in Radiant.
    - titles: Liste von Strings, Titel der einzelnen Boxen.
    """
    # Umrechnung in Grad
    data_deg = [np.degrees(data) for data in datasets]
    
    # Dynamische Breite: ca. 1.5 pro Box + extra Platz
    fig_width = max(4, len(datasets) * 2)  # Spielraum für Labels
    
    # Plot
    fig, ax = plt.subplots(figsize=(fig_width, 6))
    
    ax.boxplot(
        data_deg,
        vert=True,
        patch_artist=True,
        labels=titles,
        showmeans=True,
        widths=0.5
    )
    
    # Stil & Beschriftung
    ax.set_ylabel("Winkel (in °)", fontsize=14)
    ax.axhline(y=0, color='r', linestyle='--', label=r"0°")
    ax.legend()
    
    plt.xticks()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.show()

def rad_to_deg(radians):
    return round(np.degrees(radians),2)