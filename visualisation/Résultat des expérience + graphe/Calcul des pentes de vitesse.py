import sys

from data_reception import *
from processing import *
from speed import *

sys.path.insert(0, 'TableTennis/Interpolation_de_point')
import Test_Regression_linéaire as li

Taille_pix, h, x = data("TableTennis/Résultat des expérience + graphe/Courbe du robot.txt")
rebilitation_de_liste(h)
h = premièrevaleurhaute(h)
h = descente(h)[:-1]
v = vitesse(h)
print(monte(v))
li.curv_trace(monte(v), [t * 10e-3 for t in range(len(monte(v)))], 0, 0, "dzcgze", "dzhdzh", "hezh")
