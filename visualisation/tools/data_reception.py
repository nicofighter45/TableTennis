import collections as c
Taille_pix =  137.1e-2/911
def data(texte):
    with open(texte, "r") as file:
        h, x = [], []
        lines = file.readlines()
        for line in lines:
            cut = line[:-2].split(";")
            h.append(float(cut[0]))
            x.append(float(cut[1]))
    h, x = h[1:], x[1:]
    return Taille_pix, h, x

def ajouter_ligne_fichier(nom_fichier, ligne):
    try:
        with open(nom_fichier, 'a') as fichier:
            fichier.write(ligne + '\n')
        print("La ligne a été ajoutée avec succès au fichier", nom_fichier)
    except IOError:
        print("Erreur : Impossible d'ouvrir ou d'écrire dans le fichier", nom_fichier)

def result(nom_fichier,experience, moy , incertitude, maximu, minimum, nombre_de_coefficient) :
    ajouter_ligne_fichier(nom_fichier,str(experience))
    ajouter_ligne_fichier(nom_fichier,"Le coefficient de restitution est de "+ str(moy))
    ajouter_ligne_fichier(nom_fichier,"L'incertitude sur la valeur est de "+ str(incertitude))
    ajouter_ligne_fichier(nom_fichier,"Le maximum est le minimum sont " + str(maximu) +" et "+ str(minimum))
    ajouter_ligne_fichier(nom_fichier,"Avec en tout " + str(nombre_de_coefficient) +" valeurs déterminées")

def vitesse(position):
    vitesse = []
    for k in range(len(position) - 1):
        vitesse.append((position[k + 1] - position[k]) / (1e-2))
    return vitesse

def premièrevaleurhaute(liste):
    p = 0
    for k in range(len(liste)//2):
        if liste[k] <= liste[k + 1]:
            p = k
    return liste[p + 1:]

def rebond(liste):
    v = vitesse(liste)
    liste_rebond = [0]
    for k in range (len(v)-1) :
        if v[k] <= 0 and v[k+1] >=0 :
            liste_rebond.append(k+1)
    return liste_rebond

def maxindice(x):
    if len(x) == 0  : return (x)
    if len(x) == 1  : return (x[0],0)
    max,indice = x[0],0
    for l in range (len(x)):
        if max < x[l]: max,indice = x[l],l
    return (max,indice)


def maximum(liste):
    liste2 = premièrevaleurhaute(liste)
    max = [liste2[0]]
    p = -1
    k = 0
    while k < len(liste2):
        if p == -1:
            while k + 1 < len(liste2) and liste2[k] >= liste2[k + 1]:
                k += 1
            p = 1
        if p == 1:
            while k + 1 < len(liste2) and liste2[k] <= liste2[k + 1]:
                k += 1
            if k + 1 < len(liste2):
                max.append(liste2[k])
            p = -1
            k += 1
    return max

def maxsubdivition(liste,i,j) :
    liste2 = liste[i:j]
    n = maxindice(liste2)
    if n != [] : return n[1] + i
    else :return(i)

def localmax(liste) :
    listmax =[(liste[0],0)]
    for k in range (1,len(liste)-1) :
        if liste[k-1] <= liste[k] >= liste[k+1] :
            listmax.append((liste[k],k))
    return(listmax)

def localmaxexperimentale(liste) :
    max = localmax(liste)
    if len(max) ==1 : return([])
    max1= [max[k][0] for k in range (len(max))]
    n = len(max)
    listesubdivision = c.deque([(0,0)])
    maxi = max1[0]
    for k in range (n) :
        if abs(maxi-max1[k]) > maxi*0.25 :
            listesubdivision.append((listesubdivision[-1][1]+1,k-1))
            maxi = max1[k]
    listesubdivision.append((listesubdivision[-1][1]+1,n-1))
    listesubdivision.popleft()
    indice = [ maxsubdivition(max1,k[0],k[1]) for k in listesubdivision]
    return([max[k] for k in indice])

def premier_rebond(t):
    fin = -1
    v = vitesse(t)
    for i in range(1, len(t)):
        if v[i - 1] > 0 and v[i] < 0:
            fin = i
    if fin == -1 : raise TypeError("Pas de rebond")
    else : return t[0:fin]

def coherence(liste) :
    k = 0
    while k < len(liste) :
        if liste[k] > 3 : liste.pop(k)
        k+=1
    return(liste)

def lissage(liste,precision) :
    for k in range (2,len(liste)):
        if abs(liste[k-1]) > 10e-3 and abs(liste[k]/liste[k-1]-1) > precision :
            liste[k]=liste[k-1]

def partition(liste) :
    partition = rebond(liste)
    bond = []
    for k in range(1,len(partition)) :
        bond.append(liste[partition[k-1]:partition[k]])
    return(bond)

def maxlocal(liste) :
    reb = rebond(liste)
    parti = partition(liste)
    print(len(reb)==len(parti))
    liste = []
    for k in range(len(parti)) :
        max1,max2 = maxindice(parti[k])
        liste.append(rebond)

def fichier(nom, liste2, liste3):
    # Vérifier que les deux listes ont la même longueur
    if len(liste2) != len(liste3):
        print("Erreur : Les deux listes doivent avoir la même longueur.")
        return

    try:
        # Ouvrir (ou créer) le fichier en mode écriture
        with open(nom, 'w', encoding='utf-8') as fichier:
            for k in range(len(liste2)):
                fichier.write(f"{liste2[k]};{liste3[k]}\n")
        print(f"Le fichier '{nom}' a été créé avec succès.")
    except IOError as e:
        print(f"Erreur : Impossible de créer ou d'écrire dans le fichier '{nom}'. Détails de l'erreur : {e}")

# Exemple d'utilisation :
nom_fichier = "mon_fichier.txt"
liste2 = ["element1_liste2", "element2_liste2", "element3_liste2"]
liste3 = ["element1_liste3", "element2_liste3", "element3_liste3"]
fichier(nom_fichier, liste2, liste3)

    