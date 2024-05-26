#include <iostream>
#include <string>
#include <regex>
#include <numeric>

#include <include/mosquitto.h>

#include "helper_funcs.h"

// Callback that called when client publish the message
int OnSubscribe(mosquitto *mosq, void *obj, const mosquitto_message * msg) {
    if(!msg) {
        std::cout << "Invalid message!" << std::endl;
        return 1;
    }

    if(!msg->payload || msg->payloadlen == 0) {
        std::cout << "Received message is empty!" << std::endl;
        return 0;
    }

    std::string input = (const char *)msg->payload;
    std::cout << "Received message is: " << input << std::endl;

    // replacing comma to dot
    input = std::regex_replace(input, std::regex(","), ".");

    // Extracting numbers from input string
    std::regex pattern("[+-]?([0-9]*[.])?([0-9]*[,])?[0-9]+");
    auto begin = std::sregex_iterator(input.begin(), input.end(), pattern);
    auto end = std::sregex_iterator();

    std::string output;
    std::vector<double> resultVec;
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        resultVec.emplace_back(std::stod(match.str()));
    }

    // Preparing output data
    std::sort(resultVec.begin(), resultVec.end(), std::less());
    auto sum = std::accumulate(resultVec.begin(), resultVec.end(), 0.0);

    std::ostringstream oss;
    std::copy(resultVec.begin(), resultVec.end(), std::ostream_iterator<double>(oss, " "));
    output = oss.str();
    output += "\n" + std::to_string(sum);

    // Changing topic to outgoing
    std::string topic = msg->topic;
    topic = std::regex_replace(topic, std::regex("/in/"), "/out/");

    // Sending the resulting message to client
    int result = mosquitto_publish( mosq, nullptr, topic.c_str(),
                                    output.size(), (void*)output.c_str(), 2, false);
    if(result != MOSQ_ERR_SUCCESS) {
        std::cout   << "Publishing fails with error: "
                    << mosquitto_strerror(result) << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Parsing command line arguments
    if(argc == 1) { // Do not supported run without topic
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    // interpreting any second argv as topic
    std::string topic = "/in/" + std::string(argv[1]);

    mosquitto_lib_init();

    // Creating mosquitto client context
    auto mosq = mosquitto_new(nullptr, true, nullptr);
    if(mosq == nullptr) {
        std::cout << "MQTT server cannot be started. Failed to create server!" << std::endl;
        helpers::CleanUp();
        return 1;
    }

    // Specifying action on receiving message from client
    int result = mosquitto_subscribe_callback(  OnSubscribe, nullptr,
                                                topic.c_str(), 0,
                                                "localhost", 1883,
                                                nullptr, 60, true,
                                                nullptr, nullptr,
                                                nullptr, nullptr);
    if(result) {
        std::cout   << "Cannot to create subscription. Mosquitto error occurred: "
                    <<  mosquitto_strerror(result) << std::endl;
        helpers::CleanUp(mosq);
        return result;
    }

    helpers::CleanUp(mosq);
    return 0;
}