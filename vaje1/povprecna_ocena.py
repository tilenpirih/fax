#Copyright by Tilen Pirih
"""Povprečna ocena
Napiši program, ki mu uporabnik vpiše oceno, ki so jo pri matematiki dobili Ana, Benjamin in Cilka. Program naj izračuna in izpiše povprečno oceno.

Izziv za razmišljujoče tipe: recimo, da ne bi radi izpisali povprečne temveč srednjo oceno. Če so Ana, Benjamin in Cilka dobili 3, 2 in 5, bi radi izpisali 3. Izziv: sprogramiraj to reč brez uporabe pogojnih stavkov ali česa podobno "naprednega". Konkretno, uporabljaj le funkcije input, print, min in max. Namig: min in max lahko prejmeta poljubno število argumentov. Pomisli tudi na to, da imaš samo tri osebe; pri štirih ta trik ne bi vžgal.

Ocena [Ana]? UPORABNIK VTIPKA 2
Ocena [Benjamin]? UPORABNIK VTIPKA 4
Ocena [Cilka]? UPORABNIK VTIPKA 5
Povprečje: 3.6666666666666665
Srednja vrednost: 4.0
Še boljši izziv: recimo, da imamo štiri števila, a, b, c in d. Izpisati želimo tretje število po velikosti. Še vedno uporabljamo le max in min. Namig: mogoče se splača klicati min in max s samo po dvema argumentoma, a večkrat. Morda obstaja tudi kakšna rešitev, kjer kličemo z več argumenti. Morda; ne vem. :)"""

from math import *

a=int(input("Ocena [Ana]?"))
b=int(input("Ocena (Benjamin]?"))
c=int(input("Ocena [Cilka]?"))
d=int(input("Ocena [Cilka]?"))

min1=min(a,b)
min2=min(a,c)
min3=min(a,d)
min4=min(b,c)
min5=min(b,d)
min6=min(c,d)

print(max(min1,min2,min3,min4,min5,min6))

povprecje=float((a+b+c)/3)
print("Povprecje je: ", povprecje)
print("Srednja vrednost je: ",round(povprecje) )


