# M2 - Projet3D - Réalisation d'un Aquarium

Réalisation d'un aquarium simplifié avec graphisme poussé.
Nous nous concentrerons ici sur l'aspect graphique avec une simulation simplifié de l'eau.

## Documentation : 

https://www.flareden.fr/Aquarium3D/index.html

OR 

https://github.com/Flare00/M2-Projet3D-Aquarium/tree/main/documentation/html

## Build and Execution
```bash
# Build
cmake -Bbuild .
cmake --build build #Can add the option : --config Debug or --config Release

# Execution
./Aquarium.exe
```
> The executable and dynamics libraries are generated in the root folder.

## Inputs

Qwerty -- Azerty

- Space / LShift    : Move the camera up/down.
- W / S -- Z / S     : Move the camera forward/backward.
- A / D -- Q / D     : Move the camera left/right.
- Q / E -- A / E     : Rotate the camera on the roll axis.
- Right click drag  : Rotate the Camera on the Pitch and Yaw axis.

## Étapes du développement prévus

#### Étape 1 - Réaliser un environnement aquatique simple :
- [X] Faire une surface réagissant avec son environnement / collision
- [ ] Faire une sphère capable d'interagir avec l'eau et flotter / subir la gravité
- [X] Faire un système pour interagir avec l'environnement avec la souris

#### Étape 2 - Effet lumineux :
- [X] faire un système de lampe projeter (soleil / spot).
- [X] Caustics et/ou God Rays.
- [ ] Ombre projeté.
- [X] Réfraction/Réflexion GPU

#### Étape 3 - Ajout de vie :
- [X] Ajouter des poissons avec système de boids suivant une trajectoire / courant
- [ ] Ajouter des particules de bulles / planctons / autres
- [ ] Ajout de plante bougeant avec l'eau et les poissons.
- [ ] Ajout de bateau/sous-marin contrôlable.


#### Étape 4 - Ajout de sable :
- [ ] Rajouter des particules de sable réagissant à l'environnement / créant des particules.

#### Étape 5 - Ajout de décor et de sons : 
- [ ] Ajout des décors extérieur à l'environnement, pour plus de réalisme.
- [ ] Ajout de sons.

## Moteur de l'Aquarium - Pistes d'implémentation des modules

Réalisation d'un Moteur ECS / Semi-ECS afin de permettre une meilleure fléxibilité.

#### Module IA :
- [X] Poissons Boids (utilisé un nuage de points pour limiter le nombre de mesh 3D ? / utilisation d'un geometry shader pour appliquer le mesh3D).
- [ ] Déplacement par suivi de courbe / courants / aléatoire .
- [ ] Autre IA.

#### Module physique :
- [X] Simulation de surface aquatique.
- [ ] Collision surface / objet.
- [ ] Application de force autre sur la surface.
- [ ] Collision entre deux objets.
- [X] Différents types d'objets.
- [ ] Déplacement d'une algue.
- [ ] Déplacement du sable / particules.

#### Module Graphique : 
- [X] Affichage d'objet 3D
- [X] Lecture de fichier OBJ / STL
- [X] Lecture de texture 2D
- [X] Systeme de Framebuffer
- [ ] Lecture d'animations.
- [ ] Systeme de particules.
- [ ] Calcul d'ombre portée / Shadow Mapping.
- [X] Calcul de Caustics (et/ou God Rays).
- [ ] Tesselation Shader.
- [ ] Geometry Shader pour le particules system. (Not working)

#### Module Sons : 
- [ ] Faire des sons pour les poissons / eau / environnement

## Technologies et librairies qui seront utilisées

- C++
- OpenGL
- GLSL

- GLUT (https://www.opengl.org/resources/libraries/glut/)
- GLFW (https://www.glfw.org/)
- GLM (https://github.com/g-truc/glm)
- STB (https://github.com/nothings/stb)
- OpenFBX (https://github.com/nem0/OpenFBX)

## Inspirations :
  https://madebyevan.com/webgl-water/
