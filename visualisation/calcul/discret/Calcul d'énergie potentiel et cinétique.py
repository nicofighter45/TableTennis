import matplotlib.pyplot as plt
import numpy as np
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

def courbe(h,nombre):
    rebilitation_de_liste(h)
    h = premièrevaleurhaute(h)
    convertisseur(h, Taille_pix)
    t = [k * 10e-3 for k in range(len(h))]
    potentiel = np.array(energie_potentielle(h))
    cinetique = np.array([0] + energie_cinétique1(h))
    Mecanique = potentiel + cinetique
    plt.plot(t, potentiel, label="Énergie potentiel")
    plt.plot(t, cinetique, label="Énergie cinétique")
    plt.plot(t, Mecanique, label="Énergie mécanique")
    plt.title("Évolution de l'énergie au cours du temps / courbe {}".format(nombre))
    plt.ylabel("Énergie en Joule")
    plt.xlabel("Temps en seconde")
    plt.legend()
    plt.show()
    plt.clf()
for k in range (15) :
    Taille_pix, h, x = data("TableTennis/output/4-jets-de-balle/tracked-{}.txt".format(k))
    courbe(x,k)
