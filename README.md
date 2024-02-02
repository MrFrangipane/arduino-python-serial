# Python Arduino Serial

Bidirectionnal serial communication (C Structs and Python Dataclasses) between Python and Arduino

- declare type hinted Dataclasses in Python
- generate C Header files
- send and receive data from both Python and Arduino through serial communication

Meant to be used with [Python Arduino Serial](https://github.com/MrFrangipane/python-arduino-serial)

## Usage

- See [examples](/examples)

## Protocol

Message topology (bytes)

|    0    |     1     |     2     |  n   |  n + 3  |
|:-------:|:---------:|:---------:|:----:|:-------:|
|  begin  | direction | data type | data |   end   |
| ------- |  header   |  header   | data | ------- |

**note** data is omitted when sending a reception request (only data type is sent)
