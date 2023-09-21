# pour nos prochaine expérience on risque d'avoir besoin de regression linéaire
#donc je me suis dit que ce serait sympa de coder notre propre regression linéaire :
#avec la méthode des Moindres Carrés 

import matplotlib.pyplot as plt

class polynome():
    def __init__(self, coefficient) :
        self.coefficient = coefficient
        self.degre = len(self.coefficient)-1
        if self.coefficient ==[] : self.coefdomi = 0
        else : self.coefdomi = coefficient[-1]
    def coef(self,deg):
        if deg > self.degre : return(0)
        return(self.coefficient[deg])
    def __call__(self, valeur) :
        somme=0
        for k in range(self.degre+1) :
            somme+=self.coefficient[k]*valeur**k
        return somme
    def __add__(self,a) :
        if isinstance(a,polynome) :
            liste =[]
            if a.degre <= self.degre :
                for k in range (len(a.coefficient)):
                    liste.append(a.coefficient[k]+self.coefficient[k])
                liste += self.coefficient[len(a.coefficient):]
            else :
                for j in range (len(self.coefficient)):
                    liste.append(a.coefficient[j]+self.coefficient[j])
                liste += a.coefficient[len(self.coefficient):]
            return(polynome(liste))
        elif isinstance(a,float) or isinstance(a,int) :
            self.coefficient[0]+=a
            return(self)
        else:
            raise TypeError("Opération d'addition non prise en charge.")
    def __radd__(self, a):
        return self.__add__(a)
    def __mul__(self, a):
        if isinstance(a,float) or isinstance(a,int):
            liste = [coef * a for coef in self.coefficient]
            return polynome(liste)
        elif isinstance(a,polynome):
            multi = polynome([0])
            for i in range(a.degre + 1):
                liste = [0] * i + [coef * a.coefficient[i] for coef in self.coefficient]
                multi = multi + polynome(liste)
            return multi
        else:
            raise TypeError("Opération de multiplication non prise en charge.")
    def __rmul__(self, a):
        return self.__mul__(a)
    def der(self) :
        return polynome([k*self.coefficient[k] for k in range (1,self.degre+1)])
    def __str__(self):
        s=""
        for k in range (len(self.coefficient)) :
            if not self.coefficient[k] == 0 : s = f" {self.coefficient[k]} x^{k} + " + s
        return s[:-2]

def moyenne(liste) :
    somme = 0
    for k in liste : somme+=k
    return(somme/len(liste))

def variance(liste):
    moy= moyenne(liste)
    somme=0
    for k in liste :somme+=(k-moy)**2
    return(somme/len(liste))
def covariance(liste1,liste2):
    if len(liste1)!=len(liste2) : raise TypeError("liste pas de même longueur")
    else :
        moy1= moyenne(liste1)
        moy2= moyenne(liste2)
        somme=0
        n=len(liste1)
        for k in range (n): somme+=(liste1[k]-moy1)*(liste2[k]-moy2)
        return(somme/n)

def linaire(liste1,liste2) :
    if len(liste1)!=len(liste2) : raise TypeError("liste pas de même longueur")
    else :
        coefficientdeg1 = covariance(liste1,liste2)/variance(liste1)
        coefdeg0 = moyenne(liste2) - coefficientdeg1*moyenne(liste1)
        return(polynome([coefdeg0,coefficientdeg1]))

def qualite(liste1,liste2) :
    return(covariance(liste1,liste2)/(variance(liste1)*variance(liste2)))
