import numpy as np
import matplotlib.pyplot as plt
import math as mt
t,x,y,z = [],[],[],[]
with open("/Users/antoine/TIPE/TableTennis/Visualisation/transfer_value.txt", "r") as file:
    lines = file.readlines()
    i = 0
    for line in lines:
        cut = line[:-2].split(";")
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
fig, axs = plt.subplots(3)
fig.subplots_adjust(wspace=0.5,hspace=0.5)
axs[0].set_ylim(min(x)-0.1, max(x)+0.1)
axs[1].set_ylim(min(y), max(y) + 6*abs(mt.log10(max(y))))
axs[2].set_ylim(min(z), max(z) + 6*abs(mt.log10(max(y))))
axs[0].plot(t,x,"r-")
axs[0].plot(t,Ux,"w",t,Uxm,"w",alpha=0.2)
axs[1].plot(t,y,"b-")
axs[1].plot(t,Uy,"w",t,Uym,"w",alpha=0.2)
axs[2].plot(t,z,"g-")
axs[2].plot(t,Uz,"w",t,Uzm,"w",alpha=0.2)
axs[0].legend(["x"], loc="upper right")
axs[1].legend(["y"], loc="upper right")
axs[2].legend(["z"], loc="upper right")
axs[0].fill_between(t,Ux , Uxm,alpha=0.4)
axs[1].fill_between(t,Uy ,Uym ,alpha=0.4)
axs[2].fill_between(t, Uz,Uzm ,alpha=0.4)
axs[0].set_xlabel('Temps')
axs[0].set_ylabel('Abscisse')
axs[1].set_xlabel('Temps')
axs[1].set_ylabel('Ordonnée')
axs[2].set_xlabel('Temps')
axs[2].set_ylabel('Hauteur')
plt.show()
plt.clf()
