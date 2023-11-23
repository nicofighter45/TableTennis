import matplotlib.pyplot as plt

from abstract import *

intervalle = 1


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
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()


def plot_approximation_et_point(y_interpolation, degre):
    x_interpolation = [k * intervalle for k in range(len(y_interpolation))]
    # Effectuer l'approxilation B-spline

    fonction_interpolation = approximation(y_interpolation, degre)
    fonction_interpolation = der(fonction_interpolation)
    # Calculer les valeurs interpolées pour l'affichage
    L = [fonction_interpolation((3 * degre + k / 100) * intervalle) for k in range((len(y_interpolation) - 4) * 100)]
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


liste = [2.6485286783042397, 2.6485286783042397, 2.6485286783042397, 2.646982543640898, 2.6485286783042397,
         2.646982543640898, 2.6485286783042397, 2.6485286783042397, 2.6485286783042397, 2.6485286783042397,
         2.6485286783042397, 2.6485286783042397, 2.6485286783042397, 2.6485286783042397, 2.6485286783042397,
         2.6485286783042397, 2.6500748129675813, 2.6500748129675813, 2.6500748129675813, 2.6500748129675813,
         2.6500748129675813, 2.651620947630923, 2.651620947630923, 2.651620947630923, 2.651620947630923,
         2.651620947630923, 2.6500748129675813, 2.6500748129675813, 2.646982543640898, 2.6454364089775564,
         2.6454364089775564, 2.6407980049875315, 2.6377057356608478, 2.631521197007481, 2.6253366583541147,
         2.619152119700748, 2.6083291770573567, 2.6005985037406485, 2.5913216957605987, 2.5804987531172072,
         2.5681296758104737, 2.5557605985037406, 2.541845386533666, 2.527930174563591, 2.512468827930175,
         2.4939152119700747, 2.4769077306733167, 2.458354114713217, 2.438254364089776, 2.418154613466334,
         2.398054862842893, 2.3748628428927683, 2.3516708229426433, 2.3284788029925187, 2.3037406483790526,
         2.2774563591022443, 2.2511720698254365, 2.223341645885287, 2.195511221945137, 2.166134663341646,
         2.1367581047381545, 2.10428927680798, 2.0749127182044886, 2.0408977556109726, 2.0068827930174566,
         1.9728678304239402, 1.9373067331670824, 1.9017456359102245, 1.864638403990025, 1.8259850374064839,
         1.7888778054862844, 1.7502244389027433, 1.7100249376558605, 1.6698254364089775, 1.6280798004987531,
         1.5863341645885287, 1.5430423940149627, 1.4997506234413966, 1.4549127182044888, 1.410074812967581,
         1.3636907730673318, 1.3173067331670822, 1.2693765586034913, 1.222992518703242, 1.170423940149626,
         1.1240399002493766, 1.0730174563591022, 1.021995012468828, 0.9709725685785536, 0.9184039900249377,
         0.8658354114713217, 0.8101745635910225, 0.7560598503740649, 0.7019451371571073, 0.6447381546134664,
         0.5875311720698254, 0.5303241895261845, 0.47311720698254367, 0.4143640897755611, 0.3540648379052369,
         0.2937655860349127, 0.23346633416458853, 0.17316708229426434, 0.11132169576059851, 0.04793017456359103,
         0.004638403990024938, 0.04638403990024938, 0.10204488778054863, 0.15306733167082295, 0.20563591022443892,
         0.2551122194513716, 0.30458852867830427, 0.35097256857855363, 0.397356608478803, 0.4421945137157107,
         0.4839401496259352, 0.5256857855361596, 0.567431421446384, 0.6076309226932669, 0.6447381546134664,
         0.6818453865336659, 0.7174064837905237, 0.7498753117206983, 0.7854364089775562, 0.8179052369077306,
         0.847281795511222, 0.8782044887780549, 0.9075810473815462, 0.9369576059850374, 0.9632418952618454,
         0.9879800498753117, 1.0127182044887781, 1.0359102244389027, 1.0591022443890274, 1.079201995012469,
         1.0993017456359102, 1.1194014962593517, 1.1379551122194513, 1.1549625935162096, 1.170423940149626,
         1.1874314214463841, 1.2013466334164589, 1.213715710723192, 1.2260847880299253, 1.2384538653366584,
         1.2477306733167082, 1.2570074812967582, 1.266284289276808, 1.2724688279301746, 1.2786533665835411,
         1.2848379052369079, 1.2894763092269328, 1.2910224438902744, 1.2941147132169577, 1.2956608478802993,
         1.2956608478802993, 1.2956608478802993, 1.2941147132169577, 1.2910224438902744, 1.2879301745635912,
         1.283291770573566, 1.2786533665835411, 1.2724688279301746, 1.2647381546134664, 1.2585536159600998,
         1.2477306733167082, 1.2384538653366584, 1.2260847880299253, 1.2152618453865338, 1.2013466334164589,
         1.1874314214463841, 1.1735162094763092, 1.1565087281795512, 1.1410473815461346, 1.1240399002493766,
         1.1054862842892768, 1.0853865336658355, 1.065286783042394, 1.043640897755611, 1.0204488778054863,
         0.9988029925187033, 0.9740648379052369, 0.9508728179551122, 0.9230423940149626, 0.8983042394014963,
         0.8704738154613466, 0.8410972568578554, 0.8117206982543641, 0.7807980049875312, 0.7498753117206983,
         0.7174064837905237, 0.6849376558603492, 0.6509226932668329, 0.6153615960099751, 0.5813466334164589,
         0.545785536159601, 0.5055860349127183, 0.4684788029925187, 0.4298254364089776, 0.38962593516209476,
         0.349426433915212, 0.30768079800498754, 0.26438902743142145, 0.22109725685785536, 0.17780548628428927,
         0.1345137157107232, 0.08658354114713217, 0.04174563591022444, 0.0, 0.02628428927680798, 0.06648379052369077,
         0.10513715710723193, 0.14224438902743142, 0.18089775561097257, 0.2149127182044888, 0.2504738154613466,
         0.2829426433915212, 0.3154114713216958, 0.34633416458852867, 0.37416458852867834, 0.4035411471321696,
         0.4298254364089776, 0.45610972568578556, 0.4808478802992519, 0.5055860349127183, 0.5272319201995013,
         0.5488778054862843, 0.5705236907730673, 0.5906234413965088, 0.6076309226932669, 0.6261845386533667,
         0.641645885286783, 0.6571072319201995, 0.6710224438902743, 0.6849376558603492, 0.6973067331670824,
         0.7096758104738154, 0.7189526184538654, 0.7282294264339152, 0.7375062344139651, 0.7436907730673317,
         0.75142144638404, 0.7560598503740649, 0.7606982543640898, 0.763790523690773, 0.7653366583541148,
         0.7668827930174564, 0.7684289276807981, 0.7668827930174564, 0.7653366583541148, 0.763790523690773,
         0.7606982543640898, 0.7560598503740649, 0.75142144638404, 0.7452369077306733, 0.7375062344139651,
         0.7297755610972568, 0.720498753117207, 0.7096758104738154, 0.698852867830424, 0.6864837905236908,
         0.6741147132169576, 0.6601995012468829, 0.6447381546134664, 0.6292768079800499, 0.6122693266832918,
         0.5937157107231921, 0.5751620947630923, 0.5566084788029926, 0.5349625935162095, 0.5133167082294264,
         0.4916708229426434, 0.46693266832917707, 0.44374064837905236, 0.41900249376558607, 0.3927182044887781,
         0.3648877805486284, 0.3370573566084788, 0.3061346633416459, 0.2767581047381546, 0.2458354114713217,
         0.2149127182044888, 0.18089775561097257, 0.14842892768079802, 0.1144139650872818, 0.07885286783042394,
         0.043291770573566084, 0.006184538653366584, 0.001546134663341646, 0.03401496259351621, 0.06339152119700749,
         0.0943142144638404, 0.12214463840399004, 0.14997506234413965, 0.17780548628428927, 0.20254364089775562,
         0.22728179551122196, 0.2504738154613466, 0.2721197007481297, 0.2937655860349127, 0.31386533665835414,
         0.3339650872817955, 0.35251870324189527, 0.36798004987531174, 0.38498753117206985, 0.39890274314214463,
         0.41281795511221947, 0.4251870324189526, 0.4375561097256858, 0.4483790523690773, 0.4576558603491272,
         0.46693266832917707, 0.4746633416458853, 0.4808478802992519, 0.48703241895261845, 0.4916708229426434,
         0.4947630922693267, 0.49785536159601, 0.49940149625935165, 0.5009476309226932, 0.5009476309226932,
         0.49940149625935165, 0.49785536159601, 0.4947630922693267, 0.4901246882793018, 0.4854862842892768]


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
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()


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
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Interpolation B-spline de degré {degre}')
    plt.show()
