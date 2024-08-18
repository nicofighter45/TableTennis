import matplotlib.pyplot as plt
import numpy as np
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *
import visualisation.tools.Interpolation as it
Taille_pix, h,x = data('TableTennis/output/4-rebond-table/tracked-0.txt')
rebilitation_de_liste(h)
h=premièrevaleurhaute(h,0)
convertisseur(h,Taille_pix)
def courbe(h):
    interpolation_de_h = it.approximation(h,3)
    vitesse = it.der(interpolation_de_h)
    t=[k*10e-3 for k in range(100*(len(h)-7))]
    h = [interpolation_de_h(4*3+k) for k in t ]
    v = [vitesse(4*3+k) for k in t ]
    cinetique = energie_cinétique2(v)
    potentiel = energie_potentielle(h)
    potentiel = np.array(potentiel)
    cinetique = np.array(cinetique)
    Mecanique= potentiel + cinetique
    t = np.array(t)
    plt.plot(t,potentiel,label ="Énergie potentiel")
    plt.plot(t,cinetique,label ="Énergie cinétique")
    plt.plot(t,Mecanique,label="Énergie mécanique")
    plt.title("Évolution de l'énergie au cours du temps")
    plt.ylabel( "Énergie en Joule")
    plt.xlabel( "Temps en seconde")
    plt.legend()
    plt.show()
    plt.clf()
courbe(h)