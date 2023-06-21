import numpy as np
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec

t,x,y,z = [],[],[],[]
with open(input("name = ") + ".txt", "r") as file:
    lines = file.readlines()
    i = 0
    for line in lines:
        cut = line.replace("\\n", "").split(";")
        t.append(float(cut[0]))
        x.append(float(cut[1]))
        y.append(float(cut[2]))
        z.append(float(cut[3]))
incertitudes = 0.005
Ux,Uy,Uz = [l+incertitudes for l in x],[l+incertitudes for l in y],[l+incertitudes for l in z]
Uxm,Uym,Uzm = [l-incertitudes for l in x],[o-incertitudes for o in y],[p-incertitudes for p in z]
listede0=[0]*len(t)
def max(x) :
    max = x[0]
    for l in x :
        if max < l : max =l
    return(max)
def min(x) :
    min = x[0]
    for l in x :
        if min > l : min =l
    return(min)
fig = plt.figure(tight_layout=True)
gs = gridspec.GridSpec(2, 2)

ax = fig.add_subplot(gs[0, :])
ax1 = fig.add_subplot(gs[1, 0])
ax2 = fig.add_subplot(gs[1, 1])
fig.subplots_adjust(wspace=0.5,hspace=0.5)
ax.set_ylim(min(x)-0.1, max(x)+0.1)
if max(y) != 0:
    ax1.set_ylim(min(y), max(y) + 6*abs(mt.log10(abs(max(y)))))
    ax2.set_ylim(min(z), max(z) + 6*abs(mt.log10(abs(max(y)))))
ax.plot(t,x,"r-")
ax.plot(t,Ux,"w",t,Uxm,"w",alpha=0.2)
ax1.plot(t,y,"b-")
ax1.plot(t,Uy,"w",t,Uym,"w",alpha=0.2)
ax2.plot(t,z,"g-")
ax2.plot(t,Uz,"w",t,Uzm,"w",alpha=0.2)
ax.legend(["x"], loc="upper right")
ax1.legend(["y"], loc="upper right")
ax2.legend(["z"], loc="upper right")
ax.fill_between(t,Ux , Uxm,alpha=0.4)
ax1.fill_between(t,Uy ,Uym ,alpha=0.4)
ax2.fill_between(t, Uz,Uzm ,alpha=0.4)
ax.set_xlabel('Temps')
ax.set_ylabel('Abscisse')
ax1.set_xlabel('Temps')
ax1.set_ylabel('Ordonnée')
ax2.set_xlabel('Temps')
ax2.set_ylabel('Hauteur')
plt.show()
plt.clf()
