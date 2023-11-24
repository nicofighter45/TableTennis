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
        liste.append(1 / 2 * m * k ** 2)
    return liste


def energie_cinétique2(position):
    liste = []
    for k in position:
        liste.append(1 / 2 * m * k ** 2)
    return liste
