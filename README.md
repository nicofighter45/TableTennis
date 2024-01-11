# Etude Cinématique d'une Balle de Ping Pong

## Motivations

Lorsque les athlètes des Jeux Olympiques s'entraînent, ils ont besoin de quelqu'un pour renvoyer la balle. Ainsi prédire la trajectoire de la balle dans toutes les situations possibles s'avère vitale pour la réalisation d'un jeu vidéo ou même d'un robot humanoïde jouant au tennis de table.

## Ancrage

D'un côté le tennis de table en lui-même est un sport, et de l'autre la création d'une simulation numérique en relation avec ce sport peut être utile dans la création d'un jeu vidéo.

## Mise en Cohérence des Objectifs du TIPE (MCOT)

### Positionnements thématiques

Informatique (Informatique pratique, Traitement d'images, Simulation)

Mathématique (Analyse)

Physique (Mécanique)


### Mots clés

| Français             | Anglais                  |
| -------------------- | ------------------------ |
| Tennis de Table      | Table Tennis             |
| Frottement fluide    | Fluid Friction           |
| Suivi                | Tracking                 |
| B-spline             | B-Spline                 |
| Simulation Numérique | Computational Simulation |


### Bibliographie commentée (650 max)

Les différents effets que l’on peut appliquer à la balle de Tennis de Table (lifté, coupé, latéraux, … ) rendent son mouvement imprévisible car il est très contre-intuitif. Cela peut être expliqué par différents paramètres tels que les forces de frottements fluides, la force de Magnus [[1]](#Références) et la mécanique du rebond.

Lorsque la balle tourne sur elle-même, elle accélère ou décélère le fluide l’entourant entraînant une dépression induisant une Force de Magnus [[1]](#Références) à l’image de la balle de golf. Cette force est proportionnelle au coefficient de viscosité du fluide, à la vitesse de déplacement et la vitesse de rotation de la balle.

La balle étant assez légère et se déplaçant à une vitesse relativement rapide négliger les force de frottement fluide n’est pas possible. Le coefficient de Reynold qui nous donne le type de force de frottement s’appliquant à la balle est de l’ordre de XXX plaçant la balle dans un régime Supercritique : c’est à dire une force de frottement proportionnel au carré de la vitesse de la balle. [[2]](#Références)

Le calcul des coefficients liée à la force de Magnus et au frottement fluide ne sont pas simple 

Ensuite la balle de tennis de table semble admettre des discontinuités d’accélération et de vitesse lors des rebonds. De plus, on observe une perte de vitesse entre avant et après le rebond caractérisée par un coefficient de restitution [[7]](#Références). Ce coefficient quantifie le rapport entre la vitesse initiale et la vitesse d’après le rebond, il dépend de différents paramètres : les matériaux de la balle et du sol et la rotation de la balle.

Pour simuler le déplacement de la balle de tennis de table, l’on doit passer par l’approximation de son mouvement à partir des équations différentielles caractérisant son mouvement car elle n’est pas linéaire. Ainsi, à la place d’une méthode d’Euler classique, pour nos simulations la méthodes de Runge-Kutta[[6]](#Références).

Nous avons cherché à approximer le mouvement de la balle de tennis de table pour tout d’abord une vitesse et une accélération s’approchant de la réalité et pour acquérir plus de point avec les nombre de point fini que l’on avait déjà ainsi les Spline 2D [[5]](#Références) ont permis cela.

- La mécanique 
- effet Magnus
- effet de frottement fluide
- Physique du rebond
- B-spline
- Traitement d’image

### Problématique

Comment réaliser une simulation numérique réaliste du déplacement d'une balle de tennis de table en tenant compte des frottements de l’air, de l’effet Magnus et des rebonds ?

### Objectifs

Nico
Réaliser une simulation numérique fidèle à la réalité du comportement des balles de tennis de table. Pour ce faire, il faudra réaliser un suivi automatisé de la balle en utilisant un programme coder en c++ qui utilise la colorimétrie de l’image pour repérer la balle dans celle-ci, récupérer avec sa position différents coefficients puis les utiliser pour calculer à partir de n’importe quelle situation initiale grâce à la méthode de Runge-Kutta 4, le déplacement de la balle dans l’air et après son sur la table rebond.

Anto
Réaliser une simulation numérique fidèle à la réalité du comportement des balles de tennis de table. Pour ce faire, il faudra réaliser un suivi automatisé de la balle, récupérer avec sa position différents coefficients <insères tes trucs> puis les utiliser pour calculer le déplacement de la balle.

### Références
1. [The Dynamics of a Golf Ball 1 | Nature](sources/The-dynamics-of-a-golf-ball.pdf)
2. [Bubbles, Drops, and Particles Roland Clift, John R. Grace, Martien E. Weber](sources/Bubbles-drops-and-particles.epub)
3. [Robotic Table Tennis based on Physical Models of Aerodynamics and Rebounds, Akira Nakashima, Yuki Ogawa, Chunfang Liu and Yoshikazu Hayakawa, International Conference on Robotics and Biomimetics, December 7-11, 2011, Phuket, Thailand](sources/Robotic-Table-Tennis-based-on-Physical-Models-of-Aerodynamics-and-Rebounds.pdf)
4. [NACA test of spheres with reference to reynolds number, turbulence, and surface roughness](sources/NACA-test-of-spheres-with-reference-to-reynolds-number-turbulence-and-surface-roughness.pdf)
5. [Interpolation et approximation de données à l’aide de courbes et surfaces paramétriques de type B-splines](sources/Splines-3D.pdf)
6. [Analyse numérique et équations différentielles, Jean-Pierre Demailly](sources/Analyse-numérique-et-équations-différentielles-Jean-Pierre-DEMAILLY.pdf)
7. [Tennis de table : physique du rebond](sources/Tennis-de-table-physique-du-rebond.pdf)
