### FRAGEN FÜR KONSULTATION
----------------------------------------------------------------- 
- fft mit t auf der x-achse oder frequenzen

### AKTUELL
-----------------------------------------------------------------
- x-achse für fft beginnt schon vor 0, vor allem bei accel, damit peak bei 0 sichtbar



## Theorieteil, den ich schon recherchieren/schreiben kann 
## (?) - noch nicht sicher, ob für MA relevant
- types of noise einordnen, zusammenhang mit fft 
- bandpassfilter und alternativen

## Recherche:
- was sind lineare (signal-)filter/finite impulse response?
- quaternionen/gesamtrotation/komplexe zahlen

## Sonstiges/Nice to have:
nice to have:
- wenn bot auf dem kopf steht, UV-LED ausschalten und beep
- signal to noise ratio berechnen für mein csv signal
- charakterisierung für signal (z.b. energie, irgendein maß, nicht amplitude) --> vergleich, wurzel der summe der quadrate von allen samples (mit/ohne lp vergleich)
- analyse: frequency response von meinem filter (faktoren mit denen einzelne frequenzen verstärkt/abgeschwächt wurden durch einsatz filter --> plotten)


### ON HOLD
-----------------------------------------------------------------





### ERLEDIGT
-----------------------------------------------------------------
- core lib verwenden unter software/core (nicht kompatibel für platform.io)
- david schickt mir 2 dateien: in platformio projekt reinhauen und damit IMU ansprechen
- IMU recherche was ist das wie funktioniert das?
- rauschen in der robotik/in der IMU wodurch entsteht es und wie kriegt man es weg?
IMU auslesen
- zitationsstil: IEEE
- verbautes IMU modell: Datenrate ca. 1 kHz, Modell LSM 6DS3, kein Magnetometer
- konstruktion der digitalen wasserwaage besprechen
- müsste man den robo auf der oberfläche der wasserwaage festmachen, damit usb-kabel nicht die lage beeinflusst?
- haben wir ein gerät, das gesichert bestimmte winkel erzeugen kann, während er die IMU/und dann den roboter im ganzen dreht und wir schauen dann, wie groß der gemessene unterschied ist? nein, dafür müsste man einen anderen roboter bauen
- was für filter sind denn bisher in unserer imu? hat hardwarefilter (low-pass dinge), aber noch nie was damit gemacht, im zweifel immer erst hw-filter versuchen, dann softwarefilter
- IMU plot/werte in datei/tabelle speichern
- copilot installieren
- für fft buffer installieren (erst aufnahmen, dann schicken): 2 sek (bzw. 2000 samples, weil 1 khz) aufnehmen (als 3-vektor direkt aufnehmen), warten und dann erst über kabel an laptop schicken, damit nicht schicken und aufnehmen gleichzeitig passiert
- git installieren
- fourier-transformation --> auf welcher sequenz kommt signal an? (filter für rausch-sequenzen)
- programmierten low pass draufknallen, schauen was passiert, butterworth low pass draufgeknallt (https://www.electronics-tutorials.ws/de/filtern/butterworth-filter-design.html, stand: 22.7., https://medium.com/analytics-vidhya/how-to-filter-noise-with-a-low-pass-filter-python-885223e5e9b7 stand 22.7.)
- rauschen charakter (weiß, pink, grau) --> info aus der fft 
--> white noise (dieser abbildung folgend: https://www.google.de/imgres?q=noise%20white%20pink%20brown&imgurl=https%3A%2F%2Fwww.cnet.com%2Fa%2Fimg%2Fresize%2F36a72e59aa218b2db00492d6ae1e7234b664257b%2Fhub%2F2019%2F07%2F08%2Ffa526d4b-1d72-4e21-a008-3aed0abe0210%2Fcolors-of-noise.jpg%3Fauto%3Dwebp%26width%3D1200&imgrefurl=https%3A%2F%2Fwww.cnet.com%2Fhealth%2Fsleep%2Fbest-noise-colors-for-sleep-quality%2F&docid=9JtP2tvm2vG7VM&tbnid=BhuBK7O6izi-sM&vet=12ahUKEwj92o6-4bCHAxXravEDHe-pCtUQM3oECFgQAA..i&w=1200&h=675&hcb=2&ved=2ahUKEwj92o6-4bCHAxXravEDHe-pCtUQM3oECFgQAA stand 22.7., siehe auch bild ![noise_types_fft](noise_types_fft.png) aus chatgpt mit code dort)
- "signal to noise ratio" verstehen
- bot über wifi nutzen lernen
- welche kennzahlen geben auskunft darüber, ob mein lowpass gut funktioniert?
- problem: falsche einheit für distanz irgendwo in der berechnung/integration... sollten sowas wie 0,1m sein, sind aber 100m, also verschoben um faktor 1000, erdrotation rausgerechnet, code sieht okay aus, distanz aber immer noch verschoben von ~0,1m zu 4m
- ordentlich git aufsetzen für IMU-Audit: warum kann ich nicht committen? wegen murmecha repo?
- grundsätzlich nochmal über sinnvolle projektstruktur nachdenken: IMU Analysis und IMU Audit zusammenlegen? brauche ich die murmecha-module?
- serial to csv löschen oder deprecaten? wohin?
- ist die beschleunigung lokal oder global? --> lokal
- ich brauche nochmal das programm mit dem der ladestand der angeschlossenen batterie auf dem display angezeigt wird --> murmecha::display::show_info_screen;
- plot-funktion schreiben, mit der sich alles mögliche direkt plotten lässt velocity,  accel, gyro, signal, fft, etc . func params: x (time), y (signal werte)
- ungeklärt: beeinflusst die drehung der erde den drift über eine zeit von 15 sekunden? --> pro stunde 0,004 grad erdrotation einfluss
- nur für gyro: drift in radian/sekunde
- fft in plot_accel und plot_gyro
- welche achsen brauchen wir für die berechnung der distanz/rotation? z-achse ist relevant, weil nur aufsicht in arena messbar
- fragen, wie (gesamt-)rotation bestimmt werden soll
- test: bei accel-daten den ersten punkt als bias von allem abziehen --> wieder rausgenommen, weil das erst geht, nachdem man das rauschen entfernt wurde
- Plotten:
	1. Komplettes dashboard erstellen:
		a) alle Sensoren einzeln plotten
		b) IMU (accel, gyro) plotten (10 sek), auf welchen frequenzen ist das Rauschen am stärksten?
		c) sensoren plotten für bestimmte unterschiede, 
		z.b. bei 90 grad drehung, wo ist NOSW, unten/oben
	2. khz aufnehmen (1 sek)
	3. stehen (0,5 sek), dann losfahren, währenddessen aufnehmen
- verworfen: jupyter notebook aufteilen in mehrere notebooks mit einzelfunktionen (daten erstellen, fft-analyse, plotten, etc.) und diese durch python-file sowohl hintereinander durchlaufen lassen können als auch in notebook zwischenschritte/viz nachvollziehen können, wenn man will
- erdbeschleunigung/erdrotation korrekt aus IMU daten rausrechnen? bisher quick and dirty 9,81 abgezogen --> nicht korrekt, weil wir durch die 9,81 wissen, ob es auf dem kopf liegt, rampe hochfährt etc. --> wieder rausnehmen!
- orientierung im raum algo von madgwick mal googlen
- distanz-problem nochmal anschauen
- MA-daten in github sichern
- in quaternionen ist 3D-rotation darstellbar, madgwick-algo gibt auch in quaternionen aus, YT-videos schauen, keine eulerwinkel benutzen
- lowpass software integrieren in calc-dateien
- orientierung im raum algo von madgwick implementieren. mögliche vorbilder:
	--> https://github.com/morgil/madgwick_py
	--> https://github.com/rfayat/madgwick_imu/tree/master (mit beispiel und IMU-daten aus csv)
	--> https://pypi.org/project/AHRS/ (py-Paket inkl. Madgwick Implementierung)
	--> C++ implementierung im original-paper
- kontinuierliches aufnehmen für längere zeit --> geht jetzt für accel-live-daten, bisher ohne speicherung
- optimal: IMU benötigt nur zeit bis zum nächsten buffer, um rechenoperationen zu machen (z.b. rauschen rausrechnen) --> muss auf dem bot laufen
- woran erkenne ich gute einstellungen für den lp-filter? cutoff, order?
- gyro-daten in rad/s? nötig für madgwick! -- yes
- wenn mit gemessenen datensamples gearbeitet wird ist die versuchung groß, das gesamte array zur berechnung zu verwenden. das geht bei live-messungen aber nicht, weil wir nicht gleichzeitig alle datenpunkte vorliegen haben. 
- in windows link-sammlung: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
- Kalibrierung IMU:
	1. signal von imu so sauber wie möglich filtern (offset rausrechnen, kalibrieren etc.)?
	2. Wasserwaage für IMU bauen/offset herausfinden
	3. Wasserwaage Anzeige auf display programmieren
- mit repo von ivan dashboard ausprobieren: funktioniert das auf windows? brauche ich open gl?
- masterfunktion:
max vorschlag: 
	signal: x,y listen
	transformationen: signal -> signal
	plot_signal: signal -> plot
charlottes vorschlag:
	plot_signal(signal, [lp-hard, lp, normal], params)
	plot_fft(...)
	plot_signal_fft(...)
- overleaf MA dokument reinigen
- gliederung in overleaf aufsetzen
- Grundlagen/Einführung in IMU Recherche
- masterfunktion zum testen schreiben in python
- masterfunktion erstellen, die auf bot läuft (algorithmus vorher fertig optimiert, parameter müssen vorher selektiert werden --> entscheiden welcher lowpass, entscheiden welcher sensor-fusion-filter, etc.), rauschen rausrechnen, distanz-berechnungen (fertig für accel, gyro/rotation noch einarbeiten, sobald berechnung mit madgwick klar ist)
- filter-vergleich: madgwick/mahony/kalman
- video tobias
- mögliche implementierungen madgwick in square.cpp: stumpf (ist 90grad? falls ja weiter, falls nicht, zurück/vor drehen) oder
- madgwick fertigschreiben
- stehen/losfahren/90° drehung/weiterfahren/stehen, nur stehen (drift), drehen (360/180/90), kurve fahren und strecke vergleichen (ist strecke_accel == madgwick(strecke_accel, rotation_gyro)?)
- arenaaufnahmen vorbereiten/daten sammeln für: 
		fft
		lowpass-filter
		analyse fahrtweg gesamt (via doppel-integration accel)
- kann mit python schreiben, muss aber später in c++ funktionieren mit libs, die arduino-kompatibel sind (liste angeblich auf platformIO)
- radius auf 0 für quadrat ohne runde ecken
- pid controller (zielwinkel vorgeben) implementieren (sonst ausblick)
- ausblick: visualisierung auf dem display für einfacheres handling der kalibrierung/debugging
- saubere test-aufnahmen machen (arena): 
		quadrat madwick
		quadrat madgwick (runde ecken)
		quadrat integral
		quadrat integral (runde ecken)
		stehen/drift (1 s)
- aus daten von david extrahieren, wie genau die kurven gefahren wurden unabhängig von den geraden; dafür startpunkt und endpunkt der kurve finden und winkel zwischen ausrichtung(start) und ausrichtung(ende) errechnen --> ausgeben, brauche ich dafür nochmal eine aufnahme, die daten für startpunkt mit aufzeichnet? bisher nur ausgabe am ende der kurve
- "zwischenstufe" via doppelintegration aufnahme arena (möglicherweise für einen anwendungsfall mit weniger rechenleistung geeignet)
- mal versuchen: referenzsystem ändern - schwerkraft rausrechnen z.b. mit bandpass 
- diskrete fourier-transformation
- möglichkeiten für gute visualisierung zum vergleich unterschiedlicher varianten (lowpass, kein lowpass, lowpass hardware, etc.), schön, aber momentan nicht am wichtigsten
- aus positionsdaten der arena: ~2 sek vor/nach kurve --> winkel zwischen beiden geraden errechnen und abgleichen mit orientierung/angle aus bot daten, matchen der frame_nr
- phi ist absoluter winkel zwischen datenpunkt und x-achse, wechselt bei quadrat ende anfang zwischen pi/-pi --> muss normiert oder festgelegt werden, z.b. wenn punkte in der nähe von pi sind, 2pi abziehen
- darf ich bunte abbildungen haben? --> ja, aber schauen, dass es barrierefrei ist für farbenblinde
- pid controller implementieren für kurven UND geraden (inkl. madgwick):
	madgwick durchgehend berechnen
	zielwinkel im verhältnis zum ausgangspunkt 0: 0,90,180,-90,0
- präsentationstermin ausmachen
- alle quadrate: es fehlt ein datenset zum ende der kurve (siehe plot_position), neue aufnahmen machen
- welches modell als vergleich zu PID auf gerade benutzt? motors oder integral?
- haben wir die dft aufnahme 1s oder 2s laufen lassen?
- hunspell, aspell für spellchecking (regelbasiert)
- abbildungen nochmal generieren, dabei datensets als csv speichern und sicherstellen, dass pro methode genau 40 kurven betrachtet werden
- abbildungen fertig formatieren: tix für abbildungen in latex, R + ggplot oder Python + matplotlib am besten in pdf oder svg format, barrierefreie farben, berechnete abweichungen von rad in grad umrechnen
- fourier-transformation abbildung erstellen
- sliding window für integration: praktisch, einen datenpunkt "aus der zukunft" zu haben, wenn man integriert, weil es dann entspannter wird, was auch immer das bedeutet
- low pass hardware aktivieren und testen, LPF2 (siehe IMU-handbuch), vergleich  mit signal to noise ratio in db