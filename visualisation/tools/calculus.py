import math as mt

m = 7e-3
rho = 1.293
S = 8 / 3 * mt.pi * 4e-2 ** 2
g = 9.81

def vitesse(position):
    vitesse = []
    p = 1
    for k in range(len(position) - 1):
        vitesse.append((position[k + 1] - position[k]) / (1e-2 * p))
    return vitesse

def energie_potentielle(position):
    liste = []
    for k in position:
        liste.append(m * g * k)
    return liste


def energie_cinétique1(position):
    liste = []
    v = vitesse(position)
    for k in v:
        e = 1 / 2 * m * k ** 2
        if e < 0.5 :
            liste.append(e)
        else : liste.append(liste[-1])
    return liste


def energie_cinétique2(position):
    liste = []
    for k in position:
        liste.append(1 / 2 * m * k ** 2)
    return liste

def trouver_minimums_locaux(grandeurs):
    indices_minimums = []
    for i in range(1, len(grandeurs) - 1):
        if grandeurs[i - 1] > grandeurs[i] < grandeurs[i + 1]:
            indices_minimums.append(i)
    return indices_minimums

def trouver_maximums_locaux(grandeurs):
    indices_maximums = []
    for i in range(1, len(grandeurs) - 1):
        if grandeurs[i - 1] < grandeurs[i] > grandeurs[i + 1]:
            indices_maximums.append(i)
    return indices_maximums

def moy_energie_meca(mecanique,precision) :
    liste=[]
    somme = mecanique[0]
    p=1
    for k in range(1,len(mecanique)) :
        if abs(mecanique[k]/(somme/p)-1) <= precision : 
            somme+=mecanique[k]
            p+=1
        else:
            liste.append(somme/p)
            somme=mecanique[k]
            p=1
    liste.append(somme/p)
    return(liste)