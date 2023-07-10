import math as mt
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
import numpy as np
 
m=7e-3
rho=1.293
S=8/3*mt.pi*(4e-2)**2
g=9.81
h,x=[],[]
with open("Résultat des expérience + graphe/Rebond sur sol.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        cut = line[:-2].split(";")
        h.append(float(cut[0]))
        x.append(float(cut[1]))
Taille_pix, h,x= h[0]/x[0],h[1:],x[1:]
def vitesse(position) :
    vitesse=[]
    p=1
    for k in range (len(position)-1) :
        vitesse.append((position[k+1]-position[k])/(10e-3*p))
    return(vitesse)
def energie_potentielle(position) :
    liste = []
    for k in position :
        liste.append(m*g*k)
    return(liste)
def energie_cinétique(position) :
    liste = []
    for k in vitesse(position) :
        liste.append(1/2*m*k**2)
    return(liste)
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
def rebilitation_de_liste(liste) :
    maxi =max(liste)
    for k in range(len(liste)) :
        liste[k]=abs(maxi-liste[k])
def convertisseur(liste):
    for k in range (len(liste)):
        liste[k] = liste[k]*Taille_pix
def premièrevaleurhaute(liste) :
    p=0
    for k in range(17): 
        if liste[k]<=liste[k+1]:
            p=k
    return(liste[p+1:])
def descente(liste) :
    p=0
    while liste[p]>=liste[p+1] :
        p+=1
    return(liste[:p+1])
def courbe(h):
    rebilitation_de_liste(h)
    h=premièrevaleurhaute(h)
    convertisseur(h)
    t=[k*10e-3 for k in range(len(h))]
    potentiel = np.array(energie_potentielle(h))
    cinetique = np.array([0]+energie_cinétique(h))
    Mecanique= potentiel +cinetique
    plt.plot(t,potentiel,label ="Énergie potentiel")
    plt.plot(t,cinetique,label ="Énergie cinétique")
    plt.plot(t,Mecanique,label="Énergie mécanique")
    plt.title("Évolution de l'énergie au cours du temps")
    plt.ylabel( "Énergie en Joule")
    plt.xlabel( "Temps en seconde")
    plt.legend()
    plt.show()
    plt.clf()
courbe(h)