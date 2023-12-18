import matplotlib.pyplot as pyplot
import matplotlib.ticker as ticker

time_step = 0.001

positions, speeds, accelerations, time = [[], [], []], [[], [], []], [[], [], []], []

lines = open("C:/Users/fagot/Code/TableTennis/output/simulations/transfer.txt", "r").readlines()
for line in lines:
    line.removesuffix("\n")
    inputs = line.split(";;")
    input_positions = inputs[0].split(";")
    input_speeds = inputs[1].split(";")
    input_acceleration = inputs[2].split(";")
    for i in range(len(input_positions)):
        positions[i].append(float(input_positions[i]))
    for i in range(len(input_speeds)):
        speeds[i].append(float(input_speeds[i]))
    for i in range(len(input_acceleration)):
        accelerations[i].append(float(input_acceleration[i]))

time = [time_step * k for k in range(len(lines))]


def get_screen_size():
    from PyQt5.QtWidgets import QApplication
    app = QApplication([])
    screen_resolution = app.desktop().screenGeometry()
    return screen_resolution.width(), screen_resolution.height()


def createTimeCurve(data, label):

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


def create_3d_subplot(data, label):
    pyplot.figure(label + " 3D Path").add_subplot(projection='3d')
    pyplot.plot(data[0], data[1], data[2])


width, height = get_screen_size()
width, height = width//2, height//2
createTimeCurve(positions, "Position")
pyplot.get_current_fig_manager().window.setGeometry(0, 0, width, height)
createTimeCurve(speeds, "Speed")
pyplot.get_current_fig_manager().window.setGeometry(width, 0, width, height)
createTimeCurve(accelerations, "Acceleration")
pyplot.get_current_fig_manager().window.setGeometry(0, height, width, height)
create_3d_subplot(positions, "Position")
pyplot.get_current_fig_manager().window.setGeometry(width, height, width, height)

pyplot.show()

