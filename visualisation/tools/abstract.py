class indicatrice:
    def __init__(self, t1, t2):
        self.intervalle = (t1, t2)

    def __call__(self, point):
        return self.intervalle[0] <= point < self.intervalle[1]

    def __mul__(self, a):
        if isinstance(a, polynome):
            return ind_poly(self, a)
        raise TypeError("Opération de multiplication non prise en charge 1.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def __str__(self):
        return f" Id{self.intervalle} "

    def __eq__(self, a):
        return self.intervalle == a.intervalle


class matrice:
    def __init__(self, listedeliste):
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

    def __add__(self, B):
        if self.longueur == B.longueur and self.largeur == B.largeur:
            return matrice([[self[i][j] + B[i][j] for j in range(len(self[i]))] for i in range(len(self))])
        raise Exception("Can't add matrice that aren't the same size")

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

    def __rmul__(self, B):
        return self.__mul__(B)

    def coefligne(self, alpha, ligne):
        for k in range(self.longueur):
            self[ligne, k] *= alpha
        return self

    def liligne(self, ligne1, ligne2, alpha):
        for k in range(self.longueur):
            self[ligne1, k] -= alpha * self[ligne2, k]
        return self

    def echange(self, ligne1, ligne2):
        M = self.donnee
        for k in range(self.longueur):
            M[ligne1][k], M[ligne2][k] = M[ligne2][k], M[ligne1][k]
        return matrice(M)


class polynome:
    def __init__(self, coefficient):
        self.coefficient = coefficient
        self.degre = len(self.coefficient) - 1
        if not self.coefficient:
            self.coefdomi = 0
        else:
            self.coefdomi = coefficient[-1]

    def coef(self, deg):
        if deg > self.degre:
            return 0
        return self.coefficient[deg]

    def __call__(self, valeur):
        somme = 0
        for k in range(self.degre + 1):
            somme += self.coefficient[k] * valeur ** k
        return somme

    def __add__(self, a):
        if isinstance(a, polynome):
            liste = []
            if a.degre <= self.degre:
                for k in range(len(a.coefficient)):
                    liste.append(a.coefficient[k] + self.coefficient[k])
                liste += self.coefficient[len(a.coefficient):]
            else:
                for j in range(len(self.coefficient)):
                    liste.append(a.coefficient[j] + self.coefficient[j])
                liste += a.coefficient[len(self.coefficient):]
            return polynome(liste)
        elif isinstance(a, float) or isinstance(a, int):
            self.coefficient[0] += a
            return self
        raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a):
        if isinstance(a, float) or isinstance(a, int):
            liste = [coef * a for coef in self.coefficient]
            return polynome(liste)
        elif isinstance(a, polynome):
            multi = polynome([0])
            for i in range(a.degre + 1):
                liste = [0] * i + [coef * a.coefficient[i] for coef in self.coefficient]
                multi = multi + polynome(liste)
            return multi
        elif isinstance(a, indicatrice):
            return ind_poly(a, self)
        elif isinstance(a, ind_poly):
            a.polynome = a.polynome * self
            return a
        elif isinstance(a, Sum_ind_poly):
            for k in a.liste:
                k = self * k
            return a
        raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def der(self):
        return polynome([k * self.coefficient[k] for k in range(1, self.degre + 1)])

    def __str__(self):
        s = ""
        for k in range(len(self.coefficient)):
            if not self.coefficient[k] == 0:
                s = f" {self.coefficient[k]} x^{k} + " + s
        return s[:-2]


class ind_poly:
    def __init__(self, indicatrice, polynome):
        self.indicatrice = indicatrice
        self.polynome = polynome

    def __call__(self, point):
        if self.indicatrice(point) == 0:
            return 0
        return self.polynome(point)

    def __add__(self, a):
        if isinstance(a, float) or isinstance(a, int) or isinstance(a, polynome):
            poly = self.polynome + a
            return ind_poly(self.indicatrice, poly)
        elif isinstance(a, ind_poly):
            if self.indicatrice == a.indicatrice:
                poly = self.polynome + a.polynome
                return ind_poly(self.indicatrice, poly)
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
        raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a):
        if isinstance(a, float) or isinstance(a, int) or isinstance(a, polynome):
            self.polynome = self.polynome * a
            return self
        else:
            raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def __str__(self):
        return f" {str(self.polynome)} * {str(self.indicatrice)} "


class Sum_ind_poly:
    def __init__(self, List_ind_poly):
        self.liste = List_ind_poly
        self.polynome = [l.polynome for l in List_ind_poly]
        self.ind = [l.indicatrice for l in List_ind_poly]

    def __add__(self, a):
        if isinstance(a, float) or isinstance(a, int) or isinstance(a, polynome):
            for k in self.polynome: k = k + a
            return Sum_ind_poly(self.liste)
        elif isinstance(a, ind_poly):
            check = True
            liste = self.liste.copy()
            for k in range(len(self.liste)):
                if liste[k].indicatrice == a.indicatrice:
                    liste[k] = liste[k] + a.polynome
                    check = False
            if check:
                liste.append(a)
            return Sum_ind_poly(liste)
        elif isinstance(a, Sum_ind_poly):
            for k in a.liste:
                self = self + k
            return Sum_ind_poly(self.liste)
        raise TypeError("Opération d'addition non prise en charge.")

    def __radd__(self, a):
        return self.__add__(a)

    def __mul__(self, a):
        if isinstance(a, float) or isinstance(a, int):
            liste = []
            for k in self.liste:
                liste.append(k * a)
            return Sum_ind_poly(liste)
        raise TypeError("Opération de multiplication non prise en charge.")

    def __rmul__(self, a):
        return self.__mul__(a)

    def __call__(self, point):
        somme = 0
        for k in self.liste:
            somme += k(point)
        return somme

    def __str__(self):
        s = f"{str(self.liste[0])}"
        for k in range(1, len(self.liste)):
            s += f"+ {str(self.liste[k])} "
        return s