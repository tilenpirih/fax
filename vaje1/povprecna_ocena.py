#Copyright by Tilen Pirih
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

povprecje=float((ocena1+ocena2+ocena3)/3)
print("Povprecje je: ", povprecje)
print("Srednja vrednost je: ",round(povprecje) )
