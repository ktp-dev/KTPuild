#include <cassert>
#include "math.h"

int main() {
    assert(times(2,2) == 4);
    assert(times(100, 0) == 0);
    assert(times(0, 100) == 0);
    return 0;
}
