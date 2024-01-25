def max(x):
    max = x[0]
    for l in x:
        if max < l: max = l
    return (max)


def min(x):
    min = x[0]
    for l in x:
        if min > l: min = l
    return (min)


def rebilitation_de_liste_min(liste):
    mini = min(liste)
    for k in range(len(liste)):
        liste[k] = abs(mini - liste[k])


def rebilitation_de_liste(liste):
    maxi = max(liste)
    for k in range(len(liste)):
        liste[k] = abs(maxi - liste[k])


def convertisseur(liste, Taille_pix):
    for k in range(len(liste)):
        liste[k] = liste[k] * Taille_pix


def premièrevaleurhaute(t, epsilon):
    for i in range(1, len(t)):
        if abs(t[i] - t[i-1]) > epsilon:
            return t[i:]


def descente(liste):
    p = 0
    while p + 1 < len(liste) and liste[p] >= liste[p + 1]:
        p += 1
    return (liste[:p + 1].copy())


def monte(liste):
    p = 0
    while p + 1 < len(liste) and liste[p] <= liste[p + 1]:
        p += 1
    return (liste[:p + 1])


def moyenne(liste):
    somme = 0
    for k in liste:
        somme += k
    return (somme / len(liste))


def rebilitation_de_liste_max(liste):
    maxi = max(liste)
    for k in range(len(liste)):
        liste[k] = abs(maxi - liste[k])

def Bond(liste) :
    liste = descente(liste)
    
