#include "Logger.h"
#include <iostream>

int main() {
    Logger log("output.log", LogLevel::Warn);

    log.info("Telemetry system started");
    log.warn("Packet delay detected");
    log.error("Sensor disconnected");

    std::cout << "Log written to output.log\n";
    return 0;
}