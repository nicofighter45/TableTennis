list = []
with open("../visualisation/transfer_value.txt", "r") as file:
    lines = file.readlines()
    i = 0
    for line in lines:
        cut = line[:-2].split(";")
        list.append((float(cut[0]), float(cut[1]), float(cut[2]), float(cut[3])))
    print(list)