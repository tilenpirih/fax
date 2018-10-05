#Copyright by Tilen Pirih

"""Če na začetek programa napišemo

from time import *
bomo, med drugim, dobili funkcijo time, ki vrne čas v sekundah od nekega trenutka v davni preteklosti. Napiši program, ki uporabnika vpraša, koliko je 6 krat 7. Uporabnik bo premislil in vpisal odgovor. Program naj se ne ukvarja z odgovorom ter tem, ali je pravilen ali ne, temveč naj izpiše, koliko sekund je človek potreboval za razmišljanje.

Namig: če veš, koliko je bila ura pred klicem funkcije input in koliko je bila ura po klicu, znaš izračunati, koliko časa je minilo vmes.

Koliko je 6 krat 7? UPORABNIK VTIPKA 42
Za razmišljanje ste porabili 2.503019332885742 s."""

from math import *
import time

start = time.time()
st=float(input("Koliko je 6*7?"))
end = time.time()
print("Za razmišljanje ste porabili" ,end-start,"s")