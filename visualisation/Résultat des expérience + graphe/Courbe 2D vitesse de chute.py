import numpy as np
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
from data_reception import *
from processing import *
from speed import *
Taille_pix, x,h = data("TableTennis/Résultat des expérience + graphe/Rebond table.txt")
rebilitation_de_liste(x)
rebilitation_de_liste_min(h)
convertisseur(x,Taille_pix)
convertisseur(h,Taille_pix)
h=vitesse(h)
x=vitesse(x)
t=[k*10e-3 for k in range(len(h))]
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(2, 1)
h=np.array(h)
max_positions = np.where(x == np.max(x))[0]
min_positions = np.where(x == np.min(x))[0]
ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[1, 0])
fig.subplots_adjust(wspace=0.5,hspace=0.5)
ax1.set_title('Vitesse sur Uy ', loc="center")
ax2.set_title('Vitesse sur Uz ', loc="center")
ax2.plot(t,x,"b-")
ax1.plot(t,h,"g-")
ax2.legend(["h"], loc="upper right")
ax1.legend(["x"], loc="upper right")
ax2.set_xlabel('Temps en s')
ax2.set_ylabel('Vz en m.s-1')
ax1.set_xlabel('Temps en s')
ax1.set_ylabel('Vy en m.s-1')
for pos in max_positions:
    plt.scatter(t[pos], x[pos], color='red', label='Maximums')
    plt.text(t[pos], x[pos], f"Max: {x[pos]:.2f}", ha='center', va='bottom')

for pos in min_positions:
    plt.scatter(t[pos], x[pos], color='blue', label='Minimums')
    plt.text(t[pos], x[pos], f"Min: {x[pos]:.2f}", ha='center', va='top')
plt.show()
plt.clf()
