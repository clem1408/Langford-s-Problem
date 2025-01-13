#include <array>
#include <bitset>
#include <climits>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <vector>

constexpr int N = 12;
constexpr int DEPTH = 2;
constexpr int SIZE = 2 * N;
constexpr int MAX_STACK_SIZE = 1000000;
constexpr uint64_t PAIR_TEMPLATE[20] = {
    5,    9,    17,    33,    65,    129,    257,    513,    1025,    2049,
    4097, 8193, 16385, 32769, 65537, 131073, 262145, 524289, 1048577, 2097153};

using namespace std;

struct StaticStack {
  array<uint64_t, MAX_STACK_SIZE> data;
  int top = -1;

  inline bool isEmpty() const { return top == -1; }
  inline bool isFull() const { return top >= MAX_STACK_SIZE - 1; }

  inline void push(uint64_t value) {
    if (!isFull()) {
      data[++top] = value;
    } else {
      cerr << "Erreur : pile pleine !" << endl;
    }
  }

  inline uint64_t pop() {
    if (!isEmpty()) {
      return data[top--];
    } else {
      cerr << "Erreur : pile vide !" << endl;
      return 0;
    }
  }

  inline uint64_t peek() const {
    if (!isEmpty()) {
      return data[top];
    } else {
      cerr << "Erreur : pile vide !" << endl;
      return -1;
    }
  }
};

inline constexpr uint64_t numberOfTasks() {
  uint64_t number = 1;
  for (int i = 2; i < 2 + DEPTH; ++i) {
    number *= (2 * N - i);
  }
  return number;
}

inline uint64_t reverseBits(uint64_t n) {
  n = ((n & 0xffffffff00000000) >> 32) | ((n & 0x00000000ffffffff) << 32);
  n = ((n & 0xffff0000ffff0000) >> 16) | ((n & 0x0000ffff0000ffff) << 16);
  n = ((n & 0xff00ff00ff00ff00) >> 8) | ((n & 0x00ff00ff00ff00ff) << 8);
  n = ((n & 0xf0f0f0f0f0f0f0f0) >> 4) | ((n & 0x0f0f0f0f0f0f0f0f) << 4);
  n = ((n & 0xcccccccccccccccc) >> 2) | ((n & 0x3333333333333333) << 2);
  n = ((n & 0xaaaaaaaaaaaaaaaa) >> 1) | ((n & 0x5555555555555555) << 1);

  return n;
}

uint64_t initTasks(vector<uint64_t> &tasks) {
  StaticStack stack;
  unordered_set<uint64_t> bitset;
  int j = 0, level = N;
  uint64_t temp;

  stack.push(level);
  stack.push(0);

  while (!stack.isEmpty()) {
    temp = stack.pop();
    level = stack.pop();

    if (level > N - DEPTH) {
      uint64_t state = PAIR_TEMPLATE[level - 1];

      for (int i = 0; i < (SIZE - 1) - level; ++i) {
        uint64_t test = state << i;

        if ((temp & test) == 0) {
          uint64_t new_temp = temp | test;

          stack.push(level - 1);
          stack.push(new_temp);

          if (bitset.find(reverseBits(new_temp) >> (64 - SIZE)) ==
              bitset.end()) {
            bitset.insert(new_temp);

            if (level - 1 == N - DEPTH) {
              tasks[j] = new_temp;
              j++;
            }
          }
        }
      }
    }
  }

  return j;
}

uint64_t langford_algorithm(const uint64_t &task) {
  StaticStack stack;
  int level = N - DEPTH, count = 0;
  uint64_t temp;

  stack.push(level);
  stack.push(task);

  while (!stack.isEmpty()) {
    temp = stack.pop();
    level = stack.pop();

    if (level > 0) {
      uint64_t state = PAIR_TEMPLATE[level - 1];

      for (int i = 0; i < (SIZE - 1) - level; ++i) {
        uint64_t test = state << i;

        if ((temp & test) == 0) {
          uint64_t new_temp = temp | test;

          stack.push(level - 1);
          stack.push(new_temp);

          if (level - 1 == 0) {
            count++;
          }
        }
      }
    }
  }

  return count;
}

int main() {
  uint64_t number_of_solution = 0;
  uint64_t number_of_tasks = numberOfTasks();
  vector<uint64_t> tasks(number_of_tasks);

  double task_initialization_time[5] = {0.0};
  double algorithm_time[5] = {0.0};
  double total_task_time = 0.0;
  double total_algorithm_time = 0.0;

  cout << "===== Langford de " << N << " avec une profondeur de " << DEPTH
       << " =====" << endl
       << endl;

  for (int iteration = 0; iteration < 5; ++iteration) {
    number_of_solution = 0;

    // Mesure du temps d'initialisation des tâches
    clock_t start_task, stop_task;
    start_task = clock();
    uint64_t real_number_of_tasks = initTasks(tasks);
    stop_task = clock();
    task_initialization_time[iteration] =
        double(stop_task - start_task) / CLOCKS_PER_SEC;

    cout << "Execution " << iteration + 1 << ":\n";
    cout << "  Total number of tasks: " << real_number_of_tasks << endl;

    // Mesure du temps d'exécution de l'algorithme principal
    clock_t start_alg, stop_alg;
    start_alg = clock();
    for (size_t i = 0; i < real_number_of_tasks; ++i) {
      number_of_solution += langford_algorithm(tasks[i]);
    }
    stop_alg = clock();
    algorithm_time[iteration] = double(stop_alg - start_alg) / CLOCKS_PER_SEC;

    cout << "  Total number of solutions: " << number_of_solution << endl;
    cout << "  Temps d'initialisation des tâches: "
         << task_initialization_time[iteration] << " secondes\n";
    cout << "  Temps d'exécution de l'algorithme: " << algorithm_time[iteration]
         << " secondes\n\n";

    total_task_time += task_initialization_time[iteration];
    total_algorithm_time += algorithm_time[iteration];
  }

  cout << "Temps moyen d'initialisation des tâches: " << total_task_time / 5.0
       << " secondes\n";
  cout << "Temps moyen d'exécution de l'algorithme: "
       << total_algorithm_time / 5.0 << " secondes\n";

  return EXIT_SUCCESS;
}