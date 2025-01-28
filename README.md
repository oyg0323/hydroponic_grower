# WaterCulture Project

This project is an Arduino-based automated water culture system designed to monitor and control environmental conditions for plant growth. It uses various sensors and actuators to maintain optimal conditions.

## Features

- **Light Sensor (BH1750):** Measures ambient light intensity.
- **EC Sensor:** Monitors the electrical conductivity of the water (nutrient levels).
- **pH Sensor:** Measures the pH level of the nutrient solution.
- **DHT11 Sensor:** Tracks temperature and humidity.
- **DS3231 RTC Module:** Provides real-time clock functionality.
- **LCD Display:** Displays system status and sensor readings.
- **Relays:** Controls LED, fan, and lighting systems.

## Components

1. **Sensors:**
   - BH1750 (Light Sensor)
   - EC Sensor (Electrical Conductivity)
   - pH Sensor
   - DHT11 (Temperature and Humidity)
2. **Modules:**
   - DS3231 RTC (Real-Time Clock)
   - LiquidCrystal_I2C (16x2 LCD Display)
3. **Actuators:**
   - Relay modules for controlling LED, fan, and lighting systems.
4. **Arduino:**
   - Microcontroller to run the program and manage peripherals.

## Pin Configuration

- **DHT11 Sensor:** Pin 3
- **LED Relay:** Pin 4
- **Fan Relay:** Pin 5
- **Lighting Relay:** Pin 6
- **EC Sensor:** Analog Pin A1
- **pH Sensor:** Analog Pin A0

## How It Works

1. **Sensor Monitoring:**
   - The system continuously reads data from light, EC, pH, and DHT11 sensors.
   - Displays the readings on the LCD module.
2. **Actuator Control:**
   - Based on sensor readings, the relays activate/deactivate the LED, fan, and lighting systems.
3. **Data Logging:**
   - Utilizes the DS3231 RTC module to timestamp readings.

## Setup Instructions

1. **Hardware Setup:**
   - Connect all sensors and actuators as per the pin configuration above.
   - Ensure the relays are properly wired to control the LED, fan, and lights.
2. **Software Setup:**
   - Install the following Arduino libraries:
     - `Wire.h`
     - `BH1750.h`
     - `ECSensor.h`
     - `pHSensor.h`
     - `LiquidCrystal_I2C.h`
     - `DS3231.h`
     - `DHT.h`
   - Upload the `WaterCulture.ino` file to your Arduino board.

## Libraries Required

Make sure to install the following libraries before uploading the code:

- **Wire.h:** For I2C communication
- **BH1750.h:** For light sensor
- **ECSensor.h:** For EC sensor
- **pHSensor.h:** For pH sensor
- **LiquidCrystal_I2C.h:** For LCD display
- **DS3231.h:** For RTC module
- **DHT.h:** For DHT11 sensor

## Usage

1. Power on the Arduino system.
2. Monitor the LCD for real-time updates on light intensity, temperature, humidity, pH, and EC levels.
3. Adjust the environmental settings as needed based on sensor feedback.

## Future Improvements

- Remote monitoring function using a server using Raspberry Pi

## License

This project is open-source and available under the [MIT License](LICENSE).

