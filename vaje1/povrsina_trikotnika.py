#Copyright by Tilen Pirih
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

