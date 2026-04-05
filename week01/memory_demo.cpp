#include <iostream>

int main() {
    // Stack: automatic, fast, cleaned up for you
    int x = 5;
    std::cout << "stack x = " << x << "\n";
    std::cout << "stack address: " << &x << "\n";

    // Heap: manual, you own it
    int* y = new int(10);
    std::cout << "heap y = " << *y << "\n";
    std::cout << "heap address: " << y << "\n";

    delete y;

    // Now do this — delete the same pointer twice
    delete y;  // what happens?

    return 0;
}