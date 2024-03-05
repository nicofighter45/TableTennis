import matplotlib.pyplot as pyplot
import curves_creator

time_step = 0.001

positions, speeds, rotations, accelerations, time = \
    ([[], [], []], [[], [], []], [[], [], []], [[], [], []], [])

lines = open("D:/TableTennis/output/simulations/transfer.txt", "r").readlines()
for line in lines:
    line.removesuffix("\n")
    inputs = line.split(";;")
    input_positions = inputs[0].split(";")
    input_speeds = inputs[1].split(";")
    input_rotations = inputs[2].split(";")
    input_acceleration = inputs[3].split(";")
    for i in range(len(input_positions)):
        positions[i].append(float(input_positions[i]))
    for i in range(len(input_speeds)):
        speeds[i].append(float(input_speeds[i]))
    for i in range(len(input_rotations)):
        rotations[i].append(float(input_rotations[i]))
    for i in range(len(input_acceleration)):
        accelerations[i].append(float(input_acceleration[i]))

time = [time_step * k for k in range(len(lines))]


def get_screen_size():
    from PyQt5.QtWidgets import QApplication
    app = QApplication([])
    screen_resolution = app.desktop().screenGeometry()
    return screen_resolution.width(), screen_resolution.height()


width, height = get_screen_size()
width, height = width // 4, height // 4
space = 10
height -= space
curves_creator.createTimeCurve(positions, "Position", time)
pyplot.get_current_fig_manager().window.setGeometry(0, 0, width, height)
curves_creator.createTimeCurve(speeds, "Speed", time)
pyplot.get_current_fig_manager().window.setGeometry(0, height + space, width, height)
curves_creator.createTimeCurve(rotations, "Rotation", time)
pyplot.get_current_fig_manager().window.setGeometry(0, 2*height + 2*space, width, height)
curves_creator.createTimeCurve(accelerations, "Acceleration", time)
pyplot.get_current_fig_manager().window.setGeometry(0, 3*height + 3*space, width, height)
point_delta = 10
animation = curves_creator.create_3d_subplot((positions[0][::point_delta], positions[1][::point_delta],
                         positions[2][::point_delta]), "Position")
pyplot.get_current_fig_manager().window.setGeometry(width, 2*space, width*2, height*3)

pyplot.show()
