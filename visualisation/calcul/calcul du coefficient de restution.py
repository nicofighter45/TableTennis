from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

Taille_pix, h, x = data("TableTennis/calcul/Rebond sur sol.txt")


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
