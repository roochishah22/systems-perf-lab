#include <iostream>
#include <thread>

void work(int id) {
    // print: "thread X running" where X is the id
    std::cout << "thread " << id << " running\n";
}

int main() {
    // create 4 threads, each calling work() with its id
    std::thread t1(work, 1);
    std::thread t2(work, 2);
    std::thread t3(work, 3);
    std::thread t4(work, 4);
    // join all of them
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}