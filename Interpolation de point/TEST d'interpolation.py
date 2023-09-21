import matplotlib.pyplot as plt
import random as rd
from abstract import *

# les fonctions
def ind_degre(rang, degre,intervalle):
    if degre == 0:
        return indicatrice(rang * intervalle, (rang + 1) * intervalle)
    else:
        return (
            (polynome([-rang * intervalle, 1]) * ind_degre(rang, degre - 1,intervalle)
            + polynome([(1 + rang + degre)* intervalle, -1]) * ind_degre(rang + 1, degre - 1,intervalle))
        )
def approximation(liste, degre):
    fonction = Sum_ind_poly([])
    for rang in range(len(liste)):
        L = ind_degre(rang+degre+2, degre) * liste[rang]
        fonction = fonction + L*(1/fact(degre)*intervalle**degre)
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
            for i in range(k+1, n):
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
    print(self*Id)
    return Id
def fact (n):
    if n==1 : return 1
    else : return fact(n-1)*n
def interpolation(liste,degre,intervalle) :
    fonction = Sum_ind_poly([])
    mat1 = matrice( [[(liste[1]-liste[0])/intervalle]] + [[k] for k in liste] + [[(liste[len(liste)-1]-liste[len(liste)-2])/intervalle]])
    mat2 = matrice([[0 for k in range (len(liste)+2)]for k in range(len(liste)+2)])
    liste1 =[]
    m=len(liste)
    for rang in range (m+degre) :
        L = ind_degre(rang,degre,intervalle)
        liste1.append(L)
    for k in range (m) :
        for i in range (m+2) : mat2[k+1,i] = liste1[i]((k+degre)*intervalle**degre)
    liste1der = [der(l) for l in liste1]
    for j in range (m+2) : 
        mat2[0,j] = liste1der[j]((degre+2)*intervalle)
        mat2[-1,j] = liste1der[j]((m+3)*intervalle)
    point_de_controle = résolution(mat2,mat1)
    for k in  range(degre) : fonction = fonction + liste1[k]
    for k in range (m): 
        fonction = fonction + liste1[k+degre]*point_de_controle[k,0]*(1/(fact(degre)*intervalle))
    return fonction

def der(self) :
        if type(self) == ind_poly : return ind_poly(self.indicatrice,der(self.polynome))
        elif type(self) == polynome : return polynome([k*self.coefficient[k] for k in range (1,self.degre+1)])
        elif type(self) == Sum_ind_poly : return Sum_ind_poly([der(k) for k in self.liste])
        else : raise Exception("Pas dérivable")

def résolution(matrice1, coordonée) :
    if matrice1.longueur != coordonée.largeur : raise TypeError("Matrice et coordonée doivent être de même longueur")
    # triangulons la matrice
    for k in range (matrice1.longueur) :
        for j in range (k,matrice1.longueur) :
            if matrice1[j,k] != 0 : 
                matrice1.echange(k,j)
                coordonée.echange(k,j)
        pivot_inv = 1/matrice1[k,k]
        matrice1.coefligne(pivot_inv, k)
        coordonée.coefligne(pivot_inv, k)
        for i in range (k+1,matrice1.longueur) :
            matrice1.liligne(i,k,matrice1[i,k])
            coordonée.liligne(i,k,matrice1[i,k])
    lesresultats = matrice([[0] for k in range (coordonée.largeur)])
    for m in range (coordonée.largeur) :
        resultat = coordonée[coordonée.largeur-m-1,0]
        for l in range (coordonée.largeur-m,coordonée.largeur) :
            resultat -= lesresultats[coordonée.largeur-m-1,0]*matrice1[coordonée.largeur-m-1,coordonée.largeur-l-1]
        resultat = resultat/matrice1[coordonée.largeur-m-1,coordonée.largeur-m-1]
        lesresultats[coordonée.largeur-m-1,0] = resultat
    return(lesresultats)
    # c'est fait
def plot_interpolation_et_point(intervalle,y_interpolation,degre) :
    x_interpolation = [ k for k in range (len(y_interpolation))]
    # Effectuer l'interpolation B-spline
    
    fonction_interpolation = interpolation(y_interpolation, degre,intervalle)

    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((2*degre+k/100)*intervalle) for k in range(len(y_interpolation)*100)]
    for k in range (len(y_interpolation)) : print(fonction_interpolation((5+k)*intervalle))
    # Afficher les points d'interpolation
    plt.scatter(x_interpolation, y_interpolation, color='red', label='Points d\'interpolation')
    # Afficher la courbe interpolée
    t = [k * intervalle * 0.01 for k in range(len(L))]
    plt.plot(t, L, label='Interpolation B-spline')
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()