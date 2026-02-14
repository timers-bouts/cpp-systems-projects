#include "telemetry/PacketWriter.h"
#include "telemetry/TelemetryRecorder.h"


int main() {

    telemetry::TelemetryRecorder tr("test.bin", telemetry::TelemetryRecorder::OpenMode::Truncate);

    return 0;
}