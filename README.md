# M2 - Projet3D - Réalisation d'un Aquarium

Réalisation d'un aquarium simplifié avec graphisme poussé.
Nous nous concentrerons ici sur l'aspect graphique avec une simulation simplifié de l'eau.

## Build and Execution
```bash
cmake -Bbuild .
cmake --build build #Can add the option : --config Debug or --config Release

# For Linux :
./Aquarium
# For Windows :
./Aquarium.exe
```
> The executable and dynamics libraries are generated in the root folder.

## Étapes du développement prévus

#### Étape 1 - Réaliser un environnement aquatique simple :
- [ ] Faire une surface réagissant avec son environnement / collision
- [ ] Faire une sphère capable d'interagir avec l'eau et flotter / subir la gravité
- [ ] Faire un système pour interagir avec l'environnement avec la souris

#### Étape 2 - Effet lumineux : --- Minimum à implémenter ---
- [ ] faire un système de lampe projeter (soleil / spot)
- [ ] Caustics et God Rays
- [ ] Ombre projeté
- [ ] Utilisation du RayCast GPU ? (Réfraction/Réflexion)

#### Étape 3 - Ajout de vie :
- [ ] Ajouter des poissons avec système de boids suivant une trajectoire / courant
- [ ] Ajouter des particules de bulles / planctons / autres
- [ ] Ajout de plante bougeant avec l'eau et les poissons.
- [ ] Ajout de bateau contrôlable ? 

#### Étape 4 - Ajout de sable :
- [ ] Rajouter des particules de sable réagissant à l'environnement / créant des particules.

#### Étape 5 - Ajout de décor et de sons : 
- [ ] Ajout des décors extérieur à l'environnement, pour plus de réalisme.
- [ ] Ajout de sons.

## Moteur de l'Aquarium - Pistes d'implémentation des modules

Réalisation d'un Moteur ECS / Semi-ECS afin de permettre une meilleure fléxibilité.

#### Module IA :
- [ ] Poissons Boids (utilisé un nuage de points pour limiter le nombre de mesh 3D ? / utilisation d'un geometry shader pour appliquer le mesh3D)
- [ ] Déplacement par suivi de courbe / courants / aléatoire 
- [ ] Autre IA

#### Module physique :
- [ ] Simulation de surface aquatique
- [ ] Collision surface / objet
- [ ] Application de force autre sur la surface
- [ ] Collision entre deux objets
- [ ] différents types d'objets
- [ ] déplacement d'une algue
- [ ] déplacement du sable / particules

#### Module Graphique : 
- [ ] Lecture de fichier OBJ / STL
- [ ] Lecture d'animations ?
- [ ] Lecture de texture 2D
- [ ] Systeme de particules.
- [ ] Système de Framebuffer
- [ ] Calcul d'ombre portée / Shadow Mapping
- [ ] Calcul de Caustics et God Rays
- [ ] Affichage d'objet 3D
- [ ] Tesselation Shader
- [ ] Geometry Shader pour le particules system.

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
