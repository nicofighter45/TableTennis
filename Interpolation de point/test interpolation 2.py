import matplotlib.pyplot as plt


# Les classes
class Indicatrice:
    def __init__(self, t1, t2):
        self.intervalle = (t1, t2)

    def __call__(self, point):
        if self.intervalle[0] <= point <= self.intervalle[1]:
            return 1
        else:
            return 0

    def __mul__(self, a):
        if isinstance(a, Polynome):
            return ind_poly(self, a)
        else:
            raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)


class Matrice:
    def __init__(self, *lists):
        listedeliste = []
        for list in lists:
            listedeliste.append(list)
        self.mat = listedeliste
        self.longueur = len(listedeliste[0])
        self.largeur = len(listedeliste)

    def __getitem__(self, indices):
        i, j = indices
        return self.mat[i][j]

    def __setitem__(self, indices, valeur):
        i, j = indices
        self.mat[i][j] = valeur

    def __repr__(self):
        return repr(self.mat)

    def __add__(self, B):
        if self.longueur == B.longueur and self.largeur == B.largeur:
            return Matrice(*([self[i, j] + B[i, j] for j in range(self.longueur)] for i in range(self.largeur)))
        else:
            return False

    def __mul__(self, B):
        if isinstance(B, Matrice):
            n, m, l = self.largeur, self.longueur, B.longueur
            if m != B.largeur:
                raise Exception("The number of column of M1 must be the same as the number of line of M2")
            M = []
            for i in range(n):
                line = []
                for j in range(l):
                    sum = 0
                    for k in range(m):
                        sum += self[i, k] * B[k, j]
                    line.append(sum)
                M.append(line)
            return Matrice(*M)
        if isinstance(B, int) or isinstance(B, float):
            for i in range(self.largeur):
                for j in range(self.longueur):
                    self[i, j] = self[i, j] * B
            return self
        raise Exception("B type isn't handle")


class Polynome:
    def __init__(self, *coefficients):
        p = -1
        while len(coefficients) and coefficients[p] == 0:
            p -= 1
        if p != -1:
            self.coefficients = coefficients
        else:
            self.coefficients = coefficients[:p + 1]
        self.degre = len(self.coefficients) - 1
        if not self.coefficients:
            self.coefdomi = 0
        else:
            self.coefdomi = coefficients[-1]

    def coef(self, deg):
        if deg > self.degre:
            return 0
        return self.coefficients[deg]

    def __call__(self, valeur):
        somme = 0
        for k in range(self.degre + 1):
            somme += self.coefficients[k] * valeur ** k
        return somme

    def __add__(self, a):
        if isinstance(a, Polynome):
            liste = []
            if a.degre <= self.degre:
                for k in range(len(a.coefficients)):
                    liste.append(a.coefficients[k] + self.coefficients[k])
            else:
                for k in range(len(self.coefficients)):
                    liste.append(a.coefficients[k] + self.coefficients[k])
            return Polynome(*liste)
        elif isinstance(a, float):
            self.coefficients[0] += a
            return self
        else:
            raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a): # todo implement faster algorithm for polynomial multiplication (seen in ocaml)
        if isinstance(a, float):
            return Polynome(*(coef * a for coef in self.coefficients))
        elif isinstance(a, Polynome):
            multi = Polynome()
            for i in range(a.degre + 1):
                multi += Polynome(*(0 for _ in range(i)), *(coef * a.coef(i) for coef in self.coefficients)) # not sure
            return multi
        elif isinstance(a, Indicatrice):
            return ind_poly(a, self)
        elif isinstance(a, ind_poly):
            a.polynome = a.polynome * self
            return a
        elif isinstance(a, Sum_ind_poly):
            for k in a.liste:
                k = self * k
            return a
        else:
            raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def der(self):
        return Polynome(*(k * self.coefficients[k] for k in range(1, self.degre + 1)))


class ind_poly:
    def __init__(self, indicatrice, polynome):
        self.indicatrice = indicatrice
        self.polynome = polynome

    def __call__(self, point):
        if not self.indicatrice(point):
            return 0
        else:
            return self.polynome(point)

    def __add__(self, a):
        if isinstance(a, float) or isinstance(a, Polynome):
            self.polynome = self.polynome + a
            return self
        elif isinstance(a, ind_poly):
            if self.indicatrice == a.indicatrice:
                self.polynome = self.polynome + a.polynome
                return self
            else:
                Somme = Sum_ind_poly([self, a])
                return Somme
        elif isinstance(a, Sum_ind_poly):
            check = True
            for k in range(len(a.liste)):
                if a.ind[k] == self.indicatrice:
                    a.liste[k] = a.liste[k] + self
                    check = False
            if check:
                a.liste.append(self)
                a.polynome.append(self.polynome)
                a.ind.append(self.indicatrice)
            return a
        else:
            raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a):
        if isinstance(a, float) or isinstance(a, Polynome):
            self.polynome = self.polynome * a
            return self
        else:
            raise TypeError("Opération de multiplication non prise en charge.")

    def __rmull__(self, a):
        return self.__mul__(a)


class Sum_ind_poly():
    def __init__(self, List_ind_poly):
        self.liste = List_ind_poly
        self.polynome = [l.polynome for l in List_ind_poly]
        self.ind = [l.indicatrice for l in List_ind_poly]

    def __add__(self, a):
        if isinstance(a, float) or isinstance(a, Polynome):
            for k in self.polynome:
                k = k + a
            return self
        elif isinstance(a, ind_poly):
            check = True
            for k in range(len(self.liste)):
                if self.ind[k] == a.indicatrice:
                    self.liste[k] = self.liste[k] + a
                    check = False
            if check:
                self.liste.append(a)
                self.polynome.append(a.polynome)
                self.ind.append(a.indicatrice)
            return self
        elif isinstance(a, Sum_ind_poly):
            for k in a.liste:
                self += k
            return self
        else:
            raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a):
        if isinstance(a, (float, int)):
            liste = []
            for k in self.liste:
                liste.append(k * a)
            return Sum_ind_poly(liste)
        elif isinstance(a, Sum_ind_poly):
            res = Sum_ind_poly(self.liste.copy())
            for k in a.liste:
                res += self * k
            return res
        else:
            raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def __call__(self, point):
        somme = 0
        for k in self.liste:
            somme += k(point)
        return somme


intervalle = 1e-3


# Les fonctions
def ind_degre(rang, degre):
    if degre == 0:
        return Indicatrice(rang * intervalle, (rang + 1) * intervalle)
    else:
        return 1 / (degre * intervalle) * (Polynome(-rang - degre, 1) * ind_degre(rang, degre - 1) +
                                           Polynome(1 + rang + degre, -1) * ind_degre(rang + 1, degre - 1))


def interpolations(liste, degre):
    fonction = Sum_ind_poly([])
    for rang in range(len(liste) - degre):
        if rang < len(liste):
            fonction = fonction + ind_degre(rang, degre) * liste[rang]
    return fonction


liste = [(-1) ** k for k in range(1000)]
degre = 3
fonction = interpolations(liste, degre)
L = [fonction(k * intervalle) for k in range(100000)]
t = [k * intervalle for k in range(len(L))]
plt.plot(t, L)
plt.xlabel('x')
plt.ylabel('y')
plt.title(f'Interpolation de degré {degre}')
plt.show()
