/**
 * Algorithme de Langford
 * @author Clément Jourd'heuil
 * @version 2.0
 */

#include "langford.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h>

#define N 15

void init(langford_t *langford)
{
  langford->general_tab = calloc(2 * N + 1, sizeof(int));

  langford->pos_tab = calloc(N + 1, sizeof(int));

  langford->max_pos_tab = malloc((N + 1) * sizeof(int));

  langford->nbSolutions = 0;

  int max_pos = 2 * N - 2;

  for (int i = 1; i <= N; i++)
  {
    langford->max_pos_tab[i] = max_pos--;
  }

  if (N % 2 == 0)
  {
    langford->max_pos_tab[N - 1] /= (int)2;
  }
  else
  {
    langford->max_pos_tab[N] /= (int)2;
  }
}

void show_langford(langford_t *langford)
{
  printf("Langford's array positions : ");
  for (int i = 1; i < 2 * N + 1; i++)
  {
    printf("%d ", langford->general_tab[i]);
  }
  printf("\n");
  printf("Array of pairs positions : ");
  for (int i = 1; i < N + 1; i++)
  {
    printf("%d ", langford->pos_tab[i]);
  }
  printf("\n");
  printf("Array of maximum positions : ");
  for (int i = 1; i < N + 1; i++)
  {
    printf("%d ", langford->max_pos_tab[i]);
  }
  printf("\n");
}

inline void remove_pair(langford_t *langford, int pair)
{
  langford->general_tab[langford->pos_tab[pair]] = 0;
  langford->general_tab[langford->pos_tab[pair] + pair + 1] = 0;
  langford->pos_tab[pair] = 0;
}

int place_pair(langford_t *restrict langford, int pair)
{
  // Initialisation de i, selon si la paire est déjà placé ou non
  int i = (langford->pos_tab[pair] != 0) ? langford->pos_tab[pair] + 1 : 1;

  // Si la paire est déjà placée, on la retire
  if (langford->pos_tab[pair] != 0)
  {
    remove_pair(langford, pair);
  }

  // Parcourir les positions possibles pour la paire
  for (; i <= langford->max_pos_tab[pair]; i++)
  {
    int second_pos = i + pair + 1; // Pré-calculer la seconde position
    if (langford->general_tab[i] == 0 && langford->general_tab[second_pos] == 0)
    {
      langford->general_tab[i] = pair;
      langford->general_tab[second_pos] = pair;
      langford->pos_tab[pair] = i;
      return 1; // Placement valide
    }
  }

  return 0; // Aucun emplacement valide trouvé
}

void langford_algorithm(langford_t *langford)
{
  int level = N;

  while (level <= N)
  {
    if (place_pair(langford, level)) // Si placement de la paire valide
    {
      if (level == 1) // Si on arrive à placer le 1 (combinaison valide)
      {
        langford->nbSolutions++;
        remove_pair(langford, 1);
      }
      else // Sinon on continue de descendre
      {
        level--;
        continue;
      }
    }
    level++; // On remonte
  }
}

int main(/*int argc, char *argv[]*/)
{
  clock_t start, stop;

  langford_t *langford = langford_create();

  init(langford);

  start = clock();

  langford_algorithm(langford);

  stop = clock();

  double CPU_time = ((double)(stop - start)) / CLOCKS_PER_SEC;

  printf("Number total of solutions : %d\n", langford->nbSolutions);

  printf("Time : %lf seconds\n", CPU_time);

  langford_destroy(langford);

  return 0;
}
