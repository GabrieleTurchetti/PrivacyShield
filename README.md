# PrivacyShield

## Description

The project consists of a device that can be attached to doors, windows, and other surfaces to mask the noise of a room. The primary focus is to cover conversations inside the room to prevent people outside from hearing them. This goal is accomplished by one or more devices that capture the sound, recognize if there is a conversation or other types of noise, and decide to cover it with the most appropriate sound emitted by a transducer attached to the door or window.

In cases where multiple devices are connected together, a Leader is chosen among them to remain awake and monitor the noise. If the Leader recognizes the correct type of sound, it wakes up all the Followers in the room to prevent useless energy consumption. The Leader is chosen based on where the noise is most powerful on average.

The ecosystem includes a mobile application where it will be possible to set the type of noise to cover and view different statistics.

## Hardware list:
- ESP32-S3 Microcontroller (x2)
- Breadboard (x2)
- Connectors
- Battery (x4)
- 18650 Battery Shield V3: https://amzn.eu/d/0iAi3bPU
- I2S microphone (x2)
- Dayton Audio DAEX25: https://amzn.eu/d/07JOvqCm
- MAX98357A: https://amzn.eu/d/0508CoDR
- 22 AWG Speaker Wire: https://amzn.eu/d/0dPKMZ0M