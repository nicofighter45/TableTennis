import math as mt
m=1
rho=1
S=1
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
    while liste[p]<liste[p+1]:
        p+=1
    liste[p:]
def descente(liste) :
    p=0
    while liste[p]>liste[p+1] :
        p+=1
    liste=liste[p:]
rebilitation_de_liste(h)
convertisseur(h)
descente(h)
premièrevaleurhaute(h)
def moyenne(liste) :
    somme=0
    for k in liste :
        somme+=k
    return(somme/len(liste))
def vitesse(position) :
    vitesse=[]
    for k in range (len(position)-1) :
        vitesse.append(position[k+1]/10e-3-position[k]/10e-3)
    return(vitesse)
def coefficient_de_frottement(position):
    vitesse2 = [0] + vitesse(position)
    acceleration = vitesse(vitesse2)
    coefficient =[]
    for k in range (len(acceleration)) :
        if vitesse2[k+1] !=0 :
            C_x=(m*acceleration[k]-m*g)/(0.5*rho*S*vitesse2[k+1]**2)
            coefficient.append(abs(C_x))
    return(moyenne(coefficient))
print(coefficient_de_frottement(h))