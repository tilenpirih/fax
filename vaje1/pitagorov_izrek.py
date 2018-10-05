#Copyright by Tilen Pirih

"""Pitagorov izrek
Napiši program, ki uporabnika vpraša po dolžinah katet pravokotnega trikotnika in izpiše dolžino hipotenuze. Kot piše na vrhu strani, za koren uporabimo funkcijo sqrt, ki jo dobimo, če na začetek programa napišemo from math import *.

Kateta? UPORABNIK VTIPKA 3
Kateta? UPORABNIK VTIPKA 4
Hipotenuza: 5.0"""

from math import *

kateta1=float(input("kateta 1?"))
kateta2=float(input("kateta 2?"))
hipotenuza=sqrt(pow(kateta1,2)+pow(kateta2,2))
print("Hipotenuza je ",hipotenuza)

