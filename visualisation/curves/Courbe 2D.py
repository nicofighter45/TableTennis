import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

Taille_pix, h, x = data("TableTennis/output/4-jets-de-balle/tracked-4.txt")
rebilitation_de_liste_max(x)
rebilitation_de_liste_min(h)
x=premièrevaleurhaute(x,1)
x=premier_rebond(x)
convertisseur(x, Taille_pix)
convertisseur(h, Taille_pix)
t1 = [k * 10e-3 for k in range(len(h))]
t2 = [k * 10e-3 for k in range(len(x))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(2, 1)
ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[1, 0])
fig.subplots_adjust(wspace=0.5, hspace=0.5)
ax1.plot(t2, x, "b-")
ax2.plot(t1,h, "g-")
ax1.legend(["x"], loc="upper right")
ax2.legend(["h"], loc="upper right")
ax1.set_title('Position sur Uy ', loc="center")
ax2.set_title('Position sur Uz ', loc="center")
ax1.set_xlabel('Temps en s')
ax1.set_ylabel('Abscisse en m')
ax2.set_xlabel('Temps en s')
ax2.set_ylabel('Ordonnée en m')
plt.show()
plt.clf()
