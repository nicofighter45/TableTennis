import math as mt
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
import data_reception as dt
import processing as p
import speed as sp
from data_reception import *
from processing import *
from speed import *

Taille_pix, h,x = data("TableTennis/Résultat des expérience + graphe/Courbe du robot.txt")
def pente(h):
    v=vitesse(h)
    pente=[]
    for k in range (len(v)) :
        for j in range (k,len(v)):
            if j!= k : pente.append((v[k]-v[j])/(10e-3*(k-j)))
        return(moyenne(pente))

rebilitation_de_liste(h)
h=premièrevaleurhaute(h)
h=descente(h)[:-1]
print(pente(h))