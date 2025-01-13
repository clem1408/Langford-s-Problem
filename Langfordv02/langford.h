#ifndef LANGFORD_H
#define LANGFORD_H

typedef struct langford {
  int *general_tab;
  int *pos_tab;
  int *max_pos_tab;
  int nbSolutions;
} langford_t;

langford_t *langford_create();
void langford_destroy(langford_t *langford);

#endif // LANGFORD_H