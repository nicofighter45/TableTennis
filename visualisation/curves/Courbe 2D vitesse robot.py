import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt

from visualisation.tools.calculus import *
from visualisation.tools.data_reception import *
from visualisation.tools.processing import *

Taille_pix, x, h = data("TableTennis/calcul/Courbe du robot.txt")
rebilitation_de_liste(h)
convertisseur(x, Taille_pix)
convertisseur(h, Taille_pix)
h = vitesse(h)
x = vitesse(x)

t = [k * 10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(2, 1)
ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[1, 0])
fig.subplots_adjust(wspace=0.5, hspace=0.5)
ax1.set_title('Vitesse sur Uz ', loc="center")
ax2.set_title('Vitesse sur Uy ', loc="center")
ax2.plot(t, x, "b-")
ax1.plot(t, h, "g-")
ax2.legend(["y"], loc="upper right")
ax1.legend(["h"], loc="upper right")
ax2.set_xlabel('Temps')
ax2.set_ylabel('Vy')
ax1.set_xlabel('Temps')
ax1.set_ylabel('Vz')
plt.show()
plt.clf()
