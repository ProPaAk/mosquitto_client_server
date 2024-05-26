#include "client/client.h"

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

    try {
        Client client(topic);
        client.loop_start();

        for(;;) {
            try {
                client.RequestAndPublishMessageFromUser();
            } catch(const std::exception& ex) { // Publish exceptions
                std::cout << "Failed to publish message! Error: " << ex.what() << std::endl;
            }
        }
    } catch(const std::exception& ex) { // Constructor exceptions
        std::cout << "Failed to start up server! Error: " << ex.what() << std::endl;
        mosqpp::lib_cleanup();
        return 1;
    }

    mosqpp::lib_cleanup();
    return 0;
}