import math as mt
import sys
import numpy as np
sys.path.insert(0, 'TableTennis')
from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *
from visualisation.tools.incertitudes import *
from visualisation.tools.Regression_linéaire import *
from visualisation.tools.Interpolation import *

m = 7e-3
rho = 1.293
S = 8 / 3 * mt.pi * (4e-2) ** 2
g = 9.81

def variation_énergie_mécanique(h):
    cinetique = [0] + energie_cinétique1(h)
    lissage(cinetique,2)
    potentiel = np.array(energie_potentielle(h))
    Mecanique = cinetique + potentiel
    maxi = max(Mecanique)
    temps = [k*1e-2 for k in range (len(Mecanique))]
    fonction = lineaire(Mecanique,temps)
    valeur = min(Mecanique)
    return(maxi,valeur,(-maxi + valeur))

def travail_des_frottements(h) :
    vit =vitesse(h)
    nb = len(vit)
    somme = 0
    for k in range (nb-1) :
        somme -= abs(vit[k]**2 * 1e-2 * S * m*(h[k+1]-h[k]))
    return(somme)

def coefficient_de_frottement_2(position) :
    rebilitation_de_liste(position)
    position = premièrevaleurhaute(position,1)
    convertisseur(position, Taille_pix)
    position=coherence(position)
    liste = partition(position)

    for k in liste :
        print(variation_énergie_mécanique(k))
        EM.append(variation_énergie_mécanique(k)[2])
        Ec.append(travail_des_frottements(k))

def coefficient_de_frottement(position):
    rebilitation_de_liste(position)
    position = premièrevaleurhaute(position,10e-3)
    position = descente(position)
    convertisseur(position, Taille_pix)
    vitesse2 = vitesse(position)
    acceleration = vitesse(vitesse2)  
    coefficient = []
    for k in range(len(acceleration)):
        if vitesse2[k] != 0 and vitesse2[k + 1] != 0 and acceleration[k] > 0:
            C_x = (m * acceleration[k] - m * g) / (0.5 * rho * S * (vitesse2[k] ** 2))
            coefficient.append(abs(C_x))
    return (moyenne(coefficient))
EM =[]
Ec = []
for k in range (14) :
    Taille_pix, x, h = data("TableTennis/output/5-rebond-sol/tracked-{}.txt".format(k))
    print(coefficient_de_frottement(h))
