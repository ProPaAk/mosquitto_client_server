#ifndef SERVER_H
#define SERVER_H

#include <string>

#include <lib/cpp/mosquittopp.h>

class Server : public mosqpp::mosquittopp {
public:
    Server(const std::string& topic);
    ~Server() = default;

protected:
    void on_message(const struct mosquitto_message * msg) override;

    std::string inputTopic;
    std::string outputTopic;
};

#endif // SERVER_H
