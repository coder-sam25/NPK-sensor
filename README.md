# NPK Sensor Arduino Code

This project reads 7 parameters from an RS485 NPK soil sensor using Modbus RTU:

- Moisture
- Temperature
- EC
- pH
- Nitrogen
- Phosphorus
- Potassium

## Hardware
- Arduino (UNO/Nano)
- RS485 TTL Converter (MAX485)
- NPK Soil Sensor

## Baud Rate
4800

## Notes
- Uses SoftwareSerial
- CRC validation not yet implemented
