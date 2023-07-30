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
        elif isinstance(B, int) or isinstance(B, float):
            result = matrice([[self[i, j] * B for j in range(self.longueur)] for i in range(self.largeur)])
            return result
    def __rmul__(self,B):
        return self.__mul__(B)
    def coefligne(self,alpha,ligne) :
        for k in range (self.longueur) : 
            self[ligne,k]*=alpha 
        return self
    def liligne(self,ligne1,ligne2,alpha) :
        for k in range (self.longueur) : self[ligne1,k]-= alpha*self[ligne2,k]
        return self
    def echange(self,ligne1,ligne2) :
        M=self.donnee
        for k in range (self.longueur) : M[ligne1][k],M[ligne2][k] = M[ligne2][k],M[ligne1][k]
        return(matrice(M))
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
def plot(intervalle,y_interpolation,degre) :
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
L= [0.155, 0.156, 0.157, 0.15799, 0.158971, 0.159942, 0.16090300000000002, 0.161855, 0.162797, 0.163729, 0.164652, 0.16556500000000002, 0.166468, 0.167362, 0.168247, 0.169122, 0.169987, 0.170843, 0.17169, 0.172527, 0.173354, 0.174172, 0.174981, 0.17578, 0.17657, 0.17735, 0.178121, 0.17888300000000001, 0.17963500000000002, 0.180378, 0.181112, 0.181836, 0.182551, 0.183257, 0.183953, 0.184641, 0.185319, 0.18598800000000001, 0.186647, 0.187298, 0.187939, 0.18857100000000002, 0.189194, 0.189807, 0.190412, 0.191007, 0.19159400000000001, 0.192171, 0.192739, 0.193298, 0.193848, 0.194389, 0.194921, 0.195443, 0.195957, 0.196462, 0.196958, 0.197445, 0.19792300000000002, 0.198392, 0.198852, 0.199303, 0.199745, 0.200178, 0.200602, 0.201018, 0.201424, 0.201822, 0.202211, 0.202591, 0.202962, 0.203324, 0.203677, 0.204022, 0.204358, 0.204685, 0.205003, 0.205313, 0.205614, 0.205906, 0.206189, 0.206464, 0.20673, 0.206987, 0.207235, 0.207475, 0.207706, 0.207929, 0.208143, 0.208348, 0.208545, 0.208733, 0.20891200000000001, 0.209083, 0.20924500000000001, 0.209399, 0.209544, 0.20968, 0.209808, 0.209928, 0.210038, 0.210141, 0.210235, 0.21032, 0.210397, 0.210465, 0.21052500000000002, 0.21057700000000001, 0.21062, 0.210654, 0.21068, 0.210698, 0.210707, 0.210708, 0.2107, 0.210684, 0.21066000000000001, 0.210627, 0.210586, 0.210537, 0.210479, 0.21041300000000002, 0.210339, 0.210256, 0.210165, 0.210066, 0.209958, 0.209842, 0.20971800000000002, 0.209585, 0.209444, 0.209296, 0.209138, 0.208973, 0.208799, 0.208617, 0.208427, 0.208229, 0.208022, 0.207808, 0.207585, 0.207354, 0.207115, 0.206867, 0.20661200000000002, 0.206348, 0.206076, 0.205796, 0.205509, 0.205212, 0.204908, 0.204596, 0.204276, 0.20394700000000002, 0.20361100000000001, 0.203266, 0.202914, 0.202553, 0.202185, 0.20180800000000002, 0.201423, 0.20103100000000002, 0.20063, 0.200221, 0.199805, 0.19938, 0.198948, 0.19850700000000002, 0.198059, 0.197602, 0.197138, 0.196666, 0.196185, 0.195697, 0.195201, 0.194697, 0.194186, 0.193666, 0.193138, 0.192603, 0.19206, 0.191509, 0.19095, 0.190383, 0.189808, 0.189226, 0.188635, 0.188037, 0.18743200000000002, 0.186818, 0.186196, 0.185567, 0.18493, 0.184285, 0.183633, 0.182973, 0.182305, 0.181629, 0.180945, 0.180254, 0.179555, 0.178849, 0.17813500000000002, 0.17741300000000002, 0.176683, 0.175946, 0.175201, 0.174448, 0.173688, 0.17292000000000002, 0.172144, 0.171361, 0.17057, 0.169772, 0.168966, 0.168152, 0.167331, 0.166502, 0.165666, 0.164822, 0.16397, 0.163111, 0.162245, 0.16137, 0.160489, 0.159599, 0.158703, 0.157798, 0.156887, 0.155967, 0.155041, 0.154106, 0.153165, 0.15221600000000002, 0.151259, 0.150295, 0.149323, 0.148344, 0.14735800000000002, 0.146364, 0.145363, 0.144354, 0.143338, 0.142315, 0.141284, 0.140246, 0.13920000000000002, 0.138147, 0.13708700000000001, 0.136019, 0.134944, 0.133861, 0.132772, 0.13167500000000001, 0.13057, 0.129459, 0.12834, 0.127214, 0.12608, 0.12493900000000001, 0.123791, 0.12263600000000001, 0.121473, 0.12030300000000001, 0.11912600000000001, 0.117942, 0.11675, 0.115551, 0.114345, 0.11313200000000001, 0.111912, 0.110684, 0.109449, 0.108207, 0.106958, 0.105702, 0.1044384, 0.1031678, 0.10189000000000001, 0.1006051, 0.0993131, 0.098014, 0.09670770000000001, 0.0953944, 0.0940739, 0.0927464, 0.0914118, 0.0900701, 0.0887213, 0.0873655, 0.0860026, 0.0846327, 0.0832557, 0.08187180000000001, 0.0804807, 0.0790827, 0.0776777, 0.0762657, 0.0748466, 0.0734206, 0.0719876, 0.0705477, 0.0691007, 0.06764690000000001, 0.066186, 0.0647183, 0.0632436, 0.061761899999999995, 0.0602734, 0.058777899999999994, 0.057275599999999996, 0.0557663, 0.0542502, 0.052727199999999995, 0.051197299999999994, 0.049660499999999996, 0.0481169, 0.0465664, 0.045009099999999996, 0.043445, 0.041873999999999995, 0.0402962, 0.0387116, 0.0371202, 0.035522, 0.033917, 0.0323053, 0.0306867, 0.0290614, 0.0274294, 0.0257906, 0.024145, 0.0224927, 0.0208337, 0.019168, 0.0174955, 0.0158164, 0.014130529999999999, 0.012438000000000001, 0.01073878, 0.0090329, 0.00732035, 0.005601143, 0.00387529]
plot(1,L,3)