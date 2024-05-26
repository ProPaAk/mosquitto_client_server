#include "server/server.h"

#include <iostream>

int main(int argc, char *argv[]) {
    // Parsing command line arguments
    if(argc == 1) { // Do not supported run without topic
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    // interpreting any second argv as topic
    std::string topic = "/in/" + std::string(argv[1]);

    mosqpp::lib_init();

    Server server(topic);

    server.loop_forever(-1, 1);

    mosqpp::lib_cleanup();
    return 0;
}