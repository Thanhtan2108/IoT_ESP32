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

void oled_display_data(float temperature, float humidity, uint8_t row, uint8_t col) {
    display.clearDisplay();
    display.setCursor(row, col);
    
    display.setTextSize(1.5);
    display.println("Temp & Humidity");
    display.println("---------------");
    display.println();
    
    display.setTextSize(2);
    display.print("T: ");
    display.print(temperature, 1);
    display.println(" C");
    
    display.print("H: ");
    display.print(humidity, 1);
    display.println(" %");
    
    display.display();
}

void oled_display_message(const char* message, uint8_t row, uint8_t col) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(row, col);
    
    display.println(message);
    
    display.display();
}

void oled_clear(void) {
    display.clearDisplay();
    display.display();
}
