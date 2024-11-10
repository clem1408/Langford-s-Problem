#include <iostream>
#include <stack>

#define N 3
#define DEPTH 3
#define SIZE (2 * N)

using namespace std;

int numberOfTasks(){
    int number = 1;
    for (int i = 2; i < 2 + DEPTH; ++i){
        number *= (2 * N - i);
    }
    return number;
}

void initPairTemplate(int* tab){
    for (int i = 0; i < N; ++i){
        tab[i] = (1 << (i + 1)) | 1;  
    }
}

void initTasks(int* tasks, int size) {
    stack<int> stack;
    int level = N, temp;

    stack.push(0); // Init la stack

    while (level > N - DEPTH){
        temp = stack.top();
        stack.pop();

        for (){
            
        }
    }
    /*while (!stack.empty()) {
        cout << "Top element: " << stack.top() << endl; 
        stack.pop(); 
    }*/
}

int main() {
    int pairTemplate[N];

    initPairTemplate(pairTemplate);

    for (auto ind : pairTemplate){
        cout << ind << endl;
    }

    int number_of_tasks = numberOfTasks();

    int tasks[number_of_tasks];

    initTasks(tasks, number_of_tasks);

    cout << "Total number of tasks: " << number_of_tasks << endl;
}