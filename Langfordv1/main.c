
/**
 * Algorithme de Langford récursif
 * @author Clément Jourd'heuil
 * @version 1.0
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 12

inline bool est_valide(int *tab, int num, int pos) {
  // Si on sort du tableau 
  if (pos + num + 1 >= 2 * N)
    return false;
  // Si les emplacements sont disponibles on retoure true
  if (tab[pos] == 0 && tab[pos + num + 1] == 0)
    return true;
  return false;
}

void langford(int *tab, int num, int *solutions) {
  if (num == 0) {
    // Si on essaie de placer une zéro, 
    // c'est qu'on à une solution valide
    (*solutions)++;
    return;
  }

  for (int i = 0; i < 2 * N; i++) {
    if (est_valide(tab, num, i)) {
      tab[i] = num;
      tab[i + num + 1] = num;

      // Appel récursif
      langford(tab, num - 1, solutions);

      // Backtracking
      tab[i] = 0;
      tab[i + num + 1] = 0;
    }
  }
}

int main() {
    printf("===== Langford de %d ===== \n\n", N);

    clock_t start, stop;
    double CPU_time[5] = {0.0};
    double total_time = 0.0;

    if (N % 4 == 0 || N % 4 == 3) {
        for (int iteration = 0; iteration < 5; ++iteration) {
            int *tab = calloc(2 * N, sizeof(int));
            if (tab == NULL) {
                perror("Erreur d'allocation mémoire pour tab");
                return 1;
            }

            int solutions = 0;

            start = clock();
            langford(tab, N, &solutions);
            stop = clock();

            if (solutions == 0) {
                printf("Aucune solution trouvée pour n = %d à l'itération %d.\n", N, iteration + 1);
            } else {
                printf("Itération %d - Nombre total de solutions : %d\n", iteration + 1, solutions);
            }

            CPU_time[iteration] = ((double)(stop - start)) / CLOCKS_PER_SEC;
            printf("Itération %d - Temps : %lf secondes\n\n", iteration + 1, CPU_time[iteration]);

            total_time += CPU_time[iteration];
            free(tab);
        }

        printf("\nTemps moyen sur 5 exécutions : %lf secondes\n", total_time / 5.0);

    } else {
        printf("Aucune solution possible pour n = %d (car n %% 4 != 0 et n %% 4 != 3).\n", N);
    }

    return 0;
}