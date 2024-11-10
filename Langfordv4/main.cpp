#include <iostream>
#include <omp.h>
#include <time.h>
#include <unordered_set>
#include <vector>

<<<<<<< HEAD
#define N 16
#define DEPTH 3
=======
#define N 15
#define DEPTH 1
>>>>>>> cc7eb6160841baafd80b0e44f56113ae711be90b

// Génère le tableau des positions maximales
inline std::vector<int> generateMaxPosTab(int n) {
  std::vector<int> max_pos_tab(n);
  int max_pos = 2 * n - 2;

  for (int i = 0; i < n; i++) {
    max_pos_tab[i] = max_pos--;
  }

  if (n % 2 == 0) {
    max_pos_tab[n - 2] /= 2;
  } else {
    max_pos_tab[n - 1] /= 2;
  }

  return max_pos_tab;
}

// Génère les combinaisons possibles jusqu'à une profondeur donnée et les stocke
void generateCombinations(int depth, const std::vector<int> &max_pos_tab,
                          int &count,
                          std::vector<std::vector<int>> &solutions) {
  std::vector<int> indices(depth, 1);

  while (true) {
    std::unordered_set<int> uniqueValues(indices.begin(), indices.end());
    bool isValid = ((int)uniqueValues.size() == depth);
    bool isValid2 = true;

    for (int i = 0; i < depth && isValid; ++i) {
      if (indices[i] > max_pos_tab[N - depth + i]) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      std::vector<int> general_tab(2 * N, 0);
      for (int i = N - DEPTH; i < N; ++i) {
        if (general_tab[indices[i - (N - DEPTH)] - 1] == 0 &&
            general_tab[indices[i - (N - DEPTH)] + i + 1] == 0) {
          general_tab[indices[i - (N - DEPTH)] - 1] = i + 1;
          general_tab[indices[i - (N - DEPTH)] + i + 1] = i + 1;
        } else {
          isValid2 = false;
          break;
        }
      }
    }

    if (isValid && isValid2) {
      std::vector<int> solution(N, 0);
      for (int i = 0; i < depth; ++i) {
        solution[N - depth + i] = indices[i];
      }

#pragma omp critical
      {
        solutions.push_back(solution);
        count++;
      }
    }

    int position = depth - 1;
    while (position >= 0) {
      int maxVal = 2 * N - 2 - (N - depth + position);
      if (indices[position] < maxVal) {
        indices[position]++;
        break;
      } else {
        indices[position] = 1;
        position--;
      }
    }

    if (position < 0)
      break;
  }
}

inline void remove_pair(std::vector<int> &langford,
                        std::vector<int> &general_tab, int pair) {
  general_tab[langford[pair - 1] - 1] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(std::vector<int> &langford,
                      const std::vector<int> &max_pos_tab,
                      std::vector<int> &general_tab, int pair) {
  int i = (langford[pair - 1] != 0) ? langford[pair - 1] + 1 : 1;

  if (langford[pair - 1] != 0) {
    remove_pair(langford, general_tab, pair);
  }

  for (; i <= max_pos_tab[pair - 1]; i++) {
    int second_pos = i + pair;
    if (general_tab[i - 1] == 0 && general_tab[second_pos] == 0) {
      general_tab[i - 1] = pair;
      general_tab[second_pos] = pair;
      langford[pair - 1] = i;
      return 1;
    }
  }

  return 0;
}

void init_general_tab(std::vector<int> &langford,
                      std::vector<int> &general_tab) {
  for (int i = N - DEPTH; i < N; i++) {
    general_tab[langford[i] - 1] = i + 1;
    general_tab[langford[i] + i + 1] = i + 1;
  }
}

void langford_algorithm(std::vector<int> &langford,
                        const std::vector<int> &max_pos_tab, int &local_count) {
  int level = N - DEPTH;
  std::vector<int> general_tab(2 * N, 0);
  init_general_tab(langford, general_tab);

  while (level <= N - DEPTH) {
    if (place_pair(langford, max_pos_tab, general_tab, level)) {
      if (level == 1) {
        local_count++; // Incrémentation de la variable locale
        remove_pair(langford, general_tab, 1);
      } else {
        level--;
        continue;
      }
    }
    level++;
  }
}

int main() {
  int count = 0;
  int total_count2 = 0;
  std::vector<int> max_pos_tab = generateMaxPosTab(N);
  std::vector<std::vector<int>> solutions;

  double start_time, end_time;
#ifdef _OPENMP
  start_time = omp_get_wtime();
#else
  start_time = clock();
#endif

  generateCombinations(DEPTH, max_pos_tab, count, solutions);

#pragma omp parallel for reduction(+ : total_count2)
  for (size_t i = 0; i < solutions.size(); i++) {
    int local_count = 0; // Variable locale pour chaque thread
    langford_algorithm(solutions[i], max_pos_tab, local_count);
    total_count2 += local_count; // Additionne local_count au compteur global
  }

#ifdef _OPENMP
  end_time = omp_get_wtime();
  printf("Time : %lf seconds (parallel)\n", end_time - start_time);
#else
  end_time = clock();
  printf("Time : %lf seconds (sequential)\n",
         (end_time - start_time) / CLOCKS_PER_SEC);
#endif

  std::cout << "N: " << N << std::endl;
  std::cout << "DEPTH: " << DEPTH << std::endl;
  std::cout << "Nombre total de tâches: " << count << std::endl;
  std::cout << "Nombre total de solutions: " << total_count2 << std::endl;

  return 0;
}
