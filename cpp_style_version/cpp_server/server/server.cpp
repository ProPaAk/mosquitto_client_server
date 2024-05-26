#include "server.h"

#include <iostream>
#include <regex>
#include <numeric>
#include <exception>

Server::Server(const std::string& topic)
                :
                mosquittopp(nullptr),
                inputTopic("/in/" + topic),
                outputTopic("/out/" + topic)
{
    int result = connect("localhost", 1883, 600);
    if(result != MOSQ_ERR_SUCCESS) {
        std::string errorStr = "Failed to connect broker! Error: " +
                            std::string(mosquitto_strerror(result));
        throw std::logic_error(errorStr);
    }

    result = subscribe(nullptr, inputTopic.c_str(), 0);
    if(result != MOSQ_ERR_SUCCESS) {
        std::string errorStr = "Cannot to create subscription. Mosquitto error occurred: " +
                                                    std::string(mosquitto_strerror(result));
        throw std::logic_error(errorStr);
    }
}

void Server::on_message(const struct mosquitto_message * msg) {
    if(!msg) {
        std::cout << "Invalid message!" << std::endl;
        return;
    }

    if(!msg->payload || msg->payloadlen == 0) {
        std::cout << "Received message is empty!" << std::endl;
        return;
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

    // Sending the resulting message to client
    int result = publish(nullptr, outputTopic.c_str(),
                            output.size(), (void*)output.c_str(), 2, false);
    if(result != MOSQ_ERR_SUCCESS) {
        std::cout   << "Publishing fails with error: "
                    << mosquitto_strerror(result) << std::endl;
        return;
    }

    return;
}
