#include <array>
#include <climits>
#include <iostream>
#include <stack>
#include <unordered_set>

#define N 4
#define DEPTH 2
#define SIZE (2 * N)
#define MAX_STACK_SIZE 100000

using namespace std;

long numberOfTasks() {
  long number = 1;
  for (int i = 2; i < 2 + DEPTH; ++i) {
    number *= (2 * N - i);
  }
  return number;
}

void initPairTemplate(long *tab) {
  for (int i = 0; i < N; ++i) {
    tab[i] = (1 << (i + 2)) | 1;
  }
}

long reverseBits(long num) {
  long reversed = 0;

  for (int i = 0; i < SIZE; ++i) {
    reversed = (reversed << 1) | (num & 1);
    num >>= 1;
  }

  return reversed;
}

struct StaticStack {
  array<long, MAX_STACK_SIZE> data;
  int top = -1;

  bool isEmpty() const { return top == -1; }

  bool isFull() const { return top >= MAX_STACK_SIZE - 1; }

  void push(long value) {
    if (!isFull()) {
      data[++top] = value;
    } else {
      std::cerr << "Erreur : pile pleine !" << std::endl;
    }
  }

  long pop() {
    if (!isEmpty()) {
      return data[top--];
    } else {
      std::cerr << "Erreur : pile vide !" << std::endl;
      return 0;
    }
  }

  long peek() const {
    if (!isEmpty()) {
      return data[top];
    } else {
      std::cerr << "Erreur : pile vide !" << std::endl;
      return -1;
    }
  }
};

int initTasks(long *tasks, long *pair_template) {
  StaticStack stack;
  unordered_set<long> bitset;
  int j = 0, level = N;
  long temp;

  stack.push(level);
  stack.push(0);

  while (!stack.isEmpty()) {
    temp = stack.pop();
    level = stack.pop();

    if (level > N - DEPTH) {
      long state = pair_template[level - 1];

      for (int i = 0; i < (SIZE - 1) - level; ++i) {
        long test = state << i;

        if ((temp & test) == 0) {
          long new_temp = temp | test;

          stack.push(level - 1);
          stack.push(new_temp);

          if (bitset.find(reverseBits(new_temp)) == bitset.end()) {
            bitset.insert(new_temp);

            if (level - 1 == N - DEPTH) {
              tasks[j] = new_temp;
              j++;
              cout << "Task = " << tasks[j - 1] << endl;
            }
          }
        }
      }
    }
  }

  return j;
}

inline void remove_pair(long &task, long pair, int ind) {
  pair = pair << ind;
  task &= ~pair;
}

inline int place_pair(long &task, long pair, int level, int ind) {
  if (ind != 0) {
    remove_pair(task, pair, ind);
  }
  for (int i = ind; i < (SIZE - 1) - level; ++i) {
    long test = pair << i;

    if ((task & test) == 0) {
      task |= test;
      cout << "Task = " << task << endl;
      return i;
    }
  }
  return 0;
}

inline long langford_algorithm(long &task, long *pair_template) {
  int level = N - DEPTH, ind = 0;
  long count = 0;

  while (level <= N - DEPTH) {
    cout << "Level = " << level << endl;
    ind = place_pair(task, pair_template[level - 1], level, ind);
    cout << "Ind = " << ind << endl;
    if (ind) {
      if (level == 1) {
        count++;
        remove_pair(task, pair_template[level - 1], ind);
      } else {
        level--;
        continue;
      }
    }
    level++;
    cout << "Incremente" << endl;
    cout << endl;
  }

  return count;
}

int main() {
  long number_of_solution = 0;

  long pair_template[N];
  initPairTemplate(pair_template);

  long number_of_tasks = numberOfTasks();

  long *tasks = (long *)malloc(number_of_tasks * sizeof(long));

  std::cout << "N: " << N << "\nDEPTH: " << DEPTH << std::endl;

  clock_t start, stop;
  double CPU_time;

  start = clock();
  int real_number_of_tasks = initTasks(tasks, pair_template);

  cout << "Total number of tasks: " << real_number_of_tasks << endl;

  for (int i = 0; i < real_number_of_tasks; ++i) {
    number_of_solution += langford_algorithm(tasks[i], pair_template);
  }

  stop = clock();

  cout << "===Total number of solutions: " << number_of_solution
       << "===" << endl;

  CPU_time = double(stop - start) / CLOCKS_PER_SEC;

  printf("Time: %lf seconds\n", CPU_time);

  free(tasks);

  return EXIT_SUCCESS;
}