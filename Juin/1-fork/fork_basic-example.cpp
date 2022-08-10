// pingPongSemaphore.cpp

#include <iostream>
#include <semaphore>
#include <thread>

std::counting_semaphore<1> signal2Ping(0);       // (1)
std::counting_semaphore<1> signal2Pong(0);       // (2)

std::atomic<int> counter {};
constexpr int countlimit = 1000000;

void ping() {
    while(counter <= countlimit) {
        signal2Ping.acquire();                  // (5)
        ++counter;
        signal2Pong.release();
    }
}

void pong() {
    while(counter < countlimit) {
        signal2Pong.acquire();
        signal2Ping.release();                  // (3)
    }
}

int main() {

    auto start = std::chrono::system_clock::now();

    signal2Ping.release();                     // (4)
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << '\n';

}
