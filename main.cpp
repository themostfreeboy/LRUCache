#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <atomic>
#include <thread>
#include <csignal>
#include <unistd.h>
#include "lru_cache.h"

static std::atomic<bool> g_quit;

void signal_handle(int sig) { 
    g_quit = true;
}

int main(int argc, char** argv) {
    g_quit = false;

    signal(SIGINT, signal_handle);
    signal(SIGTERM, signal_handle);

    srand((unsigned int)time(NULL));

    LRUCache<int, int> lru_cache(5);

    while (!g_quit) {
        int key = rand() % 10;
        int op = rand() % 2;
        if (op == 0) {
            int value = rand() % 1000;
            lru_cache.set(key, value);
            std::cout << "set_key(" << key << ") success, value=" << value << std::endl;
            std::cout << lru_cache.cache_list_str();
        } else {
            int value = 0;
            bool ret = lru_cache.get(key, value);
            if (ret) {
                std::cout << "get_key(" << key << ") success, value=" << value << std::endl;
            } else {
                std::cout << "get_key(" << key << ") failed" << std::endl;
            }
            std::cout << lru_cache.cache_list_str();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return 0;
}
