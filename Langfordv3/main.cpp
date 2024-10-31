/*#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <thread>

#define N 4
#define DEPTH 3

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
void generateCombinations(int depth, const std::vector<int>& max_pos_tab, int& count, std::vector<std::vector<int>>& solutions) {
    std::vector<int> indices(DEPTH, 1); // Indices de chaque position initialisés à 1

    while (true) {
        // Vérifie si la combinaison actuelle est consistante (sans doublons)
        std::unordered_set<int> uniqueValues(indices.begin(), indices.end());
        bool isValid = (uniqueValues.size() == depth);

        // Vérifie la contrainte de max_pos_tab pour chaque position
        for (int i = 0; i < depth && isValid; ++i) {
            if (indices[i] > max_pos_tab[N - depth + i]) {
                isValid = false;
            }
        }

        if (isValid) {
            // Ajoute la combinaison valide à la liste des solutions
            solutions.push_back(indices);
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

inline void show_langford(std::vector<int>& langford, std::vector<int>& general_tab)
{
  for (auto it = general_tab.begin(); it != general_tab.end(); ++it){
    std::cout << *it << " ";
  }std::cout << std::endl;

  for (auto it = langford.begin(); it != langford.end(); ++it){
    std::cout << *it << " ";
  }std::cout << std::endl;
}

inline void remove_pair(std::vector<int>& langford, std::vector<int>& general_tab, int pair)
{
  general_tab[langford[pair - 1]] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(std::vector<int>& langford, const std::vector<int>& max_pos_tab, std::vector<int>& general_tab, int pair)
{
  // Initialisation de i, selon si la paire est déjà placé ou non
  int i = (langford[pair - 1] != 0) ? langford[pair - 1] + 1 : 1;

  // Si la paire est déjà placée, on la retire
  if (langford[pair - 1] != 0)
  {
    remove_pair(langford, general_tab, pair);
  }

  // Parcourir les positions possibles pour la paire
  for (; i <= max_pos_tab[pair - 1]; i++)
  {
    int second_pos = i + pair + 1; // Pré-calculer la seconde position
    if (general_tab[i - 1] == 0 && general_tab[second_pos - 1] == 0)
    {
      general_tab[i - 1] = pair;
      general_tab[second_pos - 1] = pair;
      langford[pair - 1] = i;
      return 1; // Placement valide
    }
  }

  return 0; // Aucun emplacement valide trouvé
}

void init_general_tab(std::vector<int>& langford, std::vector<int>& general_tab){
    for (int i = N - DEPTH; i < N;i++){
        general_tab[langford[i - (N - DEPTH)] - 1] = i + 1;
        general_tab[langford[i - (N - DEPTH)] + i + 1] = i + 1;
    }
}

void langford_algorithm(std::vector<int>& langford, const std::vector<int>& max_pos_tab, int &count)
{
  int level = N - DEPTH;

  std::vector<int> general_tab(2 * N, 0);

  init_general_tab(langford, general_tab);

  //show_langford(langford, general_tab);

  /*while (level <= N)
  {
    show_langford(langford, general_tab);

    // Pause de 0,5 seconde
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (place_pair(langford, max_pos_tab, general_tab, level)) // Si placement de la paire valide
    {
      if (level == 1) // Si on arrive à placer le 1 (combinaison valide)
      {
        count++;
        remove_pair(langford, general_tab, 1);
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

int main() {
    int count = 0, count2 = 0; // Compteur de solutions
    std::vector<int> max_pos_tab = generateMaxPosTab(N); // Génère max_pos_tab
    std::vector<std::vector<int>> solutions; // Stocke toutes les solutions trouvées

    // Génère toutes les combinaisons possibles jusqu'à la profondeur spécifiée
    generateCombinations(DEPTH, max_pos_tab, count, solutions);

    // Affiche toutes les solutions stockées
    for (auto& solution : solutions) {
        for (int num : solution) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        //langford_algorithm(solution, max_pos_tab, count);
    }

    // Affiche le nombre total de solutions
    std::cout << "Nombre total de solutions: " << count2 << std::endl;

    return 0;
}
*/

#include <iostream>
#include <vector>
#include <unordered_set>

#define N 4
#define DEPTH 3

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
void generateCombinations(int depth, const std::vector<int>& max_pos_tab, int& count, std::vector<std::vector<int>>& solutions) {
    std::vector<int> indices(depth, 1); // Indices de chaque position initialisés à 1

    int k = 1;

    while (true) {
        // Vérifie si la combinaison actuelle est consistante (sans doublons)
        std::unordered_set<int> uniqueValues(indices.begin(), indices.end());
        bool isValid = (uniqueValues.size() == depth);
        //std::cout << "uniqueValues : " << uniqueValues.size() << std::endl;
        bool isValid2 = true;

        // Vérifie la contrainte de max_pos_tab pour chaque position
        for (int i = 0; i < depth && isValid; ++i) {
            if (indices[i] > max_pos_tab[N - depth + i]) {
                isValid = false;
                break;
            }
        }

        //std::cout << "isValid : " << isValid << std::endl;

        for (auto ind : indices){
          std::cout << ind << " ";
        }std::cout << std::endl;

        /*std::vector<int> general_tab(2 * N, 0);

        for( int i = 0; i < depth; ++i){
          if(general_tab[indices[i] - 1] == 0 && general_tab[indices[i] + i + 2] == 0){
            general_tab[indices[i] - 1] = i + 2;
            general_tab[indices[i] + i + 2] = i + 2;
          }else{
            isValid2 = false;
            break;
          }
        }*/

        if (isValid && isValid2) {
            // Initialise une solution remplie de zéros
            std::vector<int> solution(N, 0);

            // Remplit seulement les éléments de l'indice `N - DEPTH` jusqu'à `N`
            for (int i = 0; i < depth; ++i) {
                solution[N - depth + i] = indices[i];
            }

            solutions.push_back(solution); // Ajoute la solution au tableau
            count++; // Incrémente le compteur de solutions
        }

        /*if(k == 1){
          for (auto& solution : solutions) {
            for (int num : solution) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
          }
        }*/

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

int main() {
    int count = 0; // Compteur de solutions
    std::vector<int> max_pos_tab = generateMaxPosTab(N); // Génère max_pos_tab
    std::vector<std::vector<int>> solutions; // Stocke toutes les solutions trouvées

    // Génère toutes les combinaisons possibles jusqu'à la profondeur spécifiée
    generateCombinations(DEPTH, max_pos_tab, count, solutions);

    // Affiche toutes les solutions stockées
    for (auto& solution : solutions) {
        for (int num : solution) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // Affiche le nombre total de solutions
    std::cout << "Nombre total de solutions: " << count << std::endl;

    return 0;
}

/*#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <thread>

#define N 4
#define DEPTH 3

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
void generateCombinations(int depth, const std::vector<int>& max_pos_tab, int& count, std::vector<std::vector<int>>& solutions) {
    std::vector<int> indices(depth, 1); // Indices de chaque position initialisés à 1

    while (true) {
        // Vérifie si la combinaison actuelle est consistante (sans doublons)
        std::unordered_set<int> uniqueValues(indices.begin(), indices.end());
        bool isValid = (uniqueValues.size() == depth);
        bool isValid2 = true;

        // Vérifie la contrainte de max_pos_tab pour chaque position
        for (int i = 0; i < depth && isValid; ++i) {
            if (indices[i] > max_pos_tab[N - depth + i]) {
                isValid = false;
            }
        }

        std::vector<int> general_tab(2 * N, 0);

        for( int i = 0; i < depth; ++i){
          if(general_tab[indices[i] - 1] == 0 && general_tab[indices[i] + i + 2] == 0){
            general_tab[indices[i] - 1] = i + 2;
            general_tab[indices[i] + i + 2] = i + 2;
          }else{
            isValid2 = false;
            break;
          }
        }

        if (isValid && isValid2) {
            // Initialise une solution remplie de zéros
            std::vector<int> solution(N, 0);

            // Remplit seulement les éléments de l'indice `N - DEPTH` jusqu'à `N`
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

inline void show_langford(std::vector<int>& langford, std::vector<int>& general_tab)
{
  for (auto it = general_tab.begin(); it != general_tab.end(); ++it){
    std::cout << *it << " ";
  }std::cout << std::endl;

  for (auto it = langford.begin(); it != langford.end(); ++it){
    std::cout << *it << " ";
  }std::cout << std::endl;
}

inline void remove_pair(std::vector<int>& langford, std::vector<int>& general_tab, int pair)
{
  general_tab[langford[pair - 1] - 1] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(std::vector<int>& langford, const std::vector<int>& max_pos_tab, std::vector<int>& general_tab, int pair)
{
  // Initialisation de i, selon si la paire est déjà placé ou non
  int i = (langford[pair - 1] != 0) ? langford[pair - 1] + 1 : 1;

  // Si la paire est déjà placée, on la retire
  if (langford[pair - 1] != 0)
  {
    remove_pair(langford, general_tab, pair);
  }

  show_langford(langford, general_tab);

  // Parcourir les positions possibles pour la paire
  for (; i <= max_pos_tab[pair - 1]; i++)
  {
    int second_pos = i + pair; // Pré-calculer la seconde position
    if (general_tab[i - 1] == 0 && general_tab[second_pos] == 0)
    {
      general_tab[i - 1] = pair;
      general_tab[second_pos] = pair;
      langford[pair - 1] = i;
      show_langford(langford, general_tab);
      return 1; // Placement valide
    }
  }

  return 0; // Aucun emplacement valide trouvé
}

void init_general_tab(std::vector<int>& langford, std::vector<int>& general_tab){
    for (int i = N - DEPTH; i < N;i++){
        general_tab[langford[i] - 1] = i + 1;
        general_tab[langford[i] + i + 1] = i + 1;
    }
}

void langford_algorithm(std::vector<int>& langford, const std::vector<int>& max_pos_tab, int &count2)
{
  int level = N - DEPTH;

  std::vector<int> general_tab(2 * N, 0);

  init_general_tab(langford, general_tab);

  show_langford(langford, general_tab);

  while (level <= N - DEPTH + 1)
  {
    std::cout << "Level : " << level << std::endl;

    show_langford(langford, general_tab);

    // Pause de 0,5 seconde
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (place_pair(langford, max_pos_tab, general_tab, level)) // Si placement de la paire valide
    {
      if (level == 1) // Si on arrive à placer le 1 (combinaison valide)
      {
        count2++;
        std::cout << "oeoeoe" << std::endl;
        remove_pair(langford, general_tab, 1);
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

int main() {
    int count = 0, count2 = 0; // Compteur de solutions
    std::vector<int> max_pos_tab = generateMaxPosTab(N); // Génère max_pos_tab
    std::vector<std::vector<int>> solutions; // Stocke toutes les solutions trouvées

    // Génère toutes les combinaisons possibles jusqu'à la profondeur spécifiée
    generateCombinations(DEPTH, max_pos_tab, count, solutions);

    // Affiche toutes les solutions stockées
    for (auto& solution : solutions) {
        for (int num : solution) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        std::cout << "Nouvelle tâche" << std::endl;
        langford_algorithm(solution, max_pos_tab, count2);
    }

    // Affiche le nombre total de solutions
    std::cout << "Nombre total de solutions: " << count2 << std::endl;

    return 0;
}*/