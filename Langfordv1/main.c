/**
 * Algorithme de Langford
 * @author Clément Jourd'heuil
 * @version 1.0
 */

#include "langford.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h>

#define N 12

void init(langford_t *langford)
{
  langford->general_tab = malloc((2 * N + 1) * sizeof(int));
  langford->pos_tab = malloc((N + 1) * sizeof(int));
  langford->max_pos_tab = malloc((N + 1) * sizeof(int));
  for (int i = 1; i < 2 * N + 1; i++)
  {
    langford->general_tab[i] = 0;
  }
  for (int i = 1; i < N + 1; i++)
  {
    langford->pos_tab[i] = 0;
  }
  langford->nbSolutions = 0;
  int max_pos = 2 * N - 2;
  for (int i = 1; i < N + 1; i++)
  {
    langford->max_pos_tab[i] = max_pos;
    max_pos--;
  }
  if (N % 2 == 0)
  {
    langford->max_pos_tab[N - 1] = langford->max_pos_tab[N - 1] / 2;
  }
  else
  {
    langford->max_pos_tab[N] = langford->max_pos_tab[N] / 2;
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

void remove_pair(langford_t *langford, int pair)
{
  langford->general_tab[langford->pos_tab[pair]] = 0;
  langford->general_tab[langford->pos_tab[pair] + pair + 1] = 0;
  langford->pos_tab[pair] = 0;
}

int place_pair(langford_t *langford, int pair)
{
  int i;
  if (langford->pos_tab[pair] != 0)
  {
    i = langford->pos_tab[pair] + 1;
    remove_pair(langford, pair);
  }
  else
  {
    i = 1;
  }
  while (langford->general_tab[i] != 0 || langford->general_tab[i + pair + 1] != 0)
    i++;
  if (i <= langford->max_pos_tab[pair])
  {
    langford->general_tab[i] = pair;
    langford->general_tab[i + pair + 1] = pair;
    langford->pos_tab[pair] = i;
    return 1;
  }
  return 0;
}

void langford_algorithm(langford_t *langford)
{
  int level = N;
  while (level != N + 1)
  {
    if (place_pair(langford, level)) // Si placement paire valide
    {
      if (level == 1) // Si on vient de trouver une solution
      {
        remove_pair(langford, 1);
        langford->nbSolutions++;
        level++;
      }
      else // Sinon on continue de descendre
      {
        level--;
      }
    }
    else // Si placement de paire pas valide on remonte
    {
      level++;
    }
  }
}

int main(/*int argc, char *argv[]*/)
{
  // Variable pour calculer le temps d'execution
  clock_t start, stop;
  double total, CPU_time;
  langford_t *langford = langford_create();
  init(langford);
  start = clock();
  langford_algorithm(langford);
  stop = clock();
  total = (double)stop - start;
  CPU_time = total / CLOCKS_PER_SEC;
  printf("Number total of solutions : %d\n", langford->nbSolutions);
  printf("Time : %lf seconds\n", CPU_time);
  langford_destroy(langford);
  return 0;
}