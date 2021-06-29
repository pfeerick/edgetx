It was found that the X9D+ and X7 (maybe others) have a slow inverter that messes up the reception of fast (400k baud) UART signals. As a result, users of X7 and X9D+ radios would often keep getting telemetry warnings or LUA scripts would not work properly when using the CRSF protocol.

Previously, the only way to fix this problem was to replace a specific 10k resistor on the circuit board on the transmitter. Even that was sometimes not enough. The other alternative was to lower the baud rate using custom firmware on the transmitter. However, this lower baud rate also increased latency in the system.

To address this issue, EdgeTX has developed OneBite Mode.  OneBit mode can help if your radio has a slow inverter circuit by changing the UART sampling behavior.  When using OneBit Mode, the signal is sampled later and the slow leading edges are ignored.  The result is that you are able to run the CRSF protocol at the full 400k baud rate without having to make any hardware modifications to your radio.

To enable OneBit Mode, go to the hardware page in the system settings menu and select the Sample Mode “OneBit”. 

