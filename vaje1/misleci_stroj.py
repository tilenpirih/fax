#Copyright by Tilen Pirih
"""Napiši program, ki uporabnika vpraša za dve števili. Program izpiše njun produkt. Da pa bi naredil vtis, da razmišlja, produkta ne izpiše takoj, temveč pred tem malo počaka. Za čakanje uporabiš funkcijo sleep, ki jo dobiš, če je na začetku programa napisano

from time import *
Napišeš lahko tri različice programa. Po eni vedno čaka tri sekunde. Po drugi čaka naključno dolgo; koliko časa bo čakal, izžrebaš s funkcijo randint (za katero je potrebno na začetek programa napisati from random import *); čaka naj med eno in petimi sekundami. Tretja pa je takšna: večji ko je produkt, težji je račun. Računalnik naj čaka toliko sekund, kolikor znaša produkt, deljeno z 10. Če mu damo množiti 6 in 7, naj pred izpisom počaka 4.2 sekunde.

Prvo število? UPORABNIK VTIPKA 6
Drugo število? UPORABNIK VTIPKA 7
42.0"""

from math import *
import time
from random import*

time.sleep(3)
st1=float(input("Prvo število?"))
time.sleep(randint(1,5))
print(randint(1,5))
st2=float(input("Drugo število?"))
time.sleep(st1*st2/10)
print(st1*st2)



