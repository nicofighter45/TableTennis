import matplotlib.ticker as ticker
import matplotlib.animation as animation
import numpy as np
import matplotlib.pyplot as pyplot


def on_close(_):
    pyplot.close('all')
    raise SystemExit


def createTimeCurve(data, label, time):
    fig, axs = pyplot.subplots(3, 1, label=label, gridspec_kw=dict(hspace=0))
    for i in range(3):
        axs[i].plot(time, data[i])
        axs[i].yaxis.set_major_locator(ticker.MaxNLocator(nbins=5, prune='both'))
    pyplot.setp(axs[0].get_xticklabels(), visible=False)
    pyplot.setp(axs[1].get_xticklabels(), visible=False)
    axs[2].set_xlabel('Time (s)')
    axs[0].set(ylabel=label + " X (m)")
    axs[1].set(ylabel=label + " Y (m)")
    axs[2].set(ylabel=label + " Z (m)")
    fig.canvas.mpl_connect('close_event', on_close)


def create2TimeCurve(data, label, time, coords):
    fig, axs = pyplot.subplots(2, 1, label=label, gridspec_kw=dict(hspace=0))
    for i in range(2):
        axs[i].plot(time, data[i])
        axs[i].yaxis.set_major_locator(ticker.MaxNLocator(nbins=5, prune='both'))
    pyplot.setp(axs[0].get_xticklabels(), visible=False)
    axs[1].set_xlabel('Time (s)')
    axs[0].set(ylabel=label + " " + coords[0] + " (m)")
    axs[1].set(ylabel=label + " " + coords[1] + " (m)")
    fig.canvas.mpl_connect('close_event', on_close)


def create1TimeCurve(ydata, xdata, color, marker, y_label=None, x_label=None, normed=False, y_pin=10, x_pin=10, label_size=24):
    fig, ax = pyplot.subplots(1, 1, gridspec_kw=dict(hspace=0))
    ax.plot(xdata, ydata, color=color, marker=marker, linewidth=3)
    ax.yaxis.set_major_locator(ticker.MaxNLocator(nbins=y_pin, prune='both'))
    ax.xaxis.set_major_locator(ticker.MaxNLocator(nbins=x_pin, prune='both'))
    if normed:
        ax.set_aspect(aspect="equal")
    ax.set_xlabel(x_label, fontsize=label_size)
    ax.set_xlabel(y_label, fontsize=label_size)
    fig.canvas.mpl_connect('close_event', on_close)
    fig.savefig(y_label + " " + x_label, bbox_inches="tight")


def update_3d_animation(num, data, ax):
    ax.set_data(data[0][:num], data[1][:num])
    ax.set_3d_properties(data[2][:num])
    return ax


def create_3d_subplot(data, label):
    fig = pyplot.figure(label + " 3D Animation")
    ax = fig.add_subplot(projection='3d')
    ax.set(xlim3d=(-1.6, 1.6), xlabel='X (m)')
    ax.set(ylim3d=(-0.2, 3), ylabel='Y (m)')
    ax.set(zlim3d=(-1, 2.2), zlabel='Z (m)')
    ball, = ax.plot([], [], [], marker='o', color="orange", markersize="0.1")
    ax.plot_surface(np.array([[-0.77, 0.77], [-0.77, 0.77]]),
                    np.array([[0, 0], [2.74, 2.74]]),
                    np.array([[0, 0], [0, 0]]), color='blue', alpha=0.1)

    def init():
        ball.set_data([], [])
        ball.set_3d_properties([])
        return ball,

    anim = animation.FuncAnimation(fig, update_3d_animation, len(data[0]),
                                   fargs=(data, ball), init_func=init, interval=50, blit=False)
    fig.canvas.mpl_connect('close_event', on_close)
    return anim
