#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <lib/cpp/mosquittopp.h>

class Client : public mosqpp::mosquittopp {
public:
    Client(const std::string& topic);
    ~Client() = default;

    void RequestAndPublishMessageFromUser();

protected:
    void on_message(const struct mosquitto_message * msg) override;

    std::string inputTopic;
    std::string outputTopic;
};

#endif // CLIENT_H
