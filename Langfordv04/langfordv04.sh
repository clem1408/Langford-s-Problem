#!/bin/bash

# Vérification si l'exécutable existe
if [ -n $1 ]; then

    echo "Compilation avec g++"
    make
    echo "Exécution"
    ./langfordv04 > result.txt
    echo "Résultat sauvegardé dans result.txt"
    echo "Clean des fichiers"
    make clean

else
    echo "Erreur : Il manque le nombre de processus."
fi