#include <Common/runtime.hpp>
#include <Common/log.hpp>

std::atomic_bool Runtime::run = true;

void Runtime::Clear() {
    run = true;
}

bool Runtime::Run() {
    return run;
}

void Runtime::Stop() {
    run = false;
}

void Runtime::SignalHandler(int signalNumber) {
    switch (signalNumber) {
        case SIGTERM:
        case SIGINT:
        case SIGABRT:
            Logger::trace("Recieved terminate signal, stopping.");
            Stop();
            break;
        default:
            Logger::warn("Recieved unhandled signal: {}.", signalNumber);
            break;
    }
}