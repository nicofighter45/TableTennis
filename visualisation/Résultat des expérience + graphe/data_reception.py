def data(texte):
    with open(texte, "r") as file:
        h, x = [], []
        lines = file.readlines()
        for line in lines:
            cut = line[:-2].split(";")
            h.append(float(cut[0]))
            x.append(float(cut[1]))
    Taille_pix, h, x = h[0] / x[0], h[1:], x[1:]
    return (Taille_pix, h, x)


def premièrevaleurhaute(liste):
    p = 0
    for k in range(40):
        if liste[k] <= liste[k + 1]:
            p = k
    return (liste[p + 1:])


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
            if k + 1 < len(liste2): max.append(liste2[k])
            p = -1
            k += 1
    return (max)
