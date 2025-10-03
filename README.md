# Smart Clock with Alarm & Temperature Monitor

An embedded system that combines real-time clock functionality with alarm management, temperature monitoring, and an interactive TFT display. The project uses an RTC module for accurate timekeeping, a DHT11 sensor for temperature readings, an infrared remote for user input, and a buzzer for alarm notifications.  

## Features
- Real-time clock using **RTC DS1307**
- **Set and adjust time, date, and alarm** via IR remote  
- **Temperature monitoring** with DHT11 sensor and thermometer display  
- **Interactive TFT display** with digital and analog clock  
- Alarm notification with buzzer and on-screen alert  
- Reset button for clearing alarms  

## Components
- Arduino-compatible microcontroller  
- RTC DS1307 module  
- DHT11 temperature sensor  
- TFT display (Adafruit ST7735)  
- Infrared receiver + DIYables IR remote  
- Buzzer  
- Push button  

## Demo
🎥 Watch the hardware demo on YouTube:  
👉 [Smart Clock Hardware Demo](https://youtube.com/shorts/9Gp2FU5_VdQ)

## Code
The code is written in **C++ for Arduino**, using the following libraries:
- `RTClib` for time management  
- `Adafruit_GFX` and `Adafruit_ST7735` for display  
- `DIYables_IRcontroller` for remote input  
- `DHT` for temperature readings  

## How It Works
1. **Main Screen** – displays current time, date, and analog clock.  
2. **Temperature Screen** – shows temperature with graphical thermometer.  
3. **Time/Date Setting** – configure hour, minute, day, month, and year via remote.  
4. **Alarm Setting** – select alarm hour and minute; system stores and displays active alarms.  
5. **Alarm Trigger** – buzzer rings and "RING!" message appears on TFT.  
6. **Reset Button** – clears alarm and returns to main screen.  
