#Copyright by Tilen Pirih
"""Indeks telesne teže
Napiši program, ki uporabnika vpraša, kako velik (v centimetrih) in kako masiven (v kilogramih) je. V odgovor naj izpiše indeks telesne mase (BMI) uporabnika.

Višina [cm]? UPORABNIK VTIPKA 180
Masa [kg]? UPORABNIK VTIPKA 75
Indeks telesne mase: 23.1481481481"""

from math import *

teza=float(input("Vnesi tezo [kg]"))
visina=float(input("Vnesi velikost [cm]"))
itm=teza/(visina/100)**2
print("Index telesne mase je: ",itm)