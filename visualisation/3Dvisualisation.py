import numpy as np
import matplotlib.pyplot as plt
import math as mt
t,x,y,z = [],[],[],[]
with open("transfer_value.txt", "r") as file:
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
def tracé_3D() :
    ax = plt.figure().add_subplot(projection='3d')
    ax.plot(x, y, z, label='lancée de balle de tennis de table')
    ax.legend()
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()

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

## Animation
def animation():
    fig2 = plt.figure()
    ax2 = fig2.add_subplot(projection='3d')
    ax2.set_xlim(min(x) -0.11, max(x)+0.11)
    ax2.set_ylim(min(y), max(y) + 6*abs(mt.log10(max(y))))
    ax2.set_zlim(0, 0.5)
    wframe = None
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_zlabel('Z')
    for phi in range (len(x)) :
        if wframe:
            wframe.remove()
        X = x[phi] + 0.01 * np.outer(np.cos(u), np.sin(v)) + 0.01
        Y = y[phi] + 0.01 * np.outer(np.sin(u), np.sin(v)) 
        Z = z[phi] + 0.01 * np.outer(np.ones(np.size(u)), np.cos(v)) +0.01
        wframe = ax2.plot_wireframe(X,Y ,Z,color= "#ff7f0e",label='lancée de balle de tennis de table')
        ax2.set_aspect('equal')
        plt.pause(0.01)

animation()