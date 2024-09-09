#include "vector.h"

int main()
{
  specialize_vector(a, int);
  initialize_vector(a);
  int i = 5;

  vector_push_back(a, i);
  vector_push_back_const(a, 625);

  printf("vector_at(a, 0): %d\n", vector_at_(a, 0));
  printf("vector_at(a, 1): %d\n", vector_at_(a, 1));
}  
