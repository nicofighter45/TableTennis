import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt

from data_reception import *
from processing import *

Taille_pix, h, x = data("TableTennis/Résultat des expérience + graphe/test1-tracked.txt")
rebilitation_de_liste(h)
convertisseur(h, Taille_pix)
convertisseur(x, Taille_pix)

t = [k * 10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(1, 1)
ax2 = fig.add_subplot(gs[0, 0])
ax2.set_title('Courbe du rebond : f(y) = z ', loc="center")
ax2.plot(x, h, "b-")
ax2.set_xlabel('Abscisse en m')
ax2.set_ylabel('Hauteur en m')
plt.show()
plt.clf()
