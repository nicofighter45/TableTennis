import matplotlib.pyplot as pyplot
import curves_creator

time_step = 0.01

positions, speeds, time = [[], []], [[], []], []

lines = open(R"C:\Users\fagot\ShadowDrive\Documents\MP\TIPE\tracked-3.txt", "r").readlines()

for line in lines:
    input_positions = line.split(";")[:-1]
    for i in range(len(input_positions)):
        positions[i].append(3.85-float(input_positions[i])*2.65/1242)

time = [time_step * k for k in range(len(lines))]
speeds[0].append((positions[0][1] - positions[0][0]) / time_step)
speeds[1].append((positions[1][1] - positions[1][0]) / time_step)
for i in range(1, len(positions[0])):
    speeds[0].append((positions[0][i] - positions[0][i-1]) / time_step)
    speeds[1].append((positions[1][i] - positions[1][i-1]) / time_step)


def get_screen_size():
    from PyQt5.QtWidgets import QApplication
    app = QApplication([])
    screen_resolution = app.desktop().screenGeometry()
    return screen_resolution.width(), screen_resolution.height()


width, height = get_screen_size()

curves_creator.create1TimeCurve(positions[1], time, "Position Z (m)", "Temps (s)", 'blue', '.')
pyplot.get_current_fig_manager().window.setGeometry(0, 0, width, height)
curves_creator.create1TimeCurve(speeds[1], time, "Vitesse Z (m)", "Temps (s)", 'green', '.')
pyplot.get_current_fig_manager().window.setGeometry(0, 0, width, height)
curves_creator.create1TimeCurve(positions[1], positions[0], "Position Z (m)", "Position Y (m)", 'red', '.')
pyplot.get_current_fig_manager().window.setGeometry(0, 0, width, height)
pyplot.show()
