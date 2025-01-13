#include <iostream>
#include <omp.h>
#include <time.h>
#include <unordered_set>
#include <vector>

#define N 12
#define DEPTH 5

using namespace std;

// Génère le tableau des positions maximales
inline vector<int> generateMaxPosTab(int n) {
  vector<int> max_pos_tab(n);
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
void generateCombinations(int depth, const vector<int> &max_pos_tab, int &count,
                          vector<vector<int>> &solutions) {
  vector<int> indices(depth, 1);

  while (true) {
    unordered_set<int> uniqueValues(indices.begin(), indices.end());
    bool isValid = ((int)uniqueValues.size() == depth);
    bool isValid2 = true;

    for (int i = 0; i < depth && isValid; ++i) {
      if (indices[i] > max_pos_tab[N - depth + i]) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      vector<int> general_tab(2 * N, 0);
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
      vector<int> solution(N, 0);
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

inline void remove_pair(vector<int> &langford, vector<int> &general_tab,
                        int pair) {
  general_tab[langford[pair - 1] - 1] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(vector<int> &langford, const vector<int> &max_pos_tab,
                      vector<int> &general_tab, int pair) {
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

void init_general_tab(vector<int> &langford, vector<int> &general_tab) {
  for (int i = N - DEPTH; i < N; i++) {
    general_tab[langford[i] - 1] = i + 1;
    general_tab[langford[i] + i + 1] = i + 1;
  }
}

void langford_algorithm(vector<int> &langford, const vector<int> &max_pos_tab,
                        int &local_count) {
  int level = N - DEPTH;
  vector<int> general_tab(2 * N, 0);
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
  vector<int> max_pos_tab = generateMaxPosTab(N);
  vector<vector<int>> solutions;

  double task_generation_time[5] = {0.0};
  double algorithm_time[5] = {0.0};
  double total_task_time = 0.0;
  double total_algorithm_time = 0.0;

  cout << "===== Langford de " << N << " avec une profondeur de " << DEPTH
       << " =====" << endl
       << endl;

  for (int i = 0; i < 5; i++) {
    count = 0;
    total_count2 = 0;
    solutions.clear();

    double start_time, end_time;

    // Mesure du temps de génération des tâches
#ifdef _OPENMP
    start_time = omp_get_wtime();
#else
    start_time = clock();
#endif
    generateCombinations(DEPTH, max_pos_tab, count, solutions);
#ifdef _OPENMP
    end_time = omp_get_wtime();
    task_generation_time[i] = end_time - start_time;
#else
    end_time = clock();
    task_generation_time[i] = (end_time - start_time) / CLOCKS_PER_SEC;
#endif

    cout << "Execution " << i + 1 << ":\n";
    cout << "  Nombre total de tâches: " << count << endl;

    // Mesure du temps d'exécution de l'algorithme principal
#ifdef _OPENMP
    start_time = omp_get_wtime();
#else
    start_time = clock();
#endif

#pragma omp parallel for reduction(+ : total_count2)
    for (size_t j = 0; j < solutions.size(); j++) {
      int local_count = 0;
      langford_algorithm(solutions[j], max_pos_tab, local_count);
      total_count2 += local_count;
    }

#ifdef _OPENMP
    end_time = omp_get_wtime();
    algorithm_time[i] = end_time - start_time;
#else
    end_time = clock();
    algorithm_time[i] = (end_time - start_time) / CLOCKS_PER_SEC;
#endif

    cout << "  Nombre total de solutions: " << total_count2 << endl;
    cout << "  Temps de génération des tâches: " << task_generation_time[i]
         << " secondes\n";
    cout << "  Temps d'exécution de l'algorithme: " << algorithm_time[i]
         << " secondes\n\n";

    total_task_time += task_generation_time[i];
    total_algorithm_time += algorithm_time[i];
  }

  cout << "Temps moyen de génération des tâches: " << total_task_time / 5.0
       << " secondes\n";
  cout << "Temps moyen d'exécution de l'algorithme: "
       << total_algorithm_time / 5.0 << " secondes\n";

  return 0;
}
