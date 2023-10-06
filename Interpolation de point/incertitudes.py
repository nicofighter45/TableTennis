import math as mt
def moyenne(liste) :
    somme = 0
    for k in liste : somme+=k
    return(somme/len(liste))
def ecart_type(liste) :
    moyen = moyenne(liste)
    somme = 0
    for k in liste : somme = (k-moyen)**2
    return (mt.sqrt(somme/(len(liste)-1)))

def variance(liste):
    moy= moyenne(liste)
    somme=0
    for k in liste :somme+=(k-moy)**2
    return(somme/len(liste))

def incertitude_type_A (liste):
    return(ecart_type(liste)/mt.sqrt(len(liste)))

def incertitude_somme(liste) :
    somme=0
    for k in liste : somme += k**2
    return(mt.sqrt(liste))

def incertitude_vitesse_acceleration(liste) :
    return(incertitude_somme(liste)/len(liste))

def incertitude_produit(lists,valeur_theo) :
    somme = 0
    for values in lists :
        somme += (values[1]/values[0])**2
    return(mt.squrt(somme)*valeur_theo)

def covariance(liste1,liste2):
    if len(liste1)!=len(liste2) : raise TypeError("liste pas de même longueur")
    else :
        moy1= moyenne(liste1)
        moy2= moyenne(liste2)
        somme=0
        n=len(liste1)
        for k in range (n): somme+=(liste1[k]-moy1)*(liste2[k]-moy2)
        return(somme/n)
