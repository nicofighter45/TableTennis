import matplotlib.pyplot as plt
import sys
sys.path.insert(0, 'TableTennis')
from visualisation.tools.abstract import *

intervalle = 1

def integrale(fonction,t1,t2) :
    primi = fonction.primitive()
    return(primi(t2)-primi(t1))

# les fonctions
def ind_degre(rang, degre, intervalle):
    if degre == 0:
        return indicatrice(rang * intervalle, (rang + 1) * intervalle)
    else:
        return (
            (polynome([-rang * intervalle, 1]) * ind_degre(rang, degre - 1, intervalle)
             + polynome([(1 + rang + degre) * intervalle, -1]) * ind_degre(rang + 1, degre - 1, intervalle))
        )


def approximation(liste, degre):
    fonction = Sum_ind_poly([])
    for rang in range(len(liste)):
        L = ind_degre(rang + degre + 2, degre, intervalle) * liste[rang]
        fonction = fonction + L * (1 / fact(degre) * intervalle ** degre)
    return fonction


def inverse(self):
    if self.largeur != self.longueur:
        raise ValueError("La matrice doit être carrée pour être inversible.")
    n = self.largeur
    Id = matrice([[1 if i == j else 0 for j in range(n)] for i in range(n)])  # Copie de la matrice identité
    matrice_temp = matrice([ligne[:] for ligne in self.donnee])

    for k in range(n):
        if matrice_temp[k, k] == 0:
            # Recherche d'une ligne non nulle pour effectuer un échange
            for i in range(k + 1, n):
                if matrice_temp[i, k] != 0:
                    # Échange de la ligne k avec la ligne i
                    matrice_temp.donnee[k], matrice_temp.donnee[i] = matrice_temp.donnee[i], matrice_temp.donnee[k]
                    Id.donnee[k], Id.donnee[i] = Id.donnee[i], Id.donnee[k]
                    break
            else:
                raise ValueError("La matrice est singulière et n'est pas inversible.")

        pivot_inv = 1 / matrice_temp[k, k]
        matrice_temp.coefligne(pivot_inv, k)
        Id.coefligne(pivot_inv, k)

        for j in range(n):
            if j != k:
                alpha = matrice_temp[j, k]
                matrice_temp.liligne(j, k, alpha)
                Id.liligne(j, k, alpha)
    return Id


def fact(n):
    if n == 1:
        return 1
    else:
        return fact(n - 1) * n


def interpolation(liste, degre):
    m = len(liste)
    fonction = Sum_ind_poly([])
    mat1 = matrice([[(liste[1] - liste[0]) / intervalle]] + [[k] for k in liste] + [
        [(liste[len(liste) - 1] - liste[len(liste) - 2]) / intervalle]])
    mat2 = matrice([[0 for k in range(m + 2)] for k in range(m + 2)])
    liste1 = []
    for rang in range(m + degre):
        L = ind_degre(rang, degre, intervalle)
        liste1.append(L)
    for k in range(m):
        for i in range(m + 2): mat2[k + 1, i] = liste1[i]((k + degre) * intervalle ** degre)
    liste1der = [der(l) for l in liste1]
    for j in range(m + 2):
        mat2[0, j] = liste1der[j]((degre + 2) * intervalle)
        mat2[-1, j] = liste1der[j]((m + 4) * intervalle)
    point_de_controle = résolution(mat2, mat1)
    for k in range(degre): fonction = fonction + liste1[k]
    for k in range(m):
        fonction = fonction + liste1[k + degre] * point_de_controle[k, 0] * (1 / (fact(degre) * intervalle))
    return fonction


def der(self):
    if type(self) == ind_poly:
        return ind_poly(self.indicatrice, der(self.polynome))
    elif type(self) == polynome:
        return 100 * polynome([k * self.coefficient[k] for k in range(1, self.degre + 1)])
    elif type(self) == Sum_ind_poly:
        return Sum_ind_poly([der(k) for k in self.liste])
    else:
        raise Exception("Pas dérivable")


def résolution(matrice1, coordonée):
    if matrice1.longueur != coordonée.largeur: raise TypeError("Matrice et coordonée doivent être de même longueur")
    # triangulons la matrice
    for k in range(matrice1.longueur):
        for j in range(k, matrice1.longueur):
            if matrice1[j, k] != 0:
                matrice1.echange(k, j)
                coordonée.echange(k, j)
        pivot_inv = 1 / matrice1[k, k]
        matrice1.coefligne(pivot_inv, k)
        coordonée.coefligne(pivot_inv, k)
        for i in range(k + 1, matrice1.longueur):
            matrice1.liligne(i, k, matrice1[i, k])
            coordonée.liligne(i, k, matrice1[i, k])
    lesresultats = matrice([[0] for k in range(coordonée.largeur)])
    for m in range(coordonée.largeur):
        resultat = coordonée[coordonée.largeur - m - 1, 0]
        for l in range(coordonée.largeur - m, coordonée.largeur):
            resultat -= lesresultats[coordonée.largeur - m - 1, 0] * matrice1[
                coordonée.largeur - m - 1, coordonée.largeur - l - 1]
        resultat = resultat / matrice1[coordonée.largeur - m - 1, coordonée.largeur - m - 1]
        lesresultats[coordonée.largeur - m - 1, 0] = resultat
    return (lesresultats)
    # c'est fait


def plot_interpolation_et_point(y_interpolation, degre):
    x_interpolation = [k * intervalle for k in range(len(y_interpolation))]
    # Effectuer l'interpolation B-spline

    fonction_interpolation = interpolation(y_interpolation, degre)

    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((2 * degre + k / 100) * intervalle) for k in range(len(y_interpolation) * 100)]
    # Afficher les points d'interpolation
    plt.scatter(x_interpolation, y_interpolation, color='red', label='Points d\'interpolation')
    # Afficher la courbe interpolée
    t = [k * intervalle * 0.01 for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('temps gradué tous les 10ms')
    plt.ylabel('hauteur de la balle')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()
    plt.clf()


def plot_approximation_et_point(y_interpolation, degre):
    x_interpolation = [k * intervalle for k in range(len(y_interpolation))]
    # Effectuer l'approxilation B-spline

    fonction_interpolation = approximation(y_interpolation, degre)
    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((3 * degre + k / 100) * intervalle) for k in range((len(y_interpolation) - 4) * 100)]
    # Afficher les points d'interpolation
    plt.scatter(x_interpolation, y_interpolation, color='red', label='Points d\'interpolation')
    # Afficher la courbe interpolée
    t = [k * intervalle * 0.01 for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('temps gradué tous les 10ms')
    plt.ylabel('hauteur de la balle')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()
    plt.clf()

def plot_vitesse_et_point1(y_interpolation, degre):
    x_interpolation = [k * intervalle for k in range(len(y_interpolation) - 1)]
    v_point = [(y_interpolation[k + 1] - y_interpolation[k]) / 10e-3 for k in range(len(y_interpolation) - 1)]
    # Effectuer l'interpolation B-spline

    fonction_interpolation = interpolation(y_interpolation, degre)
    fonction_interpolation = der(fonction_interpolation)
    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((2 * degre + k / 100) * intervalle) for k in
         range((len(y_interpolation) - degre * 2) * 100)]
    # Afficher les points d'interpolation
    plt.scatter(x_interpolation, v_point, color='red', label='Points d\'interpolation')
    # Afficher la courbe interpolée
    t = [k * intervalle * 0.01 for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('temps gradué tous les 10ms')
    plt.ylabel('vitesse de la balle sur Uz')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()
    plt.clf()


def plot_vitesse_et_point2(y_interpolation, degre):
    x_interpolation = [k * intervalle for k in range(len(y_interpolation) - 1)]
    v_point = [(y_interpolation[k + 1] - y_interpolation[k]) / 10e-3 for k in range(len(y_interpolation) - 1)]
    # Effectuer l'interpolation B-spline

    fonction_interpolation = approximation(y_interpolation, degre)
    fonction_interpolation = der(fonction_interpolation)
    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((3 * degre + k / 100) * intervalle) for k in
         range((len(y_interpolation) - degre * 2) * 100)]
    # Afficher les points d'interpolation
    plt.scatter(x_interpolation, v_point, color='red', label='Points d\'interpolation')
    # Afficher la courbe interpolée
    t = [k * intervalle * 0.01 for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('temps gradué tous les 10ms')
    plt.ylabel('vitesse de la balle sur Uz')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()
    plt.clf()

def Lagrange_construction(n,i) :
    poly = polynome([1])
    for k in range (i) :
        poly *= polynome([-k/(i-k),1/(i-k)])
    for k in range(i+1,n) :
        poly *= polynome([-k/(i-k),1/(i-k)])
    return(poly)

    
def Lagrange(liste) :
    n = len(liste)
    poly = polynome([1])
    for k in range (n) :
        poly += liste[k]*Lagrange_construction(n,k)
    return(poly+polynome([-1]))

def plot_Lagrange(y_interpolation):
    # Effectuer l'interpolation B-spline

    fonction_interpolation = Lagrange(y_interpolation)
    # Calculer les valeurs interpolées pour l'affichage
    L = [ fonction_interpolation(k) for k in range(len(y_interpolation))]
    # Afficher la courbe interpolée
    t = [k for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Interpolation Lagrange de degré')
    plt.show()
    plt.clf()