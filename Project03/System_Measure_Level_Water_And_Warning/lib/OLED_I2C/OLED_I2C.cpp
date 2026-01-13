#include "OLED_I2C.h"
#include "Common_Config.h"

// initial OLED object from Adafruit_SSD1306 Class
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void oled_init() {
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        while(1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("System Starting...");
    display.display();

    Serial.println("OLED initialized");
}

void oled_display_data(float level_water, uint8_t col, uint8_t row) {
    display.clearDisplay();
    display.setCursor(col, row);
    
    display.setTextSize(1);
    display.println("Current Level Water");
    display.println("---------------");
    display.println();
    
    display.setTextSize(1);
    display.print("Level_Water: ");
    display.print(level_water, 1);
    display.println(" cm");
    
    display.display();
}

void oled_display_message(const char* message, int textSize, uint8_t col, uint8_t row) {
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(col, row);
    
    display.println(message);
    
    display.display();
}

void oled_clear() {
    display.clearDisplay();
    display.display();
}
