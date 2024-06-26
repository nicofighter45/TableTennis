import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

Taille_pix, x, h = data("TableTennis/calcul/Courbe du robot.txt")
rebilitation_de_liste(x)
rebilitation_de_liste_min(h)
convertisseur(x, Taille_pix)
convertisseur(h, Taille_pix)
h = vitesse(vitesse(h))
x = vitesse(vitesse(x))
t = [k * 10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(2, 1)

ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[1, 0])
fig.subplots_adjust(wspace=0.5, hspace=0.5)
ax1.set_title('acceleration sur Uy ', loc="center")
ax2.set_title('acceleration sur Uz ', loc="center")
ax2.plot(t, h, "b-")
ax1.plot(t, x, "g-")
ax2.legend(["Az"], loc="upper right")
ax1.legend(["Ay"], loc="upper right")
ax2.set_xlabel('Temps')
ax2.set_ylabel('Vz')
ax1.set_xlabel('Temps')
ax1.set_ylabel('Vy')
plt.show()
plt.clf()
