#Copyright by Tilen Pirih
"""Krog
Napiši program, ki izračuna površino in obseg kroga, katerega polmer poda uporabnik. (Konstanto π boste dobili, če napišete pi, pri čemer mora na začetku programa pisati from math import *, kot smo opisali zgoraj.)

Polmer kroga? UPORABNIK VTIPKA 4.2
Obseg kroga: 26.389378290154262
Površina kroga: 55.41769440932395"""

from math import *
polmer=float(input("Polmer kroga?"))
obseg_kroga=2*pi*polmer
ploscina_kroga=pi*polmer**2
print("Obseg kroga: ", obseg_kroga)
print("Ploscina kroga: ",ploscina_kroga)