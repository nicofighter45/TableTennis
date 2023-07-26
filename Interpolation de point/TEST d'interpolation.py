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
        return self.donnee[i][j]
    def __setitem__(self, indices, valeur):
        i, j = indices
        self.donnee[i][j] = valeur
    def __repr__(self):
        return repr(self.donnee)
    def __add__(A,B):
        if A.longueur == B.longueur and A.largeur == B.largeur :
            return(matrice([[A[i][j] + B[i][j] for j in range (len(A[i]))] for i in range (len(A))]))
        else : return(False)
    def __mul__(self, B):
        if isinstance(B, matrice):
            n, m, l = self.largeur, self.longueur, B.longueur
            if m != B.largeur:
                raise Exception("The number of columns of M1 must be the same as the number of rows of M2")
            
            result = matrice([[0 for _ in range(l)] for _ in range(n)])
            
            for i in range(n):
                for j in range(l):
                    for k in range(m):
                        result[i, j] += self[i, k] * B[k, j]
            
            return result
        elif isinstance(B, int):
            result = matrice([[self[i, j] * B for j in range(self.longueur)] for i in range(self.largeur)])
            return result
    def __rmul__(self,B):
        return self.__mul__(B)
    def coefligne(self,alpha,ligne) :
        for k in range (self.largeur) : 
            if  1>=self[ligne,k]*alpha > 0.99 : self[ligne,k] = 1
            elif 0.01>abs(self[ligne,k]*alpha) >= 0 : self[ligne,k] = 0
            else : self[ligne,k]*=alpha 
        return self
    def liligne(self,ligne1,ligne2,alpha) :
        for k in range (self.largeur) : self[ligne1,k]-= alpha*self[ligne2,k]
        return self
    def échange(self,ligne1,ligne2) :
        for k in range (self.largeur) : self[ligne1,k],self[ligne2,k] = self[ligne2,k],self[ligne1,k]
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
    def __rmul__(self,a):
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
        return (1/(degre*intervalle)*
            (polynome([-rang * intervalle, 1]) * ind_degre(rang, degre - 1)
            + polynome([(1 + rang + degre)* intervalle, -1]) * ind_degre(rang + 1, degre - 1))
        )
def approximation(liste, degre):
    fonction = Sum_ind_poly([])

    for rang in range(len(liste)):
        L = ind_degre(rang+degre+2, degre) * liste[rang]
        fonction = fonction + L
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
def interpolation(liste,degre) :
    fonction = Sum_ind_poly([])
    mat1 = matrice([[0]]+[[k]for k in liste]+[[0]])
    mat2 = matrice([[0 for k in range (len(liste)+2)]for k in range(len(liste)+2)])
    liste1 =[]
    m=len(liste)
    for rang in range (m+2) :
        L = ind_degre(rang,degre)
        liste1.append(L)
    for k in range (m) :
        for i in range (m+2) : mat2[k+1,i] = liste1[i]((k+3)*intervalle)
    liste1der = [der(l) for l in liste1]
    for j in range (m+2) : 
        mat2[0,j] = liste1der[j](5*intervalle)
        mat2[-1,j] = liste1der[j](m*intervalle)
    inverse1 = résolution(mat2,mat1)
    point_de_controle = inverse1*mat1
    for k in range (0,m+2): fonction = fonction + liste1[k]*point_de_controle[k,0]
    return fonction


def der(self) :
        if type(self) == ind_poly : return ind_poly(self.indicatrice,der(self.polynome))
        elif type(self) == polynome : return polynome([k*self.coefficient[k] for k in range (1,self.degre+1)])
        elif type(self) == Sum_ind_poly : return Sum_ind_poly([der(k) for k in self.liste])
        else : raise Exception("Pas dérivable")

# Les points d'interpolation 
y_interpolation = [ rd.uniform(k,k+1)  for k in range(10,41)]

# Convertir les points d'interpolation en listes de coordonnées x et y
x_interpolation = [ k for k in range (len(y_interpolation))]

# Degré d'interpolation B-spline
degre = 3

# Effectuer l'interpolation B-spline
fonction_interpolation = approximation(y_interpolation, degre)

# Calculer les valeurs interpolées pour l'affichage
L = [fonction_interpolation((3+k/100)*intervalle) for k in range(4100)]

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