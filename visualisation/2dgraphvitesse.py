import numpy as np
import matplotlib.pyplot as plt
import math as mt
import matplotlib.gridspec as gridspec

t,x,y,z = [],[],[],[]
with open("/Users/antoine/TIPE/TableTennis/Visualisation/magnus speed.txt", "r") as file:
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
gs = gridspec.GridSpec(2, 1)

ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[1, 0])
fig.subplots_adjust(wspace=0.5,hspace=0.5)
if max(y) != 0:
    ax1.set_ylim(min(y)-0.1, max(y) + 0.1*abs(mt.log10(abs(max(y)))))
    ax2.set_ylim(min(z)-0.1, max(z) + 0.1*abs(mt.log10(abs(max(z)))))
ax1.plot(t,y,"b-")
ax1.plot(t,Uy,"w",t,Uym,"w",alpha=0.2)
ax2.plot(t,z,"g-")
ax2.plot(t,Uz,"w",t,Uzm,"w",alpha=0.2)
ax1.legend(["y"], loc="upper right")
ax1.set_title('Vitesse sur Uy ', loc="center")
ax2.set_title('Vitesse sur Uz ', loc="center")
ax2.legend(["z"], loc="upper right")
ax1.fill_between(t,Uy ,Uym ,alpha=0.4)
ax2.fill_between(t, Uz,Uzm ,alpha=0.4)
ax1.set_xlabel('Temps')
ax1.set_ylabel('Vy')
ax2.set_xlabel('Temps')
ax2.set_ylabel('Vz')
plt.show()
plt.clf()
