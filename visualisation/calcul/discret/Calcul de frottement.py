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

Taille_pix,x,h = data("TableTennis/output/5-rebond-sol/tracked-0.txt")

def variation_énergie_mécanique(h):
    cinetique = [0] + energie_cinétique1(h)
    lissage(cinetique,2)
    potentiel = np.array(energie_potentielle(h))
    Mecanique = cinetique + potentiel
    maxi = max(Mecanique)
    temps = [k*1e-2 for k in range (len(Mecanique))]
    fonction = lineaire(Mecanique,temps)
    valeur = integrale(fonction,0,temps[-1])
    curv_trace(temps,Mecanique,0,0,"enegie","EM","Ec")
    return(maxi,valeur,maxi*len(temps)*1e-2 - valeur)

def travail_des_frottements(h) :
    vit =vitesse(h)
    nb = len(vit)
    somme = 0
    for k in range (nb) :
        somme += vit[k]**2 * 1e-2
    return(somme)

def coefficient_de_frottement_2(position) :
    rebilitation_de_liste(position)
    position = premièrevaleurhaute(position,1)
    convertisseur(position, Taille_pix)
    position=coherence(position)
    liste = partition(position)
    EM =[]
    Ec = []
    for k in liste :
        EM.append(variation_énergie_mécanique(k)[2])
        Ec.append(travail_des_frottements(k))
    curv_trace(Ec,EM,0,0,"enegie","EM","Ec")

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
            coefficient.append(C_x)
    return (moyenne(position))

coefficient_de_frottement_2(h)
