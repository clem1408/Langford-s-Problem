#include <array>
#include <ctime>
#include <iostream>
#include <unordered_set>
#include <vector>

#define N 12
#define DEPTH 4

using namespace std;

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
                          vector<array<int, N>> &solutions) {
  vector<int> indices(depth,
                      1); // Indices de chaque position initialisés à 1

  while (true) {
    // Vérifie si la combinaison actuelle est consistante (sans doublons)
    unordered_set<int> uniqueValues(indices.begin(), indices.end());
    bool isValid = ((int)uniqueValues.size() == depth);
    bool isValid2 = true;

    // Vérifie la contrainte de max_pos_tab pour chaque position
    for (int i = 0; i < depth && isValid; ++i) {
      if (indices[i] > max_pos_tab[N - depth + i]) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      array<int, 2 * N> general_tab = {0};

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
      // Initialise une solution remplie de zéros
      array<int, N> solution = {0};

      // Remplit seulement les éléments de l'indice N - DEPTH jusqu'à N
      for (int i = 0; i < depth; ++i) {
        solution[N - depth + i] = indices[i];
      }

      solutions.push_back(solution); // Ajoute la solution au tableau
      count++;                       // Incrémente le compteur de solutions
    }

    // Incrémente les indices de droite à gauche
    int position = depth - 1;
    while (position >= 0) {
      int maxVal =
          2 * N - 2 -
          (N - depth + position); // Limite supérieure pour chaque position
      if (indices[position] < maxVal) {
        indices[position]++;
        break;
      } else {
        indices[position] = 1;
        position--;
      }
    }

    // Si tous les indices sont réinitialisés, la génération est terminée
    if (position < 0)
      break;
  }
}

inline void show_langford(array<int, N> &langford,
                          array<int, 2 * N> &general_tab) {
  for (auto it = general_tab.begin(); it != general_tab.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;

  for (auto it = langford.begin(); it != langford.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;
}

inline void remove_pair(array<int, N> &langford, array<int, 2 * N> &general_tab,
                        int pair) {
  general_tab[langford[pair - 1] - 1] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(array<int, N> &langford, const vector<int> &max_pos_tab,
                      array<int, 2 * N> &general_tab, int pair) {
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

inline void init_general_tab(array<int, N> &langford,
                             array<int, 2 * N> &general_tab) {
  for (int i = N - DEPTH; i < N; i++) {
    general_tab[langford[i] - 1] = i + 1;
    general_tab[langford[i] + i + 1] = i + 1;
  }
}

inline void langford_algorithm(array<int, N> &langford,
                               const vector<int> &max_pos_tab, int &count2) {
  int level = N - DEPTH;
  array<int, 2 * N> general_tab = {0};

  init_general_tab(langford, general_tab);

  while (level <= N - DEPTH) {
    if (place_pair(langford, max_pos_tab, general_tab, level)) {
      if (level == 1) {
        count2++;
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
  int count = 0, count2 = 0;
  vector<int> max_pos_tab = generateMaxPosTab(N);
  vector<array<int, N>> solutions;

  double task_generation_time[5] = {0.0};
  double algorithm_time[5] = {0.0};
  double total_task_time = 0.0;
  double total_algorithm_time = 0.0;

  cout << "===== Langford de " << N << " avec une profondeur de " << DEPTH
       << " =====" << endl
       << endl;

  for (int i = 0; i < 5; i++) {
    count = 0;
    count2 = 0;
    solutions.clear();

    clock_t start_task, stop_task;
    clock_t start_alg, stop_alg;

    // Mesure du temps de génération des tâches
    start_task = clock();
    generateCombinations(DEPTH, max_pos_tab, count, solutions);
    stop_task = clock();
    task_generation_time[i] = double(stop_task - start_task) / CLOCKS_PER_SEC;

    cout << "Execution " << i + 1 << ":\n";
    cout << "  Nombre total de tâches: " << count << endl;

    // Mesure du temps d'exécution de l'algorithme principal
    start_alg = clock();
    for (auto &solution : solutions) {
      langford_algorithm(solution, max_pos_tab, count2);
    }
    stop_alg = clock();
    algorithm_time[i] = double(stop_alg - start_alg) / CLOCKS_PER_SEC;

    cout << "  Nombre total de solutions: " << count2 << endl;
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
