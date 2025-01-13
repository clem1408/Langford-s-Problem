#include "langford.h"
#include <stdlib.h>
#include <string.h>

langford_t *langford_create() {
  langford_t *langford = malloc(sizeof(langford_t));
  if (langford == NULL) {
    return NULL;
  }
  return langford;
}

void langford_destroy(langford_t *langford) { free(langford); }