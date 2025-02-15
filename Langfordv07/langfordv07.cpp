#include <array>
#include <climits>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <unordered_set>
#include <vector>

#define N 12
#define DEPTH 3

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
  vector<int> indices(depth, 1); // Indices de chaque position initialisés à 1

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
                               const vector<int> &max_pos_tab,
                               int &count_local) {
  int level = N - DEPTH;
  array<int, 2 * N> general_tab = {0};

  init_general_tab(langford, general_tab);

  while (level <= N - DEPTH) {
    if (place_pair(langford, max_pos_tab, general_tab, level)) {
      if (level == 1) {
        count_local++;
        remove_pair(langford, general_tab, 1);
      } else {
        level--;
        continue;
      }
    }
    level++;
  }
}

int main(int argc, char *argv[]) {

    int id, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // id du processus
    MPI_Comm_size(MPI_COMM_WORLD, &p);  // nb de processus

    clock_t start = 0, stop = 0;
    double CPU_time = 0;

    int nb_tache_total = 0, nb_tache_bonus = 0, count_local = 0, count_global = 0, process_bonus = 0;
    int nb_tache = 0;

    vector<array<int, N>> solutions_global;
    vector<int> solution_global_flat; // Tableau de solutions pour le send
    vector<int> solution_local_flat;  // Tableau de solutions local pour le send
    vector<array<int, N>> solutions_local; // Tableau de solutions local

    vector<int> max_pos_tab_global = generateMaxPosTab(N);

    /**
     * Pour pouvoir choisir un nombre indéterminé de processus, on va plus pouvoir
     * faire de scatter car ce ne sera plus équitable, le dernier process aura le
     * reste des taches qui sera forcément inférieur à nb_tache_totale/p
     */
    if (id == p - 1) {
        // Temps
        cout << "N: " << N << "\nDEPTH: " << DEPTH << endl;
        start = clock();

        generateCombinations(DEPTH, max_pos_tab_global, nb_tache_total, solutions_global);

        cout << "Nombre total de tâches: " << nb_tache_total << endl;

        for (const auto &arr : solutions_global) {
            solution_global_flat.insert(solution_global_flat.end(), arr.begin(), arr.end());
        }
    }

    MPI_Bcast(&nb_tache_total, 1, MPI_INT, p - 1, MPI_COMM_WORLD);

    if (nb_tache_total % p == 0) {
        nb_tache = nb_tache_total / p; // nombre des tâches pour tous les  process
        nb_tache_bonus = 0;            // nombre de tâches du dernier process
    } else {
        process_bonus = nb_tache_total % p; // nombre de process qui auront une tâche de plus
        nb_tache = (int)(nb_tache_total / p); // nombre des tâches pour tous les  process
        nb_tache_bonus = nb_tache + 1; // nombre de tâches pour les x premier process qui ont 1 de plus
    }

    if (id == p - 1) {
        cout << "Nb taches: " << nb_tache << endl;
        cout << "Nb taches bonus: " << nb_tache_bonus << endl;
        cout << "Nb process bonus: " << process_bonus << endl;
        cout << "Taille vec global: " << solutions_global.size() << endl;
        cout << "Taille vec global flat: " << solution_global_flat.size() << endl;
    }

    // init des tableaux des tâches
    if (id < process_bonus) {
        solution_local_flat.resize(nb_tache_bonus * N); // Tableau de solutions local pour le send
        solutions_local.resize(nb_tache_bonus); // Tableau de solutions local
    } else {
        solution_local_flat.resize(nb_tache * N);    // Tableau de solutions local pour le send
        solutions_local.resize(nb_tache); // Tableau de solutions local
    }


    // Envoi des taches aux process depuis le process p-1
    if (id == p - 1) {
        int k;
        int start_send = 0;

        for (k = 0; k < (p - 1); k++) {

            if (k < p - 1) {

                MPI_Send(&solution_global_flat[start_send], (k < process_bonus ? nb_tache_bonus : nb_tache) * N, MPI_INT, k, 0, MPI_COMM_WORLD);
                start_send += ((k < process_bonus ? nb_tache_bonus : nb_tache) * N);
            }
        }

        copy(solution_global_flat.end() - ((p - 1) < process_bonus ? nb_tache_bonus : nb_tache) * N, solution_global_flat.end(), solution_local_flat.begin());

    } else { // reception des taches des process
        MPI_Recv(solution_local_flat.data(), (id < process_bonus ? nb_tache_bonus : nb_tache) * N, MPI_INT, p - 1, 0, MPI_COMM_WORLD, 0);
    }

    if (id < process_bonus) {
        for (int i = 0; i < nb_tache_bonus; ++i) {
            copy(solution_local_flat.begin() + i * N, solution_local_flat.begin() + (i + 1) * N, solutions_local[i].begin());
        }
    } else {
        for (int i = 0; i < nb_tache; ++i) {
            copy(solution_local_flat.begin() + i * N, solution_local_flat.begin() + (i + 1) * N, solutions_local[i].begin());
        }
    }


    // Langford
    for (auto &solution : solutions_local) {
        langford_algorithm(solution, max_pos_tab_global, count_local);
    }

    MPI_Reduce(&count_local, &count_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Affichage temps
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0) {
        stop = clock();
        CPU_time = double(stop - start) / CLOCKS_PER_SEC;

        printf("Time : %lf seconds\n", CPU_time);
        cout << "Nombre total de solutions: " << count_global << endl;
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
