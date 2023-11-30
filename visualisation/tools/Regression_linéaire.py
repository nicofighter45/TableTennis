# pour nos prochaine expérience on risque d'avoir besoin de regression linéaire
# donc je me suis dit que ce serait sympa de coder notre propre regression linéaire :
# avec la méthode des Moindres Carrés
import random as rd

import matplotlib.pyplot as plt

from visualisation.tools.abstract import *
from visualisation.tools.incertitudes import *

incertitude_values = (99999999, 999999999)


def linaire(liste1, liste2):
    if len(liste1) != len(liste2):
        raise TypeError("liste pas de même longueur")
    else:
        coefficientdeg1 = covariance(liste1, liste2) / variance(liste1)
        coefdeg0 = moyenne(liste2) - coefficientdeg1 * moyenne(liste1)
        return (polynome([coefdeg0, coefficientdeg1]))


def qualite(liste1, liste2):
    return (covariance(liste1, liste2) / mt.sqrt((variance(liste1) * variance(liste2))))


def incertitudes_linéaire(liste1, liste2, U1, U2):
    liste_a, liste_b, coherence_values = [], [], []
    for k in range(10000):
        liste1b, liste2b = [rd.gauss(j, U1) for j in liste1], [rd.gauss(j, U2) for j in liste2]
        poly = linaire(liste1b, liste2b)
        data = qualite(liste1, liste2)
        liste_a.append(poly.coef(1))
        liste_b.append(poly.coef(0))
        coherence_values.append(data)
    return ((moyenne(liste_b), ecart_type(liste_b)), (moyenne(liste_a), ecart_type(liste_a)), moyenne(coherence_values))


def curv_trace(liste1, liste2, U1, U2, nom_graphe, y_label, x_label):
    coefb, coefa, coef = incertitudes_linéaire(liste1, liste2, U1, U2)
    poly1 = polynome([coefb[0] + coefb[1], coefa[0] - coefa[1]])
    poly2 = polynome([coefb[0] - coefb[1], coefa[0] + coefa[1]])
    intervalle = (-min(liste1) + max(liste1)) / 1e6
    t1 = [min(liste1) + k * intervalle for k in range(int(1e6))]
    x1, x2 = [poly1(k) for k in t1], [poly2(k) for k in t1]
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(liste1, liste2, color='red')
    ax.errorbar(liste1, liste2, xerr=U1, yerr=U2, fmt='none', capsize=3, ecolor='red')
    ax.plot(t1, x2, color="blue")
    ax.plot(t1, x1, color="orange")
    textstr = '\n'.join((
        "coef de cohérence : " + str(round(coef, 3)),
        "pente : " + str(round(coefa[0], 3)) + " ± " + str(round(coefa[1], 3)),
        "Ordonnée à l'origine : " + str(round(coefb[0], 3)) + " ± " + str(round(coefb[1], 3))))
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
    ax.text(0.05, 0.95, textstr, transform=ax.transAxes, fontsize=10,
            verticalalignment='top', bbox=props)
    fig.suptitle(nom_graphe)
    ax.set_ylabel(y_label)
    ax.set_xlabel(x_label)
    plt.legend()
    plt.show()


liste1 = [1, 2, 3, 4, 5]
liste2 = [3.1, 4.9, 7.1, 8.8, 11.3]
