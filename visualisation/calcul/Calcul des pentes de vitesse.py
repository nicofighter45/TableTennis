import sys

from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

sys.path.insert(0, 'TableTennis/Interpolation_de_point')
import visualisation.test.Test_Regression_linéaire as li

Taille_pix, h, x = data("TableTennis/calcul/Courbe du robot.txt")
rebilitation_de_liste(h)
h = premièrevaleurhaute(h)
h = descente(h)[:-1]
v = vitesse(h)
print(monte(v))
li.curv_trace(monte(v), [t * 10e-3 for t in range(len(monte(v)))], 0, 0, "dzcgze", "dzhdzh", "hezh")
