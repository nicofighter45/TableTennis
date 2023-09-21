import math as mt
def moyenne(liste) :
    somme = 0
    for k in liste : somme+=k
    return(somme)
def ecart_type(liste) :
    moyen = moyenne(liste)
    somme = 0
    for k in liste : somme = (k-moyen)**2
    return (mt.squrt(somme/(len(liste)-1)))

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
