import math as mt
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec
m=7e-3
rho=1.293
S=8/3*mt.pi*(4e-2)**2
g=9.81
h,x=[],[]
with open("/Users/antoine/TIPE/TableTennis/Résultat des expérience + graphe/Rebond table.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        cut = line[:-2].split(";")
        h.append(float(cut[0]))
        x.append(float(cut[1]))
Taille_pix, h= h[0]/x[0],h[1:]
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
def moyenne(liste) :
    somme=0
    for k in liste :
        somme+=k
    return(somme/len(liste))
def vitesse(position) :
    vitesse=[]
    p=1
    for k in range (len(position)-1) :
        if abs(position[k+1]-position[k])< 10e-7 : p+=1
        else : vitesse.append((position[k+1]-position[k])/(10e-3*p))
    return(vitesse)
def pente(h):
    v=vitesse(h)
    pente=[]
    for k in range (len(v)) :
        for j in range (k,len(v)):
            if j!= k : pente.append((v[k]-v[j])/(10e-3*(k-j)))
        return(moyenne(pente))
rebilitation_de_liste(h)
h=premièrevaleurhaute(h)
h=descente(h)[:-1]
print(pente(h))