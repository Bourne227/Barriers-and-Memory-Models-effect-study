#if defined(SR)
#include "SRBarrier.h"
#elif defined(SRO)
#include "SROBarrier.h"
#else
#error "No barrier specified"
#endif
barrier_object B;

// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
//void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    // Spawn threads once, use sense reversing barrier
//}
#include <thread>
using namespace std;

void rblur(double* input, double* output, int start, int end, int tid, barrier_object& B, int repeats) {
    for (int i = start; i < end; i++) {
        output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
    }
    for (int r = 1; r < repeats; r++) {
        B.barrier(tid);
        swap(input, output);
        for (int i = start; i < end; i++) {
            output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
        }
    }
    B.barrier(tid);
}

void rblur2(double* input, double* output, int start, int end) {
    for (int i = start + 1; i < end - 1; i++) {
        output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
    }
}

void repeat_blur(double* input, double* output, int size, int num_threads, int repeats) {
    int chunks = (size - 2) / num_threads;
    int mod = (size - 2) % num_threads;
    int is[num_threads];
    int ie[num_threads];
    for (int i = 0; i < num_threads; i++) {
        is[i] = 1 + i * chunks + min(i, mod);
        ie[i] = is[i] + chunks - 1;
        if (i < mod) {
            ie[i]++;
        }
    }

    thread threads[num_threads];
    B.init(num_threads);

    for (int i = 0; i < num_threads; i++) {
        threads[i] = thread(rblur, input, output, is[i], ie[i] + 1, i, ref(B), repeats);
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }
}

void swap(double*& input, double*& output) {
    double* tmp = input;
    input = output;
    output = tmp;
}

void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    if (num_threads == 1) {
        for (int r = 0; r < repeats; r++) {
            rblur2(input, output, 0, size);
            swap(input, output);
        }
    }
    else {
        repeat_blur(input, output, size, num_threads, repeats);
        swap(input, output);
    }
}
