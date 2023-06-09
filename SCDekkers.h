#include <atomic>

class dekkers_mutex {
public:
  dekkers_mutex() {
    // implement me!
    wants_to_enter[0].store(false);
    wants_to_enter[1].store(false);
    turn.store(0);
  }

  void lock(int tid) {
    // implement me!
    wants_to_enter[tid].store(true);
    while (wants_to_enter[1 - tid].load() == true) {
      if (turn.load() != tid) {
        wants_to_enter[tid].store(false);
        while (turn.load() != tid) {}
        wants_to_enter[tid].store(true);
      }
    } 
  }

  void unlock(int tid) {
    // implement me!
    turn.store(1 - tid);
    wants_to_enter[tid].store(false);
  }


private:
  // Give me some private variables 
  std::atomic<bool> wants_to_enter[2];
  std::atomic<int> turn;
};
