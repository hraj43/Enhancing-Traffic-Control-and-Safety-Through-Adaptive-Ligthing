# Adaptive Traffic Control System

This project is an **adaptive traffic control system** that utilizes Google Maps API to retrieve real-time traffic data and dynamically adjust traffic light timings to reduce congestion. The system leverages ESP32 microcontrollers, an LCD display, and Google Maps API to analyze traffic intensity across different routes and display it on an LCD, allowing for efficient traffic management.

## Project Overview

The goal of this project is to enhance urban traffic flow by adapting traffic signals based on real-time traffic data. The system retrieves traffic intensity information for various routes using Google Maps API, calculates the optimal signal timing, and visually indicates the least congested routes. This approach aims to optimize traffic flow, reduce wait times, and improve road safety.

## Features

- **Real-time Traffic Data**: Uses Google Maps Distance Matrix API to fetch live traffic data for given routes.
- **Dynamic Traffic Signal Adjustment**: Traffic light timings are adjusted based on congestion levels, reducing wait times.
- **User-friendly Interface**: Displays traffic intensity and signal timing information on an LCD screen for quick monitoring.
- **Automatic Route Selection**: Automatically identifies and prioritizes routes with lower traffic density.

## Hardware Requirements

- **ESP32 Microcontroller**: Manages traffic data processing and controls the signal lights.
- **I2C LCD Display**: Displays traffic intensity and estimated wait times.
- **Traffic Light LEDs**: Red and green LEDs simulate traffic lights for four different routes.
- **IR Sensor**: Detects vehicle presence.
  
## Libraries Used

- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) - for controlling the LCD display.
- [WiFi.h](https://www.arduino.cc/en/Reference/WiFi) - for connecting the ESP32 to a Wi-Fi network.
- [HTTPClient.h](https://www.arduino.cc/en/Reference/HTTPClient) - for sending HTTP requests to the Google Maps API.

## Code Overview

### Configuration and API Key Setup

```
const char* ssid = "edge";
const char* password = "12345678900";
const char* API_KEY = "YOUR_GOOGLE_MAPS_API_KEY";
```#   E n h a n c i n g - T r a f f i c - C o n t r o l - a n d - S a f e t y - T h r o u g h - A d a p t i v e - L i g t h i n g  
 