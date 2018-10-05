#Copyright by Tilen Pirih
from math import *
temp_F = float(input("Temperature [F]? "))
temp_C = (temp_F - 32)*5/9 
temp_K = temp_C + 273.15

print(temp_F, "F je enako", temp_C, "C ali", temp_K, "K")