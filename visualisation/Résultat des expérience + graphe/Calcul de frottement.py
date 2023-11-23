import math as mt

import data_reception as dt
import speed as sp
from processing import *

m = 7e-3
rho = 1.293
S = 8 / 3 * mt.pi * (4e-2) ** 2
g = 9.81

Taille_pix, h, x = dt.data("TableTennis/Résultat des expérience + graphe/Rebond sur raquette.txt")


def coefficient_de_frottement(position):
    vitesse2 = sp.vitesse(position)
    acceleration = sp.vitesse(vitesse2)
    coefficient = []
    for k in range(len(acceleration)):
        if vitesse2[k] != 0 and vitesse2[k + 1] != 0 and acceleration[k] > 0:
            C_x = (m * acceleration[k] - m * g) / (0.5 * rho * S * (vitesse2[k] ** 2))
            coefficient.append(C_x)
    return (moyenne(position))


rebilitation_de_liste(h)
h = premièrevaleurhaute(h)
h = descente(h)
convertisseur(h, Taille_pix)
print(coefficient_de_frottement(h))
