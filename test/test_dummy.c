#include <assert.h>
#include <stdio.h>

void test_that_fails(void) {
  puts("normal output for debugging");

  assert(1);
}

int main(void) {
  test_that_fails();

  return 0;
}
