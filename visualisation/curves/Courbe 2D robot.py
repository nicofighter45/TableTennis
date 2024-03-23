import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

Taille_pix, x, h = data("TableTennis/output/1-robotv1/tracked-2.txt")
rebilitation_de_liste(x)
convertisseur(h, Taille_pix)
convertisseur(x, Taille_pix)

t = [k * 10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(1, 1)
ax2 = fig.add_subplot(gs[0, 0])
ax2.set_title('Courbe du rebond : f(y) = z ', loc="center")
ax2.plot(h, x, "b-")
ax2.set_xlabel('Abscisse en m')
ax2.set_ylabel('Hauteur en m')
plt.show()
plt.clf()
