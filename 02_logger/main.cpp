#include "telemetry/Logger.h"
#include <iostream>

int main() {
    telemetry::Logger log("output.log", telemetry::LogLevel::Warn);

    log.info("Telemetry system started");
    log.warn("Packet delay detected");
    log.error("Sensor disconnected");

    std::cout << "Log written to output.log\n";
    return 0;
}