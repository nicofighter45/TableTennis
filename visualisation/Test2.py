import matplotlib.pyplot as plt
import numpy as np

# Données d'exemple
x = np.linspace(0, 10, 100)
y = np.sin(x)

# Trouver les positions des maximums et minimums
max_positions = np.where(y == np.max(y))[0]
min_positions = np.where(y == np.min(y))[0]

# Tracer la courbe
plt.plot(x, y)

# Mettre en évidence les maximums et minimums avec des points rouges
plt.scatter(x[max_positions], y[max_positions], color='red', label='Maximums')
plt.scatter(x[min_positions], y[min_positions], color='blue', label='Minimums')

# Ajouter une légende
plt.legend()

# Afficher le graphique
plt.show()
