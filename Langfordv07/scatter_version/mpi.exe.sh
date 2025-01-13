#!/bin/bash

# Vérification si l'exécutable existe
if [ -n $2 ]; then

    echo "Compilation avec MPI"
    make
    echo "Exécution avec MPI"
    mpirun --use-hwthread-cpus --allow-run-as-root --oversubscribe -n $2 ./$1 

else
    echo "Erreur : Il manque le nombre de processus ou l'exécutable $1 n'existe pas ou ."
fi