import matplotlib.pyplot as plt
import numpy as np
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *
from visualisation.tools.incertitudes import *

def courbe(h,nombre):
    rebilitation_de_liste(h)
    h = premièrevaleurhaute(h,1)
    convertisseur(h, Taille_pix)
    h=coherence(h)
    t = [k * 10e-3 for k in range(len(h))]
    potentiel = np.array(energie_potentielle(h))
    max = localmaxexperimentale(h)
    max1,max2 = [max[k][0]for k in range (len(max))],[max[k][1]*10e-3 for k in range (len(max))]
    max1 = energie_potentielle(max1)
    cinetique = [0] + energie_cinétique1(h)
    Mecanique = potentiel + cinetique
    plt.plot(t, potentiel, label="Énergie potentiel")
    plt.plot(t, cinetique, label="Énergie cinétique")
    plt.plot(t, Mecanique, label="Énergie mécanique")
    plt.title("Évolution de l'énergie au cours du temps / courbe {}".format(nombre))
    plt.scatter(max2,max1 , color='red', label='Points d\'interpolation')
    plt.ylabel("Énergie en Joule")
    plt.xlabel("Temps en seconde")
    plt.legend()
    plt.show()
    plt.clf()
    return(max1)

coefficient = []
for k in range (30) :
    Taille_pix, h, x = data("TableTennis/output/5-jets-de-balle/tracked-{}.txt".format(k))
    liste = courbe(x,k)
    for k in range (len(liste)-1) :
        if liste[k+1]/liste[k] < 1 :
            coef = mt.sqrt(liste[k+1]/liste[k])
            if not coef > 0.9 : coefficient.append(coef)
moy , incertitude, maximu, minimum, nombre_de_coefficient = str(moyenne(coefficient)),str(incertitude_type_A(coefficient)), str(max(coefficient)),str(min(coefficient)),str(len(coefficient))
print(moy , incertitude, maximu, minimum, nombre_de_coefficient,coefficient)
#result("TableTennis/output/results/rebond.txt", "Rebond sur table",moy , incertitude, maximu, minimum, nombre_de_coefficient)