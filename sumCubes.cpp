#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 6

using namespace std;

struct thread_data {
    int thread_id;
    double current;
    double max;
    double upper_bound;
};

void *doWork (void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    
    double current       = my_data -> current;
    double upper_bound   = my_data -> upper_bound;
    double max           = my_data -> max;
    double current_cubed = 0;

    cout << "THREAD ID : " << (my_data -> thread_id) << ", RANGE : " << my_data -> current << " to " << upper_bound << endl;

    for (; current < upper_bound; ++current) {
        current_cubed = pow(current, 3);
        for (double j = current; j <= max; j += 2) {
            // cout << "I " << current << " J " << j << " " << (current_cubed + pow(j, 3)) << endl;
            // if (j >= 10)
               // pthread_exit(NULL);
            cout << (current_cubed + pow(j, 3)) << endl;
        }
    }

    pthread_exit(NULL);
}

void generate_cubes () {

    double icubed = 0;

    // double MAX = 100000000000;

    double MAX = 6;

    cout << fixed << setprecision(10) << 0 << endl << 1 << endl;

    for (double i = 1; i <= MAX; ++i) {
        icubed = pow(i, 3);
        for (double j = i; j <= MAX; ++j) {
            if (fmod((i + j), 2) != 0.0)
                continue;
            cout << (icubed + pow(j, 3)) << endl;
        }
    }
}

double split_current(int total, int number, double max, double& current) {
    double size_chunk = max / total;
    current = floor(size_chunk * number) + 1;
    return current + size_chunk;
}

int main (void) {
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    int rc;
    double MAX = 100000000000;
    
    // double MAX = 6;
    // unsigned int microseconds = 5000000;


    for (int i = 0; i < NUM_THREADS; ++i) {
        // cout << "main() : creating thread, " << i << endl;
        td[i].thread_id = i;
        td[i].upper_bound = split_current(NUM_THREADS, i, MAX, td[i].current);
        td[i].max = MAX;
        
        rc = pthread_create(&threads[i], NULL, doWork, (void *)&td[i]);

        if (rc) {
            cout << "Error : unable to create thread," << rc << endl;
            exit(-1);
        }

        // usleep (microseconds);
    }

    // cout << "MAX VALUE IS : " << MAX << ", CHUNKS ARE : " << (MAX / NUM_THREADS) << endl;

    pthread_exit(NULL);

    // generate_cubes();
}
