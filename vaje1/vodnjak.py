#Copyright by Tilen Pirih
"""Vodnjak
Če vržemo v vodnjak kamen in je v vodnjaku voda, se čez nekaj časa zasliši čof. Napiši program, ki mu uporabnik vpiše, koliko časa je minilo od trenutka, ko smo spustili kamen, do trenutka, ko je reklo čof, program pa izpiše globino vodnjaka. Če ne poznaš enačb, si pomagaj z wikipedijo

Čas [s]? UPORABNIK VTIPKA 1
Globina vodnjaka: 4.905 m"""

from math import *
cas=float(input("Vnesi čas [s]"))
globina=1/2*9.81*cas
print("Globina je: ", globina, " metrov")