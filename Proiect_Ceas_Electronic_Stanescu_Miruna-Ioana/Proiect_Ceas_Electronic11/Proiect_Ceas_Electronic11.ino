#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DIYables_IRcontroller.h>
#include "DHT.h"

#define IR_RECEIVER_PIN 7
#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9
#define DHTPIN 2
#define DHTTYPE DHT11
#define BUZZER_PIN 3
#define LOW_FREQUENCY_DELAY 3 
#define buttonPin 4

DIYables_IRcontroller_17 irController(IR_RECEIVER_PIN, 200);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);

bool isTemperatureScreen = false;
bool isTimeSetScreen = false;
bool isMinuteSetScreen = false;
bool isDaySetScreen = false;
bool isMonthSetScreen = false;
bool isYearSetScreen = false;
int newHour = 0;
int newMinute = 0;
int newDay = 0;
int newMonth = 0;
int newYear = 0;

bool isAlarmSetScreen = false;
int alarmHour = 0;
int alarmMinute = 0;
bool isSettingAlarmHour = true; 
char* zile[] = {
//      "Duminica", 
//      "Luni",   
//      "Marti",  
//      "Miercuri", 
//      "Joi",    
//      "Vineri", 
//      "Sambata"
//  };

void setup() {
  Serial.begin(9600);
  irController.begin();
 pinMode(BUZZER_PIN, OUTPUT);
 pinMode(buttonPin, INPUT_PULLUP);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);

  dht.begin();
}

void loop() {
  Key17 key = irController.getKey();
  if (key != Key17::NONE) {
    Serial.print("Key pressed: ");
    Serial.println((int)key);
    switch (key) {
      case Key17::KEY_1:
        isTemperatureScreen = false;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        delay(500);
        break;

      case Key17::KEY_2:
        isTemperatureScreen = true;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        delay(500);
        break;

      case Key17::KEY_3:
        isTimeSetScreen = true;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        newHour = rtc.now().hour();
        delay(500);
        break;

      case Key17::KEY_4:
        isMinuteSetScreen = true;
        isTimeSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        newMinute = rtc.now().minute();
        delay(500);
        break;

      case Key17::KEY_5:
        isDaySetScreen = true;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        newDay = rtc.now().day();
        delay(500);
        break;

      case Key17::KEY_6:
        isMonthSetScreen = true;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isYearSetScreen = false;
        newMonth = rtc.now().month();
        delay(500);
        break;

      case Key17::KEY_7:
        isYearSetScreen = true;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        newYear = rtc.now().year();
        delay(500);
        break;

      case Key17::KEY_8: // New case for entering alarm set screen
        isAlarmSetScreen = true;
        isTemperatureScreen = false;
        isTimeSetScreen = false;
        isMinuteSetScreen = false;
        isDaySetScreen = false;
        isMonthSetScreen = false;
        isYearSetScreen = false;
        isSettingAlarmHour = true; // Start with setting the hour
        alarmHour = 13; // Default alarm hour
        alarmMinute = 30; // Default alarm minute
        delay(500);
        break;

      case Key17::KEY_OK:
        if (isTimeSetScreen) {
          DateTime now = rtc.now();
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), newHour, now.minute(), now.second()));
          isTimeSetScreen = false;
        } else if (isMinuteSetScreen) {
          DateTime now = rtc.now();
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newMinute, now.second()));
          isMinuteSetScreen = false;
        } else if (isDaySetScreen) {
          DateTime now = rtc.now();
          rtc.adjust(DateTime(now.year(), now.month(), newDay, now.hour(), now.minute(), now.second()));
          isDaySetScreen = false;
        } else if (isMonthSetScreen) {
          DateTime now = rtc.now();
          rtc.adjust(DateTime(now.year(), newMonth, now.day(), now.hour(), now.minute(), now.second()));
          isMonthSetScreen = false;
        } else if (isYearSetScreen) {
          DateTime now = rtc.now();
          rtc.adjust(DateTime(newYear, now.month(), now.day(), now.hour(), now.minute(), now.second()));
          isYearSetScreen = false;
        } else if (isAlarmSetScreen) {
          // Save alarm and return to main screen
          isAlarmSetScreen = false;
          isTemperatureScreen = false;
          isTimeSetScreen = false;
          isMinuteSetScreen = false;
          isDaySetScreen = false;
          isMonthSetScreen = false;
          isYearSetScreen = false;
          Serial.println("Alarm set!");
        }
        // Return to main screen after setting any value
        isTemperatureScreen = false;
        break;

      default:
        if (isTimeSetScreen && key == Key17::KEY_UP) {
          newHour = (newHour + 1) % 24;
        }
        if (isTimeSetScreen && key == Key17::KEY_DOWN) {
          newHour = (newHour - 1 + 24) % 24;
        }
        if (isMinuteSetScreen && key == Key17::KEY_UP) {
          newMinute = (newMinute + 1) % 60;
        }
        if (isMinuteSetScreen && key == Key17::KEY_DOWN) {
          newMinute = (newMinute - 1 + 60) % 60;
        }
        if (isDaySetScreen && key == Key17::KEY_UP) {
          newDay = (newDay % 31) + 1;
        }
        if (isDaySetScreen && key == Key17::KEY_DOWN) {
          newDay = (newDay - 1 > 0) ? newDay - 1 : 31;
        }
        if (isMonthSetScreen && key == Key17::KEY_UP) {
          newMonth = (newMonth % 12) + 1;
        }
        if (isMonthSetScreen && key == Key17::KEY_DOWN) {
          newMonth = (newMonth - 1 > 0) ? newMonth - 1 : 12;
        }
        if (isYearSetScreen && key == Key17::KEY_UP) {
          newYear++;
        }
        if (isYearSetScreen && key == Key17::KEY_DOWN) {
          newYear--;
        }
         if (isAlarmSetScreen) {
          // Adjust alarm hour or minute based on user input
          if (key == Key17::KEY_UP) {
            if (isSettingAlarmHour) {
              alarmHour = (alarmHour + 1) % 24; // Increment hour
            } else {
              alarmMinute = (alarmMinute + 5) % 60; // Increment minute
            }
          } else if (key == Key17::KEY_DOWN) {
            if (isSettingAlarmHour) {
              alarmHour = (alarmHour - 1 + 24) % 24; // Decrement hour
            } else {
              alarmMinute = (alarmMinute - 5 + 60) % 60; // Decrement minute
            }
          } else if (key == Key17::KEY_RIGHT) {
            // Toggle between setting hour and minute
            isSettingAlarmHour = !isSettingAlarmHour;
          }
        }
        break;
    }
  }

  if (isTimeSetScreen) {
    showTimeSetScreen();
  } else if (isMinuteSetScreen) {
    showMinuteSetScreen();
  } else if (isDaySetScreen) {
    showDaySetScreen();
  } else if (isMonthSetScreen) {
    showMonthSetScreen();
  } else if (isYearSetScreen) {
    showYearSetScreen();
  } else if (isTemperatureScreen) {
    showTemperatureScreen();
  } else if (isAlarmSetScreen) {
    showAlarmSetScreen();
  } else {
    showMainScreen();
  }
   if(rtc.now().hour() == alarmHour && rtc.now().minute() == alarmMinute && rtc.now().second() == 0){
    displayAlarmScreen();
    ringBuzzerWithLowFrequency(3000);
    showMainScreen();
   }

  int buttonState = digitalRead(buttonPin);
   if (buttonState == LOW){
    alarmHour = 0;
    alarmMinute = 0;
   }

}

void showMainScreen() {
  DateTime now = rtc.now();

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print(now.hour());
  tft.print(":");
  if (now.minute() < 10) tft.print("0");
  tft.print(now.minute());
  tft.print(":");
  if (now.second() < 10) tft.print("0");
  tft.print(now.second());

  tft.setCursor(10, 50);
  tft.setTextSize(1);
  //int index = zellerDayOfWeek(rtc.now().year(), rtc.now().month(), rtc.now().day());
  //tft.println(zile[index]);
  tft.print(now.day());
  tft.print("/");
  tft.print(now.month());
  tft.print("/");
  tft.print(now.year());

  drawClock(now.hour(), now.minute(), now.second());

  // Check if alarm is set and display the "Alarm Set" notification in the bottom left
  if (alarmHour != 0 || alarmMinute != 0) {  // If alarm time is set
    tft.setTextSize(1); // Small font size
    tft.setTextColor(ST77XX_WHITE); // Color for alarm text
    tft.setCursor(10, 80); // Position in the bottom left
    tft.print("Alarm Set: ");
    tft.setCursor(10, 90);
    tft.print(alarmHour < 10 ? "0" : "");  // Format hour with leading zero
    tft.print(alarmHour);
    tft.print(":");
    tft.print(alarmMinute < 10 ? "0" : "");  // Format minute with leading zero
    tft.print(alarmMinute);
  }

  delay(1000);
}

void showTemperatureScreen() {
  float temperature = dht.readTemperature();

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.print(temperature);

  int circleX = 90;
  int circleY = 20;
  int circleRadius = 4;
  tft.drawCircle(circleX, circleY, circleRadius, ST77XX_WHITE);
  tft.setCursor(circleX + 8, 15);
  tft.print("C");

  drawThermometer(temperature);

  delay(1000);
}

void showTimeSetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Hour:");
  tft.setCursor(10, 50);
  tft.print(newHour);

  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Press OK to confirm");
  delay(1000);
}

void showMinuteSetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Minute:");
  tft.setCursor(10, 50);
  tft.print(newMinute);

  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Press OK to confirm");
  delay(1000);
}

void showDaySetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Day:");
  tft.setCursor(10, 50);
  tft.print(newDay);

  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Press OK to confirm");
  delay(1000);
}

void showMonthSetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Month:");
  tft.setCursor(10, 50);
  tft.print(newMonth);

  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Press OK to confirm");
  delay(1000);
}

void showYearSetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Year:");
  tft.setCursor(10, 50);
  tft.print(newYear);

  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Press OK to confirm");
  delay(1000);
}

void drawThermometer(float temperature) {
  int x = 60;
  int y = 50;
  int height = map(temperature, 0, 40, 0, 40);

  tft.drawRect(x, y, 15, 50, ST77XX_WHITE);
  tft.fillRect(x + 5, y + (50 - height), 5, height, ST77XX_RED);

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(x - 10, y + 55);
  tft.print("C");
}

void drawClock(int hour, int minute, int second) {
  int centerX = 100;
  int centerY = 100;
  int clockRadius = 25;

  tft.drawCircle(centerX, centerY, clockRadius, ST77XX_WHITE);

  float angleHour = radians((hour % 12) * 30 - 90);
  float angleMinute = radians(minute * 6 - 90);
  float angleSecond = radians(second * 6 - 90);

  int hourX = centerX + cos(angleHour) * (clockRadius * 0.4);
  int hourY = centerY + sin(angleHour) * (clockRadius * 0.4);
  tft.drawLine(centerX, centerY, hourX, hourY, ST77XX_WHITE);

  int minuteX = centerX + cos(angleMinute) * (clockRadius * 0.6);
  int minuteY = centerY + sin(angleMinute) * (clockRadius * 0.6);
  tft.drawLine(centerX, centerY, minuteX, minuteY, ST77XX_WHITE);

  int secondX = centerX + cos(angleSecond) * (clockRadius * 0.8);
  int secondY = centerY + sin(angleSecond) * (clockRadius * 0.8);
  tft.drawLine(centerX, centerY, secondX, secondY, ST77XX_RED);
} 

void showAlarmSetScreen() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Alarm:");

  // Display hour and minute with a colon separator
  tft.setCursor(10, 50);
  if (isSettingAlarmHour) {
    tft.setTextColor(ST77XX_MAGENTA); // Highlight the hour if it's being adjusted
  } else {
    tft.setTextColor(ST77XX_WHITE);
  }
  tft.print(alarmHour < 10 ? "0" : "");
  tft.print(alarmHour);

  tft.print(":");

  if (!isSettingAlarmHour) {
    tft.setTextColor(ST77XX_MAGENTA); // Highlight the minute if it's being adjusted
  } else {
    tft.setTextColor(ST77XX_WHITE);
  }
  tft.print(alarmMinute < 10 ? "0" : "");
  tft.print(alarmMinute);
  
  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.print("Use UP/DOWN to change");
  tft.setCursor(10, 100);
  tft.print("Use RIGHT to toggle");
  tft.setCursor(10, 120);
  tft.print("Press OK to confirm");

  delay(1000); // Slow down updates for clarity
}

void displayAlarmScreen() {
  tft.fillScreen(ST77XX_MAGENTA); // Fill the screen with red color

  tft.setTextColor(ST77XX_WHITE); // Set text color to white for better contrast
  tft.setTextSize(3); // Set text size (adjust as needed)
  tft.setCursor(20, 30); // Set cursor position (adjust coordinates as needed)
  tft.print("RING!");

  tft.setTextColor(ST77XX_WHITE); // Set text color to white for better contrast
  tft.setTextSize(2); // Set text size (adjust as needed)
  tft.setCursor(20, 60); // Set cursor position (adjust coordinates as needed)
  tft.print("RING!");

  tft.setTextColor(ST77XX_WHITE); // Set text color to white for better contrast
  tft.setTextSize(1); // Set text size (adjust as needed)
  tft.setCursor(20, 80); // Set cursor position (adjust coordinates as needed)
  tft.print("RING!");
  // Optional: Delay for a moment to keep the display on or update periodically if called repeatedly
}

void ringBuzzerWithLowFrequency(int duration_ms) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration_ms) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(LOW_FREQUENCY_DELAY * 100); // ON state
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(LOW_FREQUENCY_DELAY * 100); // OFF state
  }
}

void resetAlarm() {
  alarmHour = 0;
  alarmMinute = 0;
  Serial.println("Alarm reset!");
  isAlarmSetScreen = false;
  isTemperatureScreen = false;
  isTimeSetScreen = false;
  isMinuteSetScreen = false;
  isDaySetScreen = false;
  isMonthSetScreen = false;
  isYearSetScreen = false;
}

// int zellerDayOfWeek(int year, int month, int day) {
//     // Adjustments for January and February in Zeller's algorithm
//     if (month == 1 || month == 2) {
//         month += 12;
//         year -= 1;
//     }
    
//     int K = year % 100;        // Year of the century
//     int J = year / 100;        // Zero-based century

//     // Zeller's Congruence formula
//     int f = day + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) - 2 * J;
//     int dayOfWeek = (f + 6) % 7;  // Shift by +6 to align 0 with Sunday

//     // Ensure dayOfWeek is non-negative
//     if (dayOfWeek < 0) {
//         dayOfWeek += 7;
//     }

//     return dayOfWeek;
//}