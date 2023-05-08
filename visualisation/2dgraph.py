import numpy as np
import matplotlib.pyplot as plt
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
incertitudes = 0.05
Ux,Uy,Uz = [l+incertitudes for l in x],[l+incertitudes for l in y],[l+incertitudes for l in z]
Uxm,Uym,Uzm = [l-incertitudes for l in x],[o-incertitudes for o in y],[p-incertitudes for p in z]
listede0=[0]*len(t)

fig, (axs) = plt.subplots(2, 2)
fig.subplots_adjust(wspace=0.5,hspace=0.5)
axs[0,0].plot(t,x,"r-")
axs[0,0].plot(t,Ux,"w",t,Uxm,"w",alpha=0.2)
axs[1,0].plot(t,y,"b-")
axs[1,0].plot(t,Uy,"w",t,Uym,"w",alpha=0.2)
axs[0,1].plot(t,z,"g-")
axs[0,1].plot(t,Uz,"w",t,Uzm,"w",alpha=0.2)
axs[0,0].legend(["x"], loc="upper right")
axs[1,0].legend(["y"], loc="upper right")
axs[0,1].legend(["z"], loc="upper right")
axs[0,0].fill_between(t,Ux , Uxm,alpha=0.4)
axs[1,0].fill_between(t,Uy ,Uym ,alpha=0.4)
axs[0,1].fill_between(t, Uz,Uzm ,alpha=0.4)
axs[0,0].set_xlabel('Temps')
axs[0,0].set_ylabel('Abscisse')
axs[1,0].set_xlabel('Temps')
axs[1,0].set_ylabel('Ordonnée')
axs[0,1].set_xlabel('Temps')
axs[0,1].set_ylabel('Hauteur')
ax = plt.figure().add_subplot(projection='3d')
ax.plot(x, y, z, label='lancée de balle de tennis de table')
ax.legend()
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()
plt.clf()

