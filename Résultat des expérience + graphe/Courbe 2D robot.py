import numpy as np
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
from data_reception import *
from processing import *
from speed import *
Taille_pix, x,h = data("TableTennis/Résultat des expérience + graphe/Courbe du robot.txt")
rebilitation_de_liste(h)
convertisseur(h,Taille_pix)
convertisseur(x,Taille_pix)

t=[k*10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(1, 1)
ax2 = fig.add_subplot(gs[0, 0])
ax2.set_title('Courbe du rebond : f(y) = z ', loc="center")
ax2.plot(x,h,"b-")
ax2.set_xlabel('Abscisse en m')
ax2.set_ylabel('Hauteur en m')
ax2.set_ylim(-0.1,0.4)
plt.show()
plt.clf()
