#include <atomic>
#include <thread> 
class barrier_object {
public:
    barrier_object() {
        // Probably don't need to do anything here.
    }

    void init(int num_threads) {
        // Implement me
        counter.store(0);
        numthreads = num_threads;
        sense.store(false);
        thread_sense = new std::atomic<bool>[numthreads] {};
        for (int i = 0; i < numthreads; i++) {
            thread_sense[i].store(true);
        }
    }
    void barrier(int tid) {
        // Implement me
        int arrival_num = counter.fetch_add(1);
        if (arrival_num == numthreads - 1) {
            counter.store(0);
            sense.store(thread_sense[tid].load());
        }
        else {
            int trysleep = 0;
            while (sense.load() != thread_sense[tid].load()) {
                if (trysleep < 10) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
                    trysleep++;
                }
                else {
                    std::this_thread::yield();
                }
                   
            }
        }
        thread_sense[tid].store(!thread_sense[tid]);
    }

private:
    // Give me some private variables
    std::atomic<int> counter;
    int numthreads;
    std::atomic<bool> sense;
    std::atomic<bool>* thread_sense;
};