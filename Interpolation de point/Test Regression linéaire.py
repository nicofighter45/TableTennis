# pour nos prochaine expérience on risque d'avoir besoin de regression linéaire
#donc je me suis dit que ce serait sympa de coder notre propre regression linéaire :
#avec la méthode des Moindres Carrés 
import matplotlib.pyplot as plt
from abstract import *
import random as rd
from incertitudes import *
incertitude_values = (99999999,999999999)

def linaire(liste1,liste2) :
    if len(liste1)!=len(liste2) : raise TypeError("liste pas de même longueur")
    else :
        coefficientdeg1 = covariance(liste1,liste2)/variance(liste1)
        coefdeg0 = moyenne(liste2) - coefficientdeg1*moyenne(liste1)
        return(polynome([coefdeg0,coefficientdeg1]))

def qualite(liste1,liste2) :
    return(covariance(liste1,liste2)/(variance(liste1)*variance(liste2)))

def incertitudes_linéaire(liste1,liste2,U1,U2) :
    liste_a,liste_b,coherence_values=[],[],[]
    for j in range (1000) :
        liste1b,liste2b = [rd.gauss(j,U1) for j in liste1],[rd.gauss(j,U2) for j in liste2]
        poly = linaire(liste1b,liste2b)
        data = qualite(liste1,liste2)
        liste_a.append(poly.coef(1))
        liste_b.append(poly.coef(0))
        coherence_values.append(data)
    print(liste_b,liste_a)
    return((moyenne(liste_b),incertitude_type_A (liste_b)),(moyenne(liste_a),incertitude_type_A (liste_a)),moyenne(coherence_values))

def curv_trace(liste1,liste2,U1,U2) :
    coefb,coefa,coef = incertitudes_linéaire(liste1,liste2,U1,U2)
    poly1 = polynome([coefb[0]+coefb[1],coefa[0]-coefa[1]])
    poly2 = polynome([coefb[0]-coefb[1],coefa[0]+coefa[1]])
    intervalle = (-min(liste1)+max(liste1))/1e6
    t1,t2 = [min(liste1)+k*intervalle for k in range(1000000)],[min(liste1)+k*intervalle for k in range(1000000)]
    x1,x2 = [poly1(k) for k in t1 ],[poly2(k) for k in t2 ]
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(liste1, liste2, color='red')
    ax.errorbar(liste1, liste2, xerr = U1, yerr = U2,  fmt = 'none', capsize = 3, ecolor = 'red')
    ax.plot(t1,x1,color = "green")
    ax.plot(t2,x2,color = "blue")
    plt.show()
liste1=[50,55,60,65,70,75,80,85,90,95,100]
liste2=[25,27.5,30,32.5,35,37.5,40,42.5,45,47.5,50]
curv_trace(liste1,liste2,2,3)


    
    
