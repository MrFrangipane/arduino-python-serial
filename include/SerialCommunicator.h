#ifndef PLATFORMIO_SERIALCOMMUNICATOR_H
#define PLATFORMIO_SERIALCOMMUNICATOR_H


#include <Arduino.h>
#include <array>
#include <functional>
#include <vector>

#include "SerialProtocol.h"


namespace Frangitron {

    class SerialCommunicator {
    public:
        using ReceiveCallback = std::function<void(void *, const std::vector<byte> &)>;
        using SendCallback = std::function<void(void *, std::vector<byte> &)>;

        void setCallbackParent(void *callbackParent1) {
            callbackParent = callbackParent1;
        }

        void registerReceiveCallback(SerialProtocol::DataTypeCode dataTypeCode1, ReceiveCallback callback) {
            receiveCallbacks[dataTypeCode1] = callback;
        }

        void registerSendCallback(SerialProtocol::DataTypeCode dataTypeCode1, SendCallback callback) {
            sendCallbacks[dataTypeCode1] = callback;
        }

        void poll() {
            if (!Serial.dtr()) {
                return;
            }

            while (Serial.available() > 0) {
                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

                byte incomingByte = Serial.read();

                if (receivingStatus == ReceivingStatus::Idle && incomingByte == SerialProtocol::flagBegin) {
                    receivingStatus = ReceivingStatus::ReceivingHeader;
                    continue;
                }

                if (receivingStatus == ReceivingStatus::ReceivingHeader) {
                    if (headerBufferIndex < SerialProtocol::headerSize) {
                        headerBuffer[headerBufferIndex] = incomingByte;
                        headerBufferIndex++;
                    } else {
                        direction = static_cast<SerialProtocol::Direction>(headerBuffer[0]);
                        dataTypeCode = static_cast<SerialProtocol::DataTypeCode>(headerBuffer[1]);
                        if (direction == SerialProtocol::Direction::Send) {
                            inDataSize = SerialProtocol::DataSize.at(dataTypeCode);
                        } else {
                            inDataSize = 0;
                        }
                        receivingStatus = ReceivingStatus::ReceivingData;
                    }
                }

                if (receivingStatus == ReceivingStatus::ReceivingData) {
                    if (receiveDataBuffer.size() < inDataSize) {
                        receiveDataBuffer.push_back(incomingByte);
                        continue;

                    } else if (receiveDataBuffer.size() == inDataSize && incomingByte == SerialProtocol::flagEnd) {
                        if (direction == SerialProtocol::Direction::Send &&
                            receiveCallbacks.find(dataTypeCode) != receiveCallbacks.end()) {
                            receiveCallbacks.at(dataTypeCode)(callbackParent, receiveDataBuffer);

                        } else if (direction == SerialProtocol::Direction::Receive &&
                                   sendCallbacks.find(dataTypeCode) != sendCallbacks.end()) {
                            sendCallbacks.at(dataTypeCode)(callbackParent, sendDataBuffer);
                            sendResponse();
                        }
                    }
                    resetToIdle();
                }
            }
        }

        void sendResponse() {
            Serial.write(static_cast<byte>(SerialProtocol::flagBegin));
            Serial.write(static_cast<byte>(SerialProtocol::Direction::Send));
            Serial.write(static_cast<byte>(dataTypeCode));
            Serial.write(sendDataBuffer.data(), sendDataBuffer.size());
            Serial.write(static_cast<byte>(SerialProtocol::flagEnd));
        }

    private:
        enum class ReceivingStatus : byte {
            Idle, ReceivingHeader, ReceivingData
        };

        void *callbackParent;
        std::map<SerialProtocol::DataTypeCode, ReceiveCallback> receiveCallbacks;
        std::map<SerialProtocol::DataTypeCode, SendCallback> sendCallbacks;
        ReceivingStatus receivingStatus = ReceivingStatus::Idle;
        uint8_t headerBufferIndex = 0;
        std::array<byte, SerialProtocol::headerSize> headerBuffer;
        SerialProtocol::DataTypeCode dataTypeCode;
        SerialProtocol::Direction direction = SerialProtocol::Direction::Receive;
        uint16_t inDataSize = 0;
        std::vector<byte> receiveDataBuffer;
        std::vector<byte> sendDataBuffer;

        void resetToIdle() {
            receivingStatus = ReceivingStatus::Idle;
            headerBufferIndex = 0;
            inDataSize = 0;
            headerBuffer.fill(0);
            receiveDataBuffer.clear();
            sendDataBuffer.clear();
        }
    };
}

#endif //PLATFORMIO_SERIALCOMMUNICATOR_H
