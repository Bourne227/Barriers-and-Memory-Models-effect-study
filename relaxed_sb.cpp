#define SRBARRIER // So that the header is not included twice in driver.cpp
#include "SRBarrier.h"

using namespace std;

// You should not use the barrier in this part of the question, but it will be used in parts 2.2 and 2.3

// Note that the arguments to thread functions are passed as pointers
void t0_function(atomic_int* x, atomic_int* y, int* r0, barrier_object* B) {
  // complete me!
	x->store(1, memory_order_relaxed);
	int var1 = y->load(memory_order_relaxed);
	*r0 = var1;
}

void t1_function(atomic_int* x, atomic_int* y, int* r1,  barrier_object* B) {
  // complete me!
	y->store(1, memory_order_relaxed);
	int var0 = x->load(memory_order_relaxed);
	*r1 = var0;
}
