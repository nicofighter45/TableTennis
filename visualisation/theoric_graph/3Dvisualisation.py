import math as mt

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

t, x, y, z = [], [], [], []
with open("Visualisation/magnus position.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        cut = line.replace("\n", "").split(";")
        t.append(float(cut[0]))
        x.append(float(cut[1]))
        y.append(float(cut[2]))
        z.append(float(cut[3]))
incertitudes = 0.05
Ux, Uy, Uz = [l + incertitudes for l in x], [l + incertitudes for l in y], [l + incertitudes for l in z]
Uxm, Uym, Uzm = [l - incertitudes for l in x], [o - incertitudes for o in y], [p - incertitudes for p in z]
listede0 = [0] * len(t)
T, X, Y, Z = np.array(t), np.array(x), np.array(y), np.array(z)


def tracé_3D():
    ax = plt.figure().add_subplot(projection='3d')
    ax.plot(x, y, z, label='lancée de balle de tennis de table')
    ax.legend()
    ax.set_xlim(min(x), max(x))
    ax.set_ylim(min(y), max(y))
    ax.set_zlim(0, max(z))
    ax.set_aspect('equal')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()


def max(x):
    max = x[0]
    for l in x:
        if max < l: max = l
    return (max)


def min(x):
    min = x[0]
    for l in x:
        if min > l: min = l
    return (min)


## Animation
def animation():
    fig2 = plt.figure()
    ax2 = fig2.add_subplot(projection='3d')

    # Met les conditions sur les axes et les labels
    ax2.set_xlim(min(x) - 0.11, max(x) + 0.11)
    ax2.set_ylim(min(y), max(y) + 6 * abs(mt.log10(max(y))))
    ax2.set_zlim(0, 0.5)
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_zlabel('Z')

    wframe = None
    # faire la balle
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    for phi in range(len(x)):
        if wframe:
            wframe.remove()
        X = x[phi] + 0.01 * np.outer(np.cos(u), np.sin(v)) + 0.01
        Y = y[phi] + 0.01 * np.outer(np.sin(u), np.sin(v))
        Z = z[phi] + 0.01 * np.outer(np.ones(np.size(u)), np.cos(v)) + 0.01
        wframe = ax2.plot_wireframe(X, Y, Z, color="#ff7f0e", label='lancée de balle de tennis de table')
        ax2.set_aspect('equal')
        plt.pause(0.01)


def courbeanime():
    # créer la figure et les axes 3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Fait la ligne rouge
    line, = ax.plot([], [], [], color="#ff7f0e", lw=2)

    def update(frame):
        # Met a jour les frames
        line.set_data(X[:frame], Y[:frame])
        line.set_3d_properties(Z[:frame])

        # créer l'animation

    animation = FuncAnimation(fig, update, frames=len(t), interval=50)

    # Met les conditions sur les axes et les labels
    ax.set_xlim(min(x), max(x))
    ax.set_ylim(min(y), max(y))
    ax.set_zlim(0, max(z))
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('3D Animation')

    plt.show()


def optimized_animation():
    # faire la figure
    fig2 = plt.figure()
    ax2 = fig2.add_subplot(projection='3d')

    # Met les conditions sur les axes et les labels
    ax2.set_xlim(min(x) - 0.11, max(x) + 0.11)
    ax2.set_ylim(min(y), max(y) + 6 * abs(mt.log10(max(y))))
    ax2.set_zlim(0, 0.5)
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_zlabel('Z')
    # Faire la balle
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    wframe = None

    def update_frame(phi):
        nonlocal wframe

        ax2.clear()
        ax2.set_xlim(min(x) - 0.11, max(x) + 0.11)
        ax2.set_ylim(min(y), max(y) + 6 * abs(mt.log10(max(y))))
        ax2.set_zlim(0, 0.5)
        # Faire la balle
        X = x[phi] + 0.01 * np.outer(np.cos(u), np.sin(v)) + 0.01
        Y = y[phi] + 0.01 * np.outer(np.sin(u), np.sin(v))
        Z = z[phi] + 0.01 * np.outer(np.ones(np.size(u)), np.cos(v)) + 0.01
        # fait le frame
        wframe = ax2.plot_wireframe(X, Y, Z, color="#ff7f0e", label='lancée de balle de tennis de table')
        ax2.set_aspect('equal')

    # créer l'animation
    animation = FuncAnimation(fig2, update_frame, frames=len(x), interval=1)

    plt.show()


optimized_animation()
tracé_3D()
