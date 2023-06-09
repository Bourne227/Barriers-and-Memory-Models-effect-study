#include <atomic>

// this should be the same as RDekkers with the change that every
// relaxed access should be memory_order_relaxed.
// your job is to use the following MACRO to fix the mutex.
// Think about the reorderings allowed under TSO and where the fence
// needs to be placed.

#define FENCE asm volatile("mfence":::"memory");

class dekkers_mutex {
public:
  dekkers_mutex() {
    // implement me!
    wants_to_enter[0].store(false, std::memory_order_relaxed);
    wants_to_enter[1].store(false, std::memory_order_relaxed);
    turn.store(0, std::memory_order_relaxed);
  }

  void lock(int tid) {
    // implement me!
    wants_to_enter[tid].store(true, std::memory_order_relaxed);
    FENCE
    while (wants_to_enter[1 - tid].load(std::memory_order_relaxed) == true) {
      if (turn.load(std::memory_order_relaxed) != tid) {
        wants_to_enter[tid].store(false, std::memory_order_relaxed);
        FENCE
        while (turn.load(std::memory_order_relaxed) != tid) {}
        wants_to_enter[tid].store(true, std::memory_order_relaxed);
        FENCE
      }
    } 
  }

  void unlock(int tid) {
    // implement me!
    turn.store(1 - tid, std::memory_order_relaxed);
    FENCE
    wants_to_enter[tid].store(false, std::memory_order_relaxed);
  }

private:
  // Give me some private variables  
  std::atomic<bool> wants_to_enter[2];
  std::atomic<int> turn;
};