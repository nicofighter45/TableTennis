import matplotlib.pyplot as plt
import random as rd
#les classes
class indicatrice() :
    def __init__(self, t1, t2):
        self.intervalle = (t1,t2)
    def __call__(self, point):
        if self.intervalle[0] <= point < self.intervalle[1] :
            return(1)
        else : return(0)
    def __mul__(self,a):
        if isinstance(a,polynome) :
            return(ind_poly(self,a))
        else : raise TypeError("Opération de multiplication non prise en charge 1.")
    def __rmul__(self,a):
        return self.__mul__(a)
    def __str__(self):
        return(f" Id{self.intervalle} ")
    def __eq__(self,a) :
        return self.intervalle == a.intervalle
class matrice() :
    def __init__(self,listedeliste) :
        self.donnee = listedeliste
        self.longueur = len(listedeliste[0])
        self.largeur = len(listedeliste)
    def __getitem__(self, indices):
        i, j = indices
        return self.donnees[i][j]
    def __setitem__(self, indices, valeur):
        i, j = indices
        self.donnees[i][j] = valeur
    def __repr__(self):
        return repr(self.donnees)
    def __add__(A,B):
        if A.longueur == B.longueur and A.largeur == B.largeur :
            return(matrice([[A[i][j] + B[i][j] for j in range (len(A[i]))] for i in range (len(A))]))
        else : return(False)
    def __mul__(A,B) :
        if isinstance(B,matrice) :
            n, m, l = A.largeur, A.longueur, B.longueur
            if m != B.largeur :
                raise Exception("The number of column of M1 must be the same as the number of line of M2")
            M = []
            for i in range(n):
                line = []
                for j in range(l):
                    sum = 0
                    for k in range(m):
                        sum += A[i][k]*B[k][j]
                    line.append(sum)
                M.append(line)
            return matrice(M)
        if isinstance(B,int) :
            for i in range (A.largeur) :
                for j in range (A.longueur) :
                    A[i][j]=A[i][j]*B
            return(A)
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
        elif isinstance(a,indicatrice) :
            return(ind_poly(a,self))
        elif isinstance(a,ind_poly) :
            a.polynome = a.polynome * self
            return(a)
        elif isinstance(a,Sum_ind_poly) :
            for k in a.liste : 
                k = self*k
            return(a)
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
class ind_poly():
    def __init__(self,indicatrice,polynome) :
        self.indicatrice = indicatrice
        self.polynome = polynome
    def __call__(self, point) :
        if self.indicatrice(point)==0 : return 0
        else : return self.polynome(point)
    def __add__(self,a):
        if isinstance(a,float) or isinstance(a,int) or isinstance(a,polynome) :
            poly = self.polynome + a
            return ind_poly(self.indicatrice,poly)
        elif isinstance(a,ind_poly) :
            if self.indicatrice==a.indicatrice : 
                poly = self.polynome + a.polynome
                return ind_poly(self.indicatrice,poly)
            else : 
                Somme =Sum_ind_poly([self,a])
                return(Somme)
        elif isinstance(a,Sum_ind_poly) :
            check = True
            for k in range (len(a.liste)) :
                if a.ind[k]==self.indicatrice : 
                    a.liste[k]= a.liste[k]+ self
                    check = False
            if check :
                a.liste.append(self)
                a.polynome.append(self.polynome)
                a.ind.append(self.indicatrice)
            return(a)
        else : raise TypeError("Opération d'addition non prise en charge.")
    def __radd__(self,a):
        return self.__add__(a)
    def __mul__(self,a) :
            if isinstance(a,float) or isinstance(a,int) or isinstance(a,polynome) :
                self.polynome=self.polynome * a
                return self
            else : raise TypeError("Opération de multiplication non prise en charge.")
    def __rmull__(self,a):
        return self.__mul__(a)
    def __str__(self) :
        return(f" { str(self.polynome) } * {str(self.indicatrice)} ")
class Sum_ind_poly():
    def __init__(self,List_ind_poly):
        self.liste = List_ind_poly
        self.polynome = [ l.polynome for l in List_ind_poly]
        self.ind = [ l.indicatrice for l in List_ind_poly]
    def __add__(self,a):
        if isinstance(a,float) or isinstance(a,int) or isinstance(a,polynome) :
            for k in self.polynome : k = k + a
            return(Sum_ind_poly(self.liste))
        elif isinstance(a,ind_poly) : 
            check = True
            liste = self.liste.copy()
            for k in range (len(self.liste)) :
                if liste[k].indicatrice == a.indicatrice : 
                    liste[k] = liste[k] + a.polynome
                    check = False
            if check :
                liste.append(a)
            return(Sum_ind_poly(liste))
        elif isinstance(a,Sum_ind_poly) :
            for k in a.liste :
                self = self + k
            return(Sum_ind_poly(self.liste))
        else : raise TypeError("Opération d'addition non prise en charge.")
    def __radd__(self,a) :
        return self.__add__(a)
    def __mul__(self,a) :
        if isinstance(a,float) or isinstance(a,int) :
            liste = []
            for k in self.liste :
                liste.append(k*a)
            return(Sum_ind_poly(liste))
        raise TypeError("Opération de multiplication non prise en charge.")
    def __rmul__(self,a) :
        return self.__mul__(a)
    def __call__(self,point):
        somme =0
        for k in self.liste :
            somme += k(point)
        return somme
    def __str__(self) :
        s = f"{str(self.liste[0])}"
        for k in range (1,len(self.liste)) : s+=f"+ {str(self.liste[k])} "
        return s
# les fonctions
intervalle = 1
def ind_degre(rang, degre):
    if degre == 0:
        return indicatrice(rang * intervalle, (rang + 1) * intervalle)
    else:
        return (1 / ((degre) * intervalle)) * (
            polynome([-rang * intervalle, 1]) * ind_degre(rang, degre - 1)
            + polynome([(1 + rang + degre)* intervalle, -1]) * ind_degre(rang + 1, degre - 1)
        )
def approximation(liste, degre):
    fonction = Sum_ind_poly([])
    for rang in range(len(liste) - degre):
        L = ind_degre(rang, degre) * liste[rang]
        fonction = fonction + L
    return fonction


def interpolation(liste,degre) :
    fonction = Sum_ind_poly([])
    mat = matrice([[0 for k in range (len(liste))]*len(liste)])
    liste =[]
    for rang in range (len(liste)-degre) :
        L = ind_degre(rang,degre)
        liste.append(L)


import matplotlib.pyplot as plt

# Les points d'interpolation 
y_interpolation = [rd.uniform(-1,1) for k in range(10)]

# Convertir les points d'interpolation en listes de coordonnées x et y
x_interpolation = [ k for k in range (len(y_interpolation))]

# Degré d'interpolation B-spline
degre = 3

# Effectuer l'interpolation B-spline
fonction_interpolation = approximation(y_interpolation, degre)

# Calculer les valeurs interpolées pour l'affichage
L = [fonction_interpolation(k/100) for k in range(1000)]

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

