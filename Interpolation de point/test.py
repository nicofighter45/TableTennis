from pylab import *
from math import *
import random
import numpy as np
import math
import matplotlib.pyplot as plt

"""
On fait une regression linéaire ax+b sur des données (x,y) entachées d'incertitudes
Pour cela, on tire au sort des valeurs (x,y) qu'on ajuste, et on recommence N fois l'opération
On en déduit les coefficients d'ajustement (a,b) avec leurs incertitudes
"""

xvalues=[1,2,3,4,5]                     #valeurs de x à rentrer à la main
yvalues=[3.1,4.9,7.1,8.8,11.3]          #valeurs de y à rentrer à la main
incertitude_x=0.2 ; incertitude_y=0.6   #incertitudes-types toutes égales


liste_a=[] ; liste_b=[]                 #liste encore vides des différentes valeurs des paramètres a et b


for i in range (100):                   #on va faire N tirages au sort des valeurs de x et y
    valeurs_de_x=[] ; valeurs_de_y=[]   #les listes vides qu'on va remplir avec les valeurs tirées au sort
    for j in range (len(yvalues)) :     #remplissage des listes de valeurs de x et y
        valeurs_de_x.append(random.gauss(xvalues[j],incertitude_x))
        valeurs_de_y.append(random.gauss(yvalues[j],incertitude_y))
    fit = np.polyfit(valeurs_de_x, valeurs_de_y, 1)     #fit qui renvoie [a,b] en ajustant par y=ax+b
    liste_a.append(fit[0]) ; liste_b.append(fit[1])     #on remplit les listes avec les N valeurs trouvées pour a et b

a=[np.mean(liste_a),np.std(liste_a)] #moyenne et écart-type de toutes les valeurs de a
b=[np.mean(liste_b),np.std(liste_b)] #pareil pour b

print("a=",a[0],"u(a)=",a[1])
print("b=",b[0],"u(b)=",b[1])


xfonction=np.arange(0,6,0.1) #liste de abcisses pour le tracé des droites ax+b

#tracé des valeurs expérimentales avec les barres d'erreur correspondant aux incertitudes-types
plt.xlabel("xvalues")
plt.ylabel("yvalues")
plt.scatter(xvalues, yvalues, color='red')
plt.errorbar(xvalues, yvalues, xerr = incertitude_x, yerr = incertitude_y,
  fmt = 'none', capsize = 3, ecolor = 'red')
print(a[0],a[1])
#tracé des droites "extrêmes" avec les incertitudes-types de $a$ et $b$
plt.plot(xfonction, (a[0]-a[1])*xfonction+(b[0]+b[1]), color='blue')
plt.plot(xfonction, (a[0]+a[1])*xfonction+(b[0]-b[1]), color='orange')
plt.show()