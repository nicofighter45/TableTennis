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
def convertisseur(liste,Taille_pix):
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