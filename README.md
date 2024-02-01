# Etude Cinématique d'une Balle de Ping Pong

## Motivations

Le tennis de table est un sport de balle où les échanges sont extrêmement rapides. Cela implique donc d'avoir de bons réflexes et une bonne lecture de la trajectoire de la balle. Il m'a donc semblé intéressant d'identifier les différentes forces s’exerçant sur la balle pour prévoir précisément sa trajectoire.

## Ancrage

Le tennis de table est une discipline sportive où les effets de balles sont au cœur de la dynamique de jeu rendant les trajectoires de balles difficiles à prévoir après rebond. L'étude de la mécanique du rebond et des frottements fluides est donc essentielle pour prévoir la trajectoire de balle.

## Mise en Cohérence des Objectifs du TIPE (MCOT)

### Positionnements thématiques

1. Informatique (Informatique pratique)

2. Physique (Mécanique)

3. Mathématiques (Analyse)


### Mots clés

| Français             | Anglais                  |
| -------------------- | ------------------------ |
| Tennis de Table      | Table Tennis             |
| Frottement fluide    | Fluid Friction           |
| Suivi                | Tracking                 |
| Courbe de Bézier     | B-Spline                 |
| Simulation Numérique | Computational Simulation |


### Bibliographie commentée (650 max)

Les différents effets que l’on peut appliquer à la balle de tennis de table (lifté, coupé, latéraux, … ) rendent son mouvement difficilement prévisible. Cela peut être expliqué par différents paramètres tels que les forces de frottements fluides, la force de Magnus [[1]](#Références) et la mécanique du rebond.

La balle étant assez légère et se déplaçant à une vitesse relativement élevée, il est nécessaire de prendre en compte la force de frottements exercée par l'air : c'est la force de trainée. Le nombre de Reynold permettant d'évaluer le type de force de frottement s’appliquant à la balle est ici de l’ordre de 100 mille. Cela place la balle dans un régime critique où la une force de frottement est proportionnelle au carré de la vitesse de la balle. [[2]](#Références)

Lorsque on donne de l'effet à la balle, elle accélère ou décélère le fluide l’entourant entraînant une dépression induisant une Force de Magnus, perpendiculaire à son vecteur vitesse. Ce type de mouvement est également observable avec une balle de golf [[1]](#Références). Cette force dépend du coefficient de viscosité du fluide, de la vitesse de déplacement et de la vitesse de rotation de la balle.

La mesure des coefficients liés à la force de trainée et à la force de Magnus est complexe. En effet, les entreprises gouvernementales telles que la NASA [[3]](#Références) ont utilisé des souffleries pour pouvoir mesurer ces coefficients avec précisions. Dans notre cas, on va prendre des mesures à l'aide d'un robot lanceur de balles et d'une caméra pour pouvoir enregistrer la position de la balle.

On calcule numériquement à l'aide d’un programme c++ développé pour l’occasion, la position de la balle dans l’image puis on en déduit sa position réelle, comme dans cette étude japonaise [[4]](#Références).

Ensuite, en approximant le mouvement de la balle par des courbes de Bézier [[5]](#Références), on déduit de la position, la vitesse et l'accélération.

En revanche, l’accélération et la vitesse de la balle de tennis de table admettent des discontinuités lors des rebonds. On observe une perte de vitesse entre avant et après le rebond caractérisée par un coefficient de restitution. Ce coefficient quantifie le rapport entre la vitesse initiale et la vitesse après le rebond, il dépend de différents paramètres : les matériaux de la balle et du sol ainsi que la rotation de la balle.

Pour simuler le déplacement de la balle de tennis de table, une résolution numérique des équations différentielles est nécessaire car ces dernières sont insolvables de façon explicite. On opte pour une méthode plus sophistiquée que la méthode d’Euler afin de gagner en précision, la méthode de Runge-Kutta 4 [[6]](#Références) codée en c++.

Enfin, on crée un petit programme qui vient, à partir d’une situation initiale, calculer grâce à la simulation, la trajectoire de la balle dans l’air avec précision.

### Problématique

Comment réaliser une simulation numérique réaliste du déplacement d'une balle de tennis de table en tenant compte des frottements de l’air, de l’effet Magnus et des rebonds ?

### Objectifs

Version Nicolas :

Réaliser une simulation numérique fidèle à la réalité du comportement des balles de tennis de table :
- Faire un suivi automatisé de la balle
  - Utiliser un robot pour lancer des balles de façon régulière
  - Utiliser c++ pour retrouver automatiquement la balle dans l'image à partir de sa colorimétrie
- Récupérer avec sa position différents coefficients
- Utiliser ces coefficients dans une simulation
  - Utiliser Runge-Kutta 4 pour résoudre les équations différentielles
  - Modéliser le rebond de la balle
  - Réaliser une représentation graphique de la simulation
---
Version Antoine :

Réaliser une simulation numérique fidèle à la réalité du comportement des balles de tennis de table :
- Faire un suivi automatisé de la balle
- Traiter des données de position, vitesse et accélération
  - En faire des représentations graphiques
  - Déterminer les différents coefficients caractérisant les forces s'exerçant sur la balle ( Magnus, frottement fluide )
  - Écrire un programme permettant d'ajuster les pointts expérimentaux par une fonction polynomiale continue
- Utiliser ces coefficients dans une simulation permettant de déterminer la trajectoire de la balle étant donné les conditions initiales


### Références
|Numéro|Auteur|Titre|Référence/URL|
|---|---|---|---|
|1|J. J. Thomson|The Dynalmics of a Golf Ball|[Nature 85 (1910), 251-257](sources/The-dynamics-of-a-golf-ball.pdf)|
|2|Roland Clift, John R. Grace, Martien E. Weber|Bubbles, Drops, and Particles|[1978](sources/Bubbles-drops-and-particles.epub)|
|3|Sighard F. Hoerner|Tests of Spheres with Reference to Reynolds Number, Turbulence, and Surface Roughness|[NACA-TM-777 (1935)](sources/NACA-test-of-spheres-with-reference-to-reynolds-number-turbulence-and-surface-roughness.pdf)|
|4|Akira Nakashima, Yuki Ogawa, Chunfang Liu, Yoshikazu Hayakawa|Robotic table tennis based on physical models of aerodynamics and rebounds|[Internation Conference on Robotics and Biomimetics, (2011), Karon Beach, Thailand](sources/Robotic-Table-Tennis-based-on-Physical-Models-of-Aerodynamics-and-Rebounds.pdf)|
|5|A. Valdescault, A. Batailly, S. Jones|Interpolation et approximation de données à l’aide de courbes et surfaces paramétriques de type B-splines|[Université McGill, Laboratoire de dynamique des structures et vibrations (2012)](sources/Splines-3D.pdf)|
|6|Jean-Pierre Demailly|Analyse numérique et équations différentielles|[EDP Sciences, 2016 (237-247)](sources/Analyse-numérique-et-équations-différentielles-Jean-Pierre-DEMAILLY.pdf)|
