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

def vitesse(position):
    vitesse = []
    p = 1
    for k in range(len(position) - 1):
        vitesse.append((position[k + 1] - position[k]) / (1e-2 * p))
    return vitesse

def premièrevaleurhaute(liste):
    p = 0
    for k in range(len(liste)//2):
        if liste[k] <= liste[k + 1]:
            p = k
    return liste[p + 1:]

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
        if liste[k-1] <= liste[k]>= liste[k+1] :
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

def bond_division(liste):
    subdivision=[]
    min = 0
    for k in range (1,len(liste)-1) :
        if liste[k] < liste[k+1] and liste[k] < liste[k-1] :
            subdivision.append([min,k])
            min = k+1
    return(subdivision)

def premier_rebond(t):
    fin = -1
    v = vitesse(t)
    for i in range(1, len(t)):
        if v[i - 1] > 0 and v[i] < 0:
            fin = i
    if fin == -1 : raise TypeError("Pas de rebond")
    else : return t[0:fin]

def haut_du_bond(liste):
    liste1=[]
    sub = bond_division(liste)
    for k in sub :
        (max,indice) = maxindice(liste[k[0]:k[1]])
        liste1.append((max,indice+k[0]))
    return(liste1)
