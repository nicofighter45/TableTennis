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
        positions[i].append(input_positions[i])
    for i in range(len(input_speeds)):
        speeds[i].append(input_speeds[i])
    for i in range(len(input_acceleration)):
        accelerations[i].append(input_acceleration[i])

time = [time_step * k for k in range(len(lines))]


def createTimeCurve(axs, data, label):
    axs.plot(time, data)
    axs.set_title(label + " = f(t)")
    #axs.yaxis.set_major_locator(ticker.MaxNLocator(nbins=5, prune='both'))
    axs.set(xlabel="Time (s)", ylabel= label + " (m)")

# Function to create a 3D subplot
def create_3d_subplot(ax, data):
    ax.plot(data[0], data[1], data[2], label='Path')
    ax.set_title('3D Path')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.legend()


fig, axs = pyplot.subplots(2, 2, gridspec_kw=dict(wspace=1, hspace=1), num="Position")
createTimeCurve(axs[0, 0], positions[0], "Position X")
createTimeCurve(axs[0, 1], positions[1], "Position Y")
createTimeCurve(axs[1, 0], positions[2], "Position Z")
createTimeCurve(axs[1, 1], positions[2], "Position Z")

pyplot.show()