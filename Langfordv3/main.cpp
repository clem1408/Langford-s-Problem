#include <array>
#include <iostream>
#include <time.h>
#include <vector>

#define N 12
#define DEPTH 3
#define SIZE 2 * N

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

void generateCombinations(int depth, const std::vector<int> &max_pos_tab,
                          int &count,
                          std::vector<std::vector<int>> &solutions) {
  std::vector<int> indices(depth, 1);

  while (true) {
    bool isValid = true;
    for (int i = 0; i < depth && isValid; ++i) {
      if (indices[i] > max_pos_tab[N - depth + i]) {
        isValid = false;
      }
    }

    if (isValid) {
      std::array<int, SIZE> general_tab = {0};
      bool isValid2 = true;

      for (int i = N - DEPTH; i < N; ++i) {
        int idx = indices[i - (N - DEPTH)];
        if (general_tab[idx - 1] == 0 && general_tab[idx + i + 1] == 0) {
          general_tab[idx - 1] = i + 1;
          general_tab[idx + i + 1] = i + 1;
        } else {
          isValid2 = false;
          break;
        }
      }

      if (isValid2) {
        std::vector<int> solution(N, 0);
        for (int i = 0; i < depth; ++i) {
          solution[N - depth + i] = indices[i];
        }
        solutions.push_back(std::move(solution));
        count++;
      }
    }

    int position = depth - 1;
    while (position >= 0) {
      int maxVal = SIZE - (N - depth + position);
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
                        std::array<int, SIZE> &general_tab, int pair) {
  general_tab[langford[pair - 1] - 1] = 0;
  general_tab[langford[pair - 1] + pair] = 0;
  langford[pair - 1] = 0;
}

inline int place_pair(std::vector<int> &langford,
                      const std::vector<int> &max_pos_tab,
                      std::array<int, SIZE> &general_tab, int pair) {
  int i = (langford[pair - 1] != 0) ? langford[pair - 1] + 1 : 1;
  if (langford[pair - 1] != 0)
    remove_pair(langford, general_tab, pair);

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

inline void init_general_tab(const std::vector<int> &langford,
                             std::array<int, SIZE> &general_tab) {
  for (int i = N - DEPTH; i < N; i++) {
    general_tab[langford[i] - 1] = i + 1;
    general_tab[langford[i] + i + 1] = i + 1;
  }
}

inline void langford_algorithm(std::vector<int> &langford,
                               const std::vector<int> &max_pos_tab,
                               int &count2) {
  int level = N - DEPTH;
  std::array<int, SIZE> general_tab = {0};
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
  std::vector<std::vector<int>> solutions;
  solutions.reserve(1000); // Reserve space based on expected tasks

  clock_t start = clock();
  generateCombinations(DEPTH, max_pos_tab, count, solutions);

  for (auto &solution : solutions) {
    langford_algorithm(solution, max_pos_tab, count2);
  }

  double CPU_time = (clock() - start) / (double)CLOCKS_PER_SEC;

  std::cout << "N: " << N << "\nDEPTH: " << DEPTH << std::endl;
  printf("Time : %lf seconds\n", CPU_time);
  std::cout << "Nombre total de tâches: " << count << std::endl;
  std::cout << "Nombre total de solutions: " << count2 << std::endl;

  return 0;
}
