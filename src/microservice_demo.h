#pragma once

#include "image_interface.h"

#include <string>


class MicroserviceDemo final : public ImageInterface {
public:
    MicroserviceDemo() = delete;
    MicroserviceDemo(const std::string &interface_url, 
                     const crow::request &request) : 
        ImageInterface(interface_url, request) {}

public:
    void ProcessRequest(std::string &response);

private:
    TALError handler(Json::Value &result) override;
};
