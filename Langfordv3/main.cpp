#include <array>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <ctime>

#define N 16
#define DEPTH 6

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
void generateCombinations(int depth, const std::vector<int>& max_pos_tab, int& count, std::vector<std::array<int, N>>& solutions) {
    std::vector<int> indices(depth, 1); // Indices de chaque position initialisés à 1

    while (true) {
        // Vérifie si la combinaison actuelle est consistante (sans doublons)
        std::unordered_set<int> uniqueValues(indices.begin(), indices.end());
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
            std::array<int, 2 * N> general_tab = {0};
          
            for (int i = N - DEPTH; i < N; ++i) {
                if (general_tab[indices[i - (N - DEPTH)] - 1] == 0 && general_tab[indices[i - (N - DEPTH)] + i + 1] == 0) {
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
            std::array<int, N> solution = {0};

            // Remplit seulement les éléments de l'indice N - DEPTH jusqu'à N
            for (int i = 0; i < depth; ++i) {
                solution[N - depth + i] = indices[i];
            }

            solutions.push_back(solution); // Ajoute la solution au tableau
            count++; // Incrémente le compteur de solutions
        }

        // Incrémente les indices de droite à gauche
        int position = depth - 1;
        while (position >= 0) {
            int maxVal = 2 * N - 2 - (N - depth + position); // Limite supérieure pour chaque position
            if (indices[position] < maxVal) {
                indices[position]++;
                break;
            } else {
                indices[position] = 1;
                position--;
            }
        }

        // Si tous les indices sont réinitialisés, la génération est terminée
        if (position < 0) break;
    }
  }


inline void show_langford(std::array<int, N>& langford, std::array<int, 2 * N>& general_tab) {
    for (auto it = general_tab.begin(); it != general_tab.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (auto it = langford.begin(); it != langford.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

inline void remove_pair(std::array<int, N>& langford, std::array<int, 2 * N>& general_tab, int pair) {
    general_tab[langford[pair - 1] - 1] = 0;
    general_tab[langford[pair - 1] + pair] = 0;
    langford[pair - 1] = 0;
}

inline int place_pair(std::array<int, N>& langford, const std::vector<int>& max_pos_tab, std::array<int, 2 * N>& general_tab, int pair) {
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

inline void init_general_tab(std::array<int, N>& langford, std::array<int, 2 * N>& general_tab) {
    for (int i = N - DEPTH; i < N; i++) {
        general_tab[langford[i] - 1] = i + 1;
        general_tab[langford[i] + i + 1] = i + 1;
    }
}

inline void langford_algorithm(std::array<int, N>& langford, const std::vector<int>& max_pos_tab, int &count2) {
    int level = N - DEPTH;
    std::array<int, 2 * N> general_tab = {0};

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
    std::vector<int> max_pos_tab = generateMaxPosTab(N);
    std::vector<std::array<int, N>> solutions;

    clock_t start, stop;
    double CPU_time; 

    std::cout << "N: " << N << "\nDEPTH: " << DEPTH << std::endl;

    start = clock();
    generateCombinations(DEPTH, max_pos_tab, count, solutions);
    std::cout << "Nombre total de tâches: " << count << std::endl;

    /*for (auto& solution : solutions) {
        std::cout << "Task : ";
        for (auto ind : solution){
            std::cout << ind << " ";
        }
        std::cout << std::endl;
        
        //langford_algorithm(solution, max_pos_tab, count2);
    }*/

    stop = clock();
    CPU_time = double(stop - start) / CLOCKS_PER_SEC;

    printf("Time : %lf seconds\n", CPU_time);
    std::cout << "Nombre total de solutions: " << count2 << std::endl;

    return 0;
}
