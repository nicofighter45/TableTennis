from data_reception import *
from processing import *
from speed import *

Taille_pix, h, x = data("TableTennis/Résultat des expérience + graphe/Rebond sur sol.txt")


def coefficient_de_rebond(liste):
    max = maximum(liste)
    coefficient = []
    for k in range(len(max) - 1):
        coefficient.append(mt.sqrt(max[k + 1] / max[k]))
    print(coefficient)
    return (moyenne(coefficient))


h, x = h[1:], x[1:]
rebilitation_de_liste(x)
rebilitation_de_liste(h)
h = premièrevaleurhaute(h)
print(maximum(h))
print(coefficient_de_rebond(h))
