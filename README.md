<a href="https://colab.research.google.com/github/nicofighter45/TableTennis/blob/main/README.ipynb" target="_parent"><img src="https://colab.research.google.com/assets/colab-badge.svg" alt="Open In Colab"/></a>

# TIPE
Le but de notre TIPE et d'abord de simuler avec des valeurs donnés un lancer de balle de tennis de table
en prenant en compte
 - le poids de la balle
 - les frottements de l'air 
 - la force de Magnus 
---
Puis grâce à une observation de lancées de balle de tennis de table, on prend frame par frame la position de la balle pour enfin comparer ses valeurs avec des simulations de lancée possédant différentes valeurs de coefficient de trajectoire : en prenant celle qui s'approche le plus de la réalité on pourra approximer notre coefficient de frottement.
 

# Simulations





Pour les simultions, nous utiliserons un code en C++ ainsi nous aurons un calcul plus rapide.

---
Ensuite nous utiliserons une Méthode d'Euler avec un écart entre les valeurs d'une milli-seconde



## Bilan des forces

###Poids



$P = m\times g$


P : le poids

m : la masse 

g : la constante de gravition terrestre

###Frottement


$\overrightarrow{f} = \frac{1}{2} \times rho\times Cx \times S \times \lvert \lvert \overrightarrow{v} \rvert \rvert \times \overrightarrow{v}$

f = Force de frottement fluide

Cx = coefficient de frottement fluide

S = surface de frottement

rho = masse volumique de l'air

$\overrightarrow{v}$ = vecteur vitesse

###Force de magnus 

$\overrightarrow{Fm} = S0 \times \overrightarrow{v}$

S0 = Surface de frottement de l'air

$\overrightarrow{v}$ = vecteur vitesse

## Poid seul


Une simulation simple


```python
#pragma once

#include <iostream>
#include "../tools/vector.h"
#include "../tools/constants.h"
#include <cmath>

using namespace std;

static Vect3D getVector(double time, Vect3D initialPosition, Vect3D initialSpeed)
{
    return Vect3D(initialSpeed.getX() * time + initialPosition.getX(),
                  initialSpeed.getY() * time + initialPosition.getY(),
                  -gravity / 2 * sq(time) + initialSpeed.getZ() * time + initialPosition.getZ());
}

vector<Vect3D> runGravitySimulation(Vect3D initialPosition, Vect3D initialSpeed)
{
    vector<Vect3D> positions(1, initialPosition);
    double const final_time = (sqrt(sq(initialSpeed.getZ()) + 2 * gravity * initialPosition.getZ()) - initialSpeed.getZ()) / 2;

    double const interval(1 / spacing_per_second);

    for (double time = interval; time < final_time; time += interval)
    {
        positions.push_back(getVector(time, initialPosition, initialSpeed));
    }

    Vect3D position(initialPosition), speed(initialSpeed), acceleration(0, 0, -9.81);
    cout << "Time = " << final_time << "s" << endl
         << "Position:" << endl
         << position.getValue() << endl
         << "Speed:" << endl
         << speed.getValue() << endl
         << "Acceleration:" << endl
         << acceleration.getValue() << endl;

    return positions;
}

Vect3D getGravitySimulationEnd(Vect3D initialPosition, Vect3D initialSpeed)
{
    double const time = (sqrt(sq(initialSpeed.getZ()) + 2 * gravity * initialPosition.getZ()) - initialSpeed.getZ()) / 2;
    return getVector(time, initialPosition, initialSpeed);
}
```

##Frottement + Poid


```python
#pragma once

#include <string>
#include <iostream>
#include "../tools/vector.h"
#include "../tools/constants.h"
#include <vector>

using namespace std;

vector<Vect3D> runFrictionSimulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const e((rho * Cx * surface) / (2 * mass));

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), speed_size(0);

    vector<Vect3D> positions(1, position);

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -e * speed_size;
        acceleration.setValue(coefficient * speed.getX(), coefficient * speed.getY(), coefficient * speed.getZ() - gravity);
        speed.addValue(old_acceleration.getX() * interval, old_acceleration.getY() * interval, old_acceleration.getZ() * interval);
        position.addValue(old_speed.getX() * interval, old_speed.getY() * interval, old_speed.getZ() * interval);

        positions.push_back(position);
        old_speed = speed;
        old_acceleration = acceleration;

        frame++;
    }

    cout << "Time = " << frame / spacing_per_second << "s" << endl
         << "Position:" << endl
         << position.getValue() << endl
         << "Speed:" << endl
         << speed.getValue() << endl
         << "Acceleration:" << endl
         << acceleration.getValue() << endl;

    return positions;
}

```

##Frottement + Poid + Effet Magnus


```python
#pragma once

#include <string>
#include <iostream>
#include "../tools/vector.h"
#include "../tools/constants.h"

using namespace std;

vector<Vect3D> runMagnusSimulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const rotation(1);
    Vect3D const rotationVector(-1, 0, 0);
    double const S0(1);

    double const e((rho * Cx * surface) / (2 * mass));
    double const f(S0 * rotation / mass);

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), coefficient_magnus(0), speed_size(0);

    vector<Vect3D> positions(1, initialPosition);

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -e * speed_size;
        coefficient_magnus = f * speed_size;
        Vect3D magnusForce = vectorialProduct(speed, rotationVector, coefficient_magnus);
        acceleration.setValue(coefficient * speed.getX() + magnusForce.getX(), coefficient * speed.getY() + magnusForce.getY(),
                              coefficient * speed.getZ() - gravity + magnusForce.getZ());
        speed.addValue(old_acceleration.getX() * interval, old_acceleration.getY() * interval, old_acceleration.getZ() * interval);
        position.addValue(old_speed.getX() * interval, old_speed.getY() * interval, old_speed.getZ() * interval);

        old_speed = speed;
        old_acceleration = acceleration;

        frame++;

        positions.push_back(position);
    }

    cout << "Time = " << frame / spacing_per_second << "s" << endl
         << "Position:" << endl
         << position.getValue() << endl
         << "Speed:" << endl
         << speed.getValue() << endl
         << "Acceleration:" << endl
         << acceleration.getValue() << endl;

    return positions;
}
```

#Acquisition des points

On utilisera une bibliotheque C++ nous permettant de tracker des couleurs : nous prendrons dans notre expérience des balles de tennis de table de couleur orange. elle s'appelle OpenCV.


```python

```

Du à la déformation de l'image par la caméra l'on se doit de recalculer les différentes valeurs des points expirémentaux pour avoir une meilleur précision sur leurs valeurs


```python

```

#Traitement


Pour déterminer le coefficient de frottement fluide et le coefficient de Magnus, à partir des valeurs obtenues par l'Acquisition, l'on fait des simulation avec des coefficients de Magnus et de frottement compris dans un intervalle donné puis l'on tire la meilleur approximarion des coefficients possible puis à partir de la moyenne de chaque point avec son homologue expérimental l'on forme un écart type à partir

### Moyenne de liste


```python

```

##Variance de liste


```python

```

##Écart type de liste


```python

```

## Calcul l'écart type moyen d'une simulation au valeurs expérimentale


```python

```

## Comparateur de valeur et output de la valeur final de coefficient de frottement


```python

```

#Modélisation 3D de mouvement finale de balle de tennis de la simultion final avec les coefficients finaux (en python)




```python
list = []
with open("../visualisation/transfer_value.txt", "r") as file:
    lines = file.readlines()
    i = 0
    for line in lines:
        cut = line[:-2].split(";")
        list.append((float(cut[0]), float(cut[1]), float(cut[2]), float(cut[3])))
    print(list)
```

#Autre

###Constante

Code nous permettant de garder toutes nos constantes


```python
#pragma once

double const rho(1.0);
double const Cx(0.5);
double const diametre_in_cm(4.0);
double const mass_in_gram(2.7);
double const gravity(9.81);
double const spacing_per_second(1000);

double const pi(3.141592653589793);
double const surface(diametre_in_cm *diametre_in_cm * 0.0001 * pi * 2 / 3);
double const mass(mass_in_gram * 0.001);
```

##Objet3D
Nouvelle classe pour répondre a des attentes précise sur nos vecteurs 3D ( ex : produit vectoriel )

###Création de la classe et création de la fonction carré


```python
#pragma once

#include <string>
#include <cmath>
#include <vector>

using namespace std;

double sq(double number)
{
    return number * number;
}

class Vect3D
{

private:
    double x;
    double y;
    double z;
```

###fonction de la classe permettant d'obtenir les valeurs de chaque coordonnée d'un vecteur3D


```python
public:
    double getX() { return x; };
    double getY() { return y; };
    double getZ() { return z; };
```

###Constructeur de l'objet


```python
    Vect3D(double ix, double iy, double iz)
    {
        x = ix;
        y = iy;
        z = iz;
    }
```

###Permet de créer un Vect3D à partir d'un string


```python
   Vect3D(string input)
    {
        vector<string> inputs;
        string current("");
        for (int i = 0; i < input.length(); i++)
        {
            char c = input[i];
            if (c == ' ')
            {
                inputs.push_back(current);
                current = "";
            }
            else
            {
                current += c;
            }
        }
        inputs.push_back(current);
        if (inputs.size() != 3)
        {
            return;
        }
        x = stod(inputs[0]);
        y = stod(inputs[1]);
        z = stod(inputs[2]);
    }
```

### Ajouter un triplet à un Vect3D


```python
    void addValue(double xt = 0, double yt = 0, double zt = 0)
    {
        x += xt;
        y += yt;
        z += zt;
    }
```

### Transforme un vecteur en a autre


```python
    void setValue(double xt, double yt, double zt)
    {
        x = xt;
        y = yt;
        z = zt;
    }
```

### size() : calcul de la norme du vecteur 3D


```python
    double size()
    {
        return sqrt(sq(x) + sq(y) + sq(z));
    }
```

### .getValues() : output le triplet du Vect3D


```python
    string getValue()
    {
        return "X = " + to_string(x) + "\nY = " + to_string(y) + "\nZ = " + to_string(z);
    }
};
```

### calcul de produit vectorielle avec une norme précise.


```python
Vect3D vectorialProduct(Vect3D u, Vect3D v, double size)
{
    double x(u.getY() * v.getZ() - u.getZ() * v.getY()), y(v.getX() * u.getZ() - u.getX() * v.getZ()), z(u.getX() * v.getY() - u.getY() * v.getX());
    size /= sqrt(sq(x) + sq(y) + sq(z));
    return Vect3D(x * size, y * size, z * size);
}
```

## Exportation de fichier de python vers C++ ( pour la modélisation )


```python
#pragma once

#include <vector>
#include <iostream>
#include <Python.h>
#include "vector.h"
#include "constants.h"

using namespace std;;

void print2DGraph(vector<Vect3D> positions)
{

    Py_Initialize();

    PyObject *pModule = PyImport_ImportModule("2dgraph");
    if (!pModule)
    {
        PyErr_Print();
        exit(1);
    }
    PyObject *pClass = PyObject_GetAttrString(pModule, "2dgraph.py");
    if (!pClass)
    {
        PyErr_Print();
        exit(1);
    }
    PyObject *pInit = PyObject_GetAttrString(pClass, "graph");
    if (!pInit || !PyCallable_Check(pInit))
    {
        PyErr_Print();
        exit(1);
    }

    PyObject *pList = PyList_New(4);
    for (int time = 0; time < positions.size(); time++)
    {
        PyObject *pTuple = PyTuple_New(4);
        PyTuple_SetItem(pTuple, 0, PyFloat_FromDouble(time / spacing_per_second));
        PyTuple_SetItem(pTuple, 1, PyFloat_FromDouble(positions[time].getX()));
        PyTuple_SetItem(pTuple, 2, PyFloat_FromDouble(positions[time].getY()));
        PyTuple_SetItem(pTuple, 3, PyFloat_FromDouble(positions[time].getZ()));
        PyList_Append(pList, pTuple);
    }

    PyObject *pArgs = PyTuple_Pack(positions.size(), pList);
    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    if (!pInstance)
    {
        PyErr_Print();
        exit(1);
    }
    Py_XDECREF(pInstance);
    Py_XDECREF(pArgs);
    Py_XDECREF(pList);
    Py_XDECREF(pClass);
    Py_DECREF(pModule);
    Py_Finalize();
}
```

## Fichier texte 


```python

Welcome to tennis table c++ simulation !

Here are the available simulations, please enter the corresponding number :
1) Without friction
2) With friction
3) Magnus effect


Nice, you've chosen simulation {type}!


Enter initial position Values: (Ex: 0 0 0.15)


Enter initial speed Values: (Ex: 0 4 1)


Solving differential equation
```
