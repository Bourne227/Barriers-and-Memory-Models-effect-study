// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
#include <thread>
using namespace std;

void rblur(double* input, double* output, int start, int end) {
    for (int i = start; i < end; i++) {
        output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
    }
}

void rblur2(double* input, double* output, int start, int end) {
    for (int i = start + 1; i < end - 1; i++) {
        output[i] = (input[i] + input[i + 1] + input[i - 1]) / 3;
    }
}

void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    // Spawn and join threads repeatedly.
    thread threads[num_threads];
    for (int r = 0; r < repeats; r++) {
        if (num_threads == 1) {
            int chunks = size / num_threads;
            int notmo = size % num_threads;
            int is = 0;
            int ie = 0;

            for (int i = 0; i < num_threads; i++) {
                ie += chunks;
                if (i < notmo) {
                    ie += 1;
                }
                threads[i] = thread(rblur2, input, output, is, ie);
                is = ie - 1;
            }
            for (int j = 0; j < num_threads; j++) {
                threads[j].join();
            }

            double* tmp = input;
            input = output;
            output = tmp;
        }
        else {
            int chunks = size / num_threads;
            int is = 1;
            int ie = chunks + 1;
            thread threads[num_threads];
            for (int i = 0; i < num_threads; i++) {
                threads[i] = thread(rblur, input, output, is, ie);
                is = ie;
                ie += chunks;
                if (i == num_threads - 2) {
                    ie = size - 1;
                }
            }
            for (int i = 0; i < num_threads; i++) {
                threads[i].join();
            }

            double* tmp = input;
            input = output;
            output = tmp;
        }
    }

}
