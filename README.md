# SimulationProject(Claire Baskevitch, Tristan BESSAC)

###lancement de la simulation
./script_simul

###structure du programme

les fichiers echeancier.c et echeancier.h contiennent le code permettant de manipuler l'échéancier ainsi que les fonctions de calcul des temps d'attente et du 90-percentile, ces fichiers sont utilisés pour les 3 files.
le fichier file1.c contient le code utilisé uniquement pour simuler la première file.
idem pour le fichier file2.c avec la deuxième file.
idem pour le fichier file3.c avec la troisième file.   
Le fichier script.R contient du code en R pour générer des courbes du temps d'attente moyen et du 90-percentile en fonction de lambda
Le fichier script_simul est un script permettant de compiler le code pour les 3 files et de lancer les 3 simulations. ce fichier appelle aussi un script en R pour réaliser des graphes comparant les résultats des 3 files.
Le fichier lambda.txt contient les valeurs de lambda à utiliser pour les simulations
