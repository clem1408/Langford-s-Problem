#include <array>
#include <climits>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <unordered_set>
#include <vector>
#include <cstdint>

using namespace std;

constexpr int N = 12;
constexpr int DEPTH = 2;
constexpr int SIZE = 2 * N;
constexpr int MAX_STACK_SIZE = 1000000;
constexpr uint64_t PAIR_TEMPLATE[20] = {
    5,    9,    17,    33,    65,    129,    257,    513,    1025,    2049,
    4097, 8193, 16385, 32769, 65537, 131073, 262145, 524289, 1048577, 2097153};



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

int main(int argc, char *argv[]) {

    int id, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // id du processus
    MPI_Comm_size(MPI_COMM_WORLD, &p);  // nb de processus

    clock_t start = 0, stop = 0;
    double CPU_time = 0;

    uint64_t process_bonus = 0; // Range de process avec une tâche de plus, allant de 0 à process_bonus

    uint64_t nb_tache_global = 0;
    uint64_t nb_solution_global = 0;

    uint64_t nb_tache_local = 0;
    uint64_t nb_tache_local_bonus = 0;
    uint64_t nb_solution_local = 0;

    vector<uint64_t> tab_tache_global;
    vector<uint64_t> tab_tache_local;


    // Générer tâches
    if(id == p-1){

        start = clock();

        cout << "N: " << N << "\nDEPTH: " << DEPTH << endl;

        uint64_t nb_tache_temp = numberOfTasks();
        tab_tache_global.resize(nb_tache_temp);
        nb_tache_global = initTasks(tab_tache_global);
        tab_tache_global.resize(nb_tache_global);

        cout << "nb_tache_global: " << nb_tache_global << endl;
    }


    MPI_Bcast(&nb_tache_global, 1, MPI_INT, p-1, MPI_COMM_WORLD);


    /*  Vérification de si on a un nombre juste de processus pour diviser le nombre de tâches ou non.
        Dans le cas où, le nb de process ne divise pas, alors les x premier process ont une tâche de plus. */
    if (nb_tache_global % p == 0) { // Cas parfait
        nb_tache_local = (uint64_t)(nb_tache_global / p);   // nombre des tâches pour tous les process
        nb_tache_local_bonus = 0;                                 // nombre de tâches du dernier process
    } else { // Cas où c'est non divisible
        process_bonus = nb_tache_global % p;                // nombre de process qui auront une tâche de plus
        nb_tache_local = (uint64_t)(nb_tache_global / p);   // nombre des tâches pour tous les process
        nb_tache_local_bonus = nb_tache_local + 1;                // nombre de tâches pour les x premiers process qui ont 1 de plus
    }


    // Initialisation des vect de tâches locaux
    if (id < process_bonus) { // Premiers process aec une tache de plus
        tab_tache_local.resize(nb_tache_local_bonus); // Tableau de solutions local pour le send
    } else {
        tab_tache_local.resize(nb_tache_local);    // Tableau de solutions local pour le send
    }


    // Envoi des taches aux process depuis le process p-1
    if (id == p - 1) {
        uint64_t k;
        uint64_t start_send = 0;

        for (k = 0; k < (p - 1); k++) {

            if (k < p - 1) {

                MPI_Send(&tab_tache_global[start_send], (k < process_bonus ? nb_tache_local_bonus : nb_tache_local), MPI_UNSIGNED_LONG, k, 0, MPI_COMM_WORLD);
                start_send += ((k < process_bonus ? nb_tache_local_bonus : nb_tache_local));
            }
        }

        // Copie dans le vect local du dernier process de la fin du tableau de tâches
        copy(tab_tache_global.end() - ((p - 1) < process_bonus ? nb_tache_local_bonus : nb_tache_local), tab_tache_global.end(), tab_tache_local.begin());

    } else { // reception des taches des process
        MPI_Recv(tab_tache_local.data(), (id < process_bonus ? nb_tache_local_bonus : nb_tache_local), MPI_UNSIGNED_LONG , p - 1, 0, MPI_COMM_WORLD, 0);
    }



    // Langford
    if (id < process_bonus) { // Premiers process aec une tache de plus
        for (size_t i = 0; i < nb_tache_local_bonus; ++i) {
            nb_solution_local += langford_algorithm(tab_tache_local[i]);
        }
    } else {
        for (size_t i = 0; i < nb_tache_local; ++i) {
            nb_solution_local += langford_algorithm(tab_tache_local[i]);
        }
    }
    
    MPI_Reduce(&nb_solution_local, &nb_solution_global, 1, MPI_INT, MPI_SUM, p-1, MPI_COMM_WORLD);


    // Affichage temps
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == p-1) {
        stop = clock();
        CPU_time = double(stop - start) / CLOCKS_PER_SEC;

        printf("Time : %lf seconds\n", CPU_time);
        cout << "Nombre total de solutions: " << nb_solution_global << endl;
    }


    MPI_Finalize();

    return EXIT_SUCCESS;
}
