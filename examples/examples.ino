// Minimal example using a basic Header generated from Python library
#include <SerialCommunicator.h>

using namespace Frangitron;

SerialCommunicator serialCommunicator;


void sendAllTypes(void *somePointer, std::vector<byte> &data) {
    // Get data from somePointer
    auto someClass = static_cast<SomeClass>(somePointer);
    SerialProtocol::AllTypes allTypes = someClass->getAllTypes();

    // Send through serial
    data.resize(sizeof(SerialProtocol::BoardSettings));
    memcpy(data.data(), &allTypes, sizeof(SerialProtocol::BoardSettings));
}


void receiveAllTypes(void *somePointer, std::vector<byte> &data) {
    // Cast from received serial data
    SerialProtocol::AllTypes allTypes;
    memcpy(&allTypes, data.data(), data.size());

    // Update somePointer
    auto someClass = static_cast<SomeClass>(somePointer);
    someClass->updateAllTypes(allTypes);
}


void setup() {
    serialCommunicator.init(somePointer);  // somePointer will be passed to callbacks
    serialCommunicator.registerSendCallback(
            SerialProtocol::DataTypeCode::AllTypes,
            sendAllTypes
    );
    serialCommunicator.registerReceiveCallback(
            SerialProtocol::DataTypeCode::AllTypes,
            receiveAllTypes
    );
}


void loop() {
  serialCommunicator.poll();
}
