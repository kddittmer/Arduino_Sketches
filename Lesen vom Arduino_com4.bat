@echo off
echo Setzt COM4 auf 115200 Baud, no Parity, 8 Databit 1 Stopbit
echo und oeffnet den COM4 Port fuer die Ausgabe des Arduino
mode com4: baud=115200 parity=n data=8 stop=1 >> nul
type com4:
