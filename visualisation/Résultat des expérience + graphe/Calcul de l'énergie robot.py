import matplotlib.pyplot as plt
import numpy as np

from data_reception import *
from processing import *
from speed import *

Taille_pix, h, x = data("TableTennis/Résultat des expérience + graphe/Courbe du robot.txt")


def courbe(h):
    rebilitation_de_liste(h)
    h = premièrevaleurhaute(h)
    convertisseur(h, Taille_pix)
    t = [k * 10e-3 for k in range(len(h))]
    potentiel = np.array(energie_potentielle(h))
    cinetique = np.array([0] + energie_cinétique(h))
    Mecanique = potentiel + cinetique
    plt.plot(t, potentiel, label="Énergie potentiel")
    plt.plot(t, cinetique, label="Énergie cinétique de la vitesse selon Uz")
    plt.plot(t, Mecanique, label="Énergie mécanique")
    plt.title("Évolution de l'énergie au cours du temps")
    plt.ylabel("Énergie en Joule")
    plt.xlabel("Temps en seconde")
    plt.legend()
    plt.show()
    plt.clf()


courbe(x)


def courbe2(h):
    rebilitation_de_liste(h)
    h = premièrevaleurhaute(h)
    convertisseur(h)
    t = [k * 10e-3 for k in range(len(h))]
    cinetique = np.array(energie_cinétique1(h)[:-1])
    plt.plot(t[1:-1], cinetique, label="Énergie cinétique de la vitesse selon Uy")
    plt.title("Évolution de l'énergie au cours du temps")
    plt.ylabel("Énergie en Joule")
    plt.xlabel("Temps en seconde")
    plt.legend()
    plt.show()


courbe2(h)
