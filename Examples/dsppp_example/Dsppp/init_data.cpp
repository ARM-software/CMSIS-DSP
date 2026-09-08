#include "init_data.hpp"

void init_vectors(std::size_t l,float* a,float* b,float* c) {
  for (int i = 0; i < l; i++) {
    a[i] = b[i] = c[i] = i;
  }
}