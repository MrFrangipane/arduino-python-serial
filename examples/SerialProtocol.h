#ifndef TESTS_ALL_TYPES_H
#define TESTS_ALL_TYPES_H

#include <map>

#include <Arduino.h>


namespace Tests {

    class SerialProtocol {
        /*
         * Message topology
         *
         * ```
         * |   0   |    1      |     2     |   n  | 3 + n |
         * | begin | direction | data type | data |  end  |
         * |-------| ------ header ------- | data |-------|
         * ```
         */
    public:
        enum class Direction : int {
            Send,
            Receive
        };

        static constexpr uint8_t headerSize = 2;
        static constexpr byte flagBegin = 0x3c;
        static constexpr byte flagEnd = 0x3e;

        /* //////////////////////////////// */

        enum DataTypeCode : int {
            AllTypesCode,
        };

        struct AllTypes {
            char fieldString[10] = "         ";
            float fieldFloat = 0.0;
            bool fieldBoolean = false;
            int fieldInteger = 0;
            int fieldIntegers[3] = {0, 0, 0};
            byte fieldBytes[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
        };

        static const std::map<DataTypeCode, uint16_t> DataSize;
    };

    const std::map<SerialProtocol::DataTypeCode, uint16_t> SerialProtocol::DataSize = {
        {SerialProtocol::DataTypeCode::AllTypesCode, sizeof(SerialProtocol::AllTypes)},
    };
}

#endif // TESTS_ALL_TYPES_H