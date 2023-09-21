import numpy as np
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
import data_reception as dt
import processing as p
import speed as sp
h,x=[],[]
with open("/Users/antoine/TIPE/TableTennis/Résultat des expérience + graphe/Rebond Table.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        cut = line[:-2].split(";")
        x.append(float(cut[0]))
        h.append(float(cut[1]))
Taille_pix, h, x = x[0]/h[0],h[1:],x[1:]
def max(x) :
    max = x[0]
    for l in x :
        if max < l : max =l
    return(max)
def min(x) :
    min = x[0]
    for l in x :
        if min > l : min =l
    return(min)
def convertisseur(liste):
    for k in range (len(liste)):
        liste[k] = liste[k]*Taille_pix
def rebilitation_de_liste(liste) :
    maxi =max(liste)
    for k in range(len(liste)) :
        liste[k]=abs(maxi-liste[k])
def vitesse(position) :
    vitesse=[]
    p=1
    for k in range (len(position)-1) :
        vitesse.append((position[k+1]-position[k])/(10e-3*p))
    return(vitesse)
def rebilitation_de_liste_min(liste) :
    mini =min(liste)
    for k in range(len(liste)) :
        liste[k]=abs(mini-liste[k])
rebilitation_de_liste(x)
rebilitation_de_liste_min(h)
convertisseur(x)
convertisseur(h)
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
