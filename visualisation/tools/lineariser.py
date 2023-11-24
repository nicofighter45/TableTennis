h, x = [], []
with open("calcul/Rebond sur sol.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        cut = line[:-2].split(";")
        h.append(float(cut[0]))
        x.append(float(cut[1]))


def linéarisation_de_liste(liste):
    newliste = []
    for k in range(0, len(liste) - 1):
        newliste.append(liste[k])
        newliste.append((liste[k] + liste[k + 1] / 2))
    newliste.append(liste[-1])
    return (newliste)


def linéarisation15(h):
    for k in range(10):
        h = linéarisation_de_liste(h)
    return (h)
