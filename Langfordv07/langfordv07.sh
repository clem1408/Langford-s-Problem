#!/bin/bash

# Vérification si l'exécutable existe
if [ -n $1 ]; then

    echo "Compilation avec MPI"
    make
    echo "Exécution avec MPI"
    mpirun --use-hwthread-cpus --allow-run-as-root --oversubscribe -n $1 ./langfordv07 > result.txt
    echo "Résultat sauvegardé dans result.txt"
    echo "Clean des fichiers"
    make clean

else
    echo "Erreur : Il manque le nombre de processus."
fi