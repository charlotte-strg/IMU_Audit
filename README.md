# IMU_Audit

Ein Programm zur IMU-basierten Steuerung des Low-Cost-Roboters **Murmecha**. Ziel ist es, verschiedene Steuerungsmodelle zu evaluieren und die Genauigkeit von Bewegungsmustern wie z. B. Quadratfahrten zu untersuchen.

## Projektüberblick

Untersucht wird, ob und wie gut sich IMU-Daten zur Steuerung von Murmecha eignen – im Vergleich zur klassischen motorbasierten Methode. Implementiert wurden:

- Motorbasierte Steuerung  
- Numerische Integration der Gyroskopdaten  
- Madgwick-Filter zur Echtzeitorientierung  
- PID-Regelung zur Streckenkorrektur

## Projektstruktur
IMU_Audit/
├── src/
│   └── runs_on_bot/
│       ├── main.cpp          # Einstiegspunkt für den Robotercode
│       ├── square.cpp        # Steuerung ohne PID
│       ├── square_pid.cpp    # Steuerung mit PID-Regler
├── notebooks/                # Jupyter Notebooks zur Analyse
├── include/                  # Header-Dateien
├── lib/                      # Abhängigkeiten (u.a. murmecha-core)
└── platformio.ini            # PlatformIO-Projektdatei

## Verwendung

### Voraussetzungen

- [PlatformIO](https://platformio.org/) installiert (z. B. als VS Code Plugin)
- Arduino-kompatibler Microcontroller (z. B. ESP32-S3)
- USB-Verbindung zum Roboter

### Setup und Upload

1. Projekt mit PlatformIO öffnen
2. In `main.cpp` gewünschte `setup()`/`loop()` aktivieren:
   - `square.cpp` → Steuerung ohne PID
   - `square_pid.cpp` → Steuerung mit PID-Regler
3. Optional: Pfaddefinitionen und Parameter anpassen
4. Code auf den Roboter laden

## Analyse

IMU- und Kameradaten werden über serielle Schnittstellen gesammelt und mit Python/Jupyter ausgewertet:

- Pfadgenauigkeit
- Drift- und Rauschanalyse (z. B. via FFT)
- Vergleich der Steuerungsmodelle

## Verwendete Technologien

- C++ (Robotersteuerung)
- Python / Jupyter Notebooks (Datenauswertung)
- PlatformIO (Projektmanagement)
- murmecha-core (Hardwareanbindung)
- ESP32-S3 (Microcontroller)