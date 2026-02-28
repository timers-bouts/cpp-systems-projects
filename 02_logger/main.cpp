#include "telemetry/Logger.h"
#include <iostream>

int main() {
    telemetry::Logger logger("output.log", telemetry::LogLevel::Warn, false);

    logger.info("Telemetry system started");
    logger.warn("Packet delay detected");
    logger.error("Sensor disconnected");

    std::cout << "Log written to output.log\n";
    return 0;
}