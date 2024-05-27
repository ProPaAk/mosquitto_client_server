#include <iostream>
#include <string>

#include <include/mosquitto.h>

#include "helper_funcs.h"

static std::string inputTopic;
static std::string outputTopic;

// Callback that called when server publish the message
void OnMessage(mosquitto *mosq, void *obj, const mosquitto_message *msg) {
    if(!msg) {
        std::cout << "Invalid message!" << std::endl;
        return;
    }

    if(!msg->payload || msg->payloadlen == 0) {
        std::cout << "Received message is empty!" << std::endl;
        return;
    }

    // Printing received  message
    std::cout << "Received  message: " << (const char *)msg->payload << std::endl;
    return;
}

int main(int argc, char *argv[]) {
    // Parsing command line arguments
    if(argc == 1) { // Do not supported run without topic
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    // interpreting any second argv as topic
    inputTopic = "/in/" + std::string(argv[1]);
    outputTopic = "/out/" + std::string(argv[1]);

    int result;
    mosquitto_lib_init();

    // Creating mosquitto client context
    auto mosq = mosquitto_new(nullptr, true, nullptr);
    if(mosq == nullptr) {
        std::cout << "MQTT server cannot be started. Failed to create server!" << std::endl;
        helpers::CleanUp();
        return 1;
    }

    // Setting callback to server message
    mosquitto_message_callback_set(mosq, OnMessage);

    // Connecting to mosquitto broker
    // TIP: broker must be started before starting server
    result = mosquitto_connect(mosq, "localhost", 1883, 600);
    if(result != MOSQ_ERR_SUCCESS) {
        std::cout   << "Failed to connect broker! Error: "
                    << mosquitto_strerror(result) << std::endl;
        helpers::CleanUp(mosq);
        return 1;
    }

    // Starting an client thread
    result = mosquitto_loop_start(mosq);
    if(result != MOSQ_ERR_SUCCESS) {
        std::cout   << "Failed to start network loop! Error: "
                    << mosquitto_strerror(result) << std::endl;
        helpers::CleanUp(mosq);
        return 1;
    }

    // Subscribing to server responce
    result = mosquitto_subscribe(mosq, nullptr, outputTopic.c_str(), 0);
    if(result != MOSQ_ERR_SUCCESS) {
        std::cout   << "Publishing fails with error: "
                    << mosquitto_strerror(result) << std::endl;
        helpers::CleanUp(mosq);
        return 1;
    }

    while(true) {
        std::cout << "Please, write message to server: " << std::endl;

        std::string message;
        std::getline(std::cin, message);

        // Sending message to server
        result = mosquitto_publish( mosq, nullptr, inputTopic.c_str(),
                            message.size(), (void*)message.c_str(), 0, false);
        if(result != MOSQ_ERR_SUCCESS) {
            std::cout   << "Publishing fails with error: "
                        << mosquitto_strerror(result) << std::endl;
            break;
        }
    }

    helpers::CleanUp(mosq);
    return 0;
}