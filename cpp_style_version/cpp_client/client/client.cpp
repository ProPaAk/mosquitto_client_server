#include "client.h"

#include <iostream>
#include <regex>
#include <numeric>
#include <exception>

Client::Client(const std::string& topic)
                :
                mosquittopp{},
                inputTopic("/in/" + topic),
                outputTopic("/out/" + topic)
{
    int result = connect("localhost", 1883, 600);
    if(result != MOSQ_ERR_SUCCESS) {
        std::string errorStr = "Failed to connect broker! Error: " +
                            std::string(mosquitto_strerror(result));
        throw std::logic_error(errorStr);
    }

    result = subscribe(nullptr, outputTopic.c_str(), 0);
    if(result != MOSQ_ERR_SUCCESS) {
        std::string errorStr = "Cannot to create subscription. Mosquitto error occurred: " +
                                                    std::string(mosquitto_strerror(result));
        throw std::logic_error(errorStr);
    }
}

void Client::on_message(const struct mosquitto_message * msg) {
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

void Client::RequestAndPublishMessageFromUser() {
    std::cout << "Please, write message to server: " << std::endl;
    std::string message;
    std::cin >> message;

    // Sending message to server
    int result = publish(nullptr, inputTopic.c_str(),
                        message.size(), (void*)message.c_str());
    if(result != MOSQ_ERR_SUCCESS) {
        std::string errorStr = "Publishing fails with error: " +
                            std::string(mosquitto_strerror(result));
        throw std::logic_error(errorStr);
    }
}