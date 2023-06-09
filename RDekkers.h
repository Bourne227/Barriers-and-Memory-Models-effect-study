#include <atomic>

// this should be the same as SCDekkers with the change that every
// relaxed access should be memory_order_relaxed

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
    while (wants_to_enter[1 - tid].load(std::memory_order_relaxed) == true) {
      if (turn.load(std::memory_order_relaxed) != tid) {
        wants_to_enter[tid].store(false, std::memory_order_relaxed);
        while (turn.load(std::memory_order_relaxed) != tid) {}
        wants_to_enter[tid].store(true, std::memory_order_relaxed);
      }
    } 
  }

  void unlock(int tid) {
    // implement me!
    turn.store(1 - tid, std::memory_order_relaxed);
    wants_to_enter[tid].store(false, std::memory_order_relaxed);
  }


private:
  // Give me some private variables  
  std::atomic<bool> wants_to_enter[2];
  std::atomic<int> turn;
};
