#Copyright by Tilen Pirih

"""Površina trikotnika
Napiši program, ki uporabnika vpraša po dolžinah stranic poljubnega trikotnika in izpiše njegovo ploščino, ter ploščini včrtanega in očrtanega kroga.

Dolžina stranice a? UPORABNIK VTIPKA 3
Dolžina stranice b? UPORABNIK VTIPKA 4
Dolžina stranice c? UPORABNIK VTIPKA 5
Površina trikotnika: 6.0
Površina včrtanega kroga: 3.141592653589793
Površina očrtanega kroga: 19.634954084936208"""

from math import *

s1=float(input("Dolzina stranice 1?"))
s2=float(input("Dolzina stranice 2?"))
s3=float(input("Dolzina stranice 3?"))

s=(s1+s2+s3)/2 # s je polovični obseg
p=sqrt(s*(s-s1)*(s-s2)*(s-s3))

r_vcrt=p/s
p_vcrt=pi*r_vcrt**2

r_ocrt=(s1*s2*s3)/(4*p)
p_ocrt=pi*r_ocrt**2

print("Površina trikotnika: ", p)
print("Površina včrtanega kroga: ", p_vcrt)
print("Površina očrtanega kroga: ", p_ocrt)

