def multiply(M1: [[]], M2: [[]]) -> [[]]:
    n, m, l = len(M1), len(M1[0]), len(M2[0])
    if m != len(M2):
        raise Exception("The number of column of M1 must be the same as the number of line of M2")
    M = []
    for i in range(n):
        line = []
        for j in range(l):
            sum = 0
            for k in range(m):
                sum += M1[i][k]*M2[k][j]
            line.append(sum)
        M.append(line)
    return M