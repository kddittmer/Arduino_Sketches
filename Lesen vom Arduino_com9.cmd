@echo off
echo Setzt COM9 auf 115200 Baud, no Parity, 8 Databit 1 Stopbit
echo und oeffnet den COM9 Port fuer die Ausgabe des Arduino
mode com9: baud=115200 parity=n data=8 stop=1 >> nul
type com9: