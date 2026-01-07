#include <Arduino.h>
#include <Common_Config.h>
#include <Connect_Wifi.h>
#include <DHT11_Sensor.h>
#include <OLED_I2C.h>
#include <Send_Data_Firebase.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Stack size - QUAN TRỌNG: Tăng mạnh cho task dùng Firebase
const uint32_t WIFI_TASK_STACK_SIZE    = 4096;
const uint32_t SENSOR_TASK_STACK_SIZE  = 12288;  // Tối thiểu 12KB cho Firebase
const uint32_t DISPLAY_TASK_STACK_SIZE = 4096;

// Priority
const BaseType_t WIFI_TASK_PRIORITY    = tskIDLE_PRIORITY + 3;
const BaseType_t SENSOR_TASK_PRIORITY  = tskIDLE_PRIORITY + 2;
const BaseType_t DISPLAY_TASK_PRIORITY = tskIDLE_PRIORITY + 1;

// Struct dữ liệu sensor
typedef struct {
    float temperature;
    float humidity;
    bool is_valid;
} SensorData;

// Global variables
static SensorData g_sensor_data = {0.0f, 0.0f, false};
static unsigned long g_last_sensor_read_time = 0;
static unsigned long g_last_display_update_time = 0;

// Handle for Mutex
static SemaphoreHandle_t g_data_mutex = NULL;

// Task handles
TaskHandle_t g_wifi_task_handle = NULL;
TaskHandle_t g_sensor_task_handle = NULL;
TaskHandle_t g_display_task_handle = NULL;

// Prototypes
void manager_wifi_status(void);
void read_and_upload_sensor_data(void);
void update_oled_display(void);
void wifi_task(void *pvParameters);
void sensor_task(void *pvParameters);
void display_task(void *pvParameters);

void setup() {
    Serial.begin(115200);
    delay(100);  // Dùng delay ngắn ở setup vẫn ổn

    Serial.println("\n=== ESP32 Temperature & Humidity Monitor ===");

    // Tạo mutex
    g_data_mutex = xSemaphoreCreateMutex();
    if (g_data_mutex == NULL) {
        Serial.println("FATAL: Failed to create mutex!");
        while (1);
    }

    // Khởi tạo các module theo thứ tự đúng
    oled_init();

    // === QUAN TRỌNG: Xóa màn hình khởi động và hiển thị mặc định ===
    oled_clear();  // Nếu thư viện có hàm clear
    // Hoặc nếu không có clear, thì hiển thị thông báo ban đầu
    oled_display_message("Waiting for sensor...", 0, 20);
    oled_display_message("WiFi: Connecting...", 0, 100);

    wifi_init();
    firebase_init();  // Phải sau wifi_init()
    dht_init();

    // Tạo tasks
    BaseType_t status;

    status = xTaskCreatePinnedToCore(wifi_task, "WifiTask", WIFI_TASK_STACK_SIZE, NULL, WIFI_TASK_PRIORITY, &g_wifi_task_handle, 0);
    if (status != pdPASS) Serial.println("Error creating WiFi task");

    status = xTaskCreatePinnedToCore(sensor_task, "SensorTask", SENSOR_TASK_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &g_sensor_task_handle, 1);
    if (status != pdPASS) Serial.println("Error creating Sensor task");

    status = xTaskCreatePinnedToCore(display_task, "DisplayTask", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &g_display_task_handle, 1);
    if (status != pdPASS) Serial.println("Error creating Display task");

    Serial.println("System initialization complete!");
}

void loop() {
    // FreeRTOS quản lý hết
}

// Helper functions
void manager_wifi_status(void) {
    pinMode(LED_STATUS_WIFI, OUTPUT);
    if (wifi_is_connected()) {
        digitalWrite(LED_STATUS_WIFI, HIGH);
    } else {
        digitalWrite(LED_STATUS_WIFI, LOW);
        wifi_reconnect();
    }
}

void read_and_upload_sensor_data(void) {
    unsigned long current_time = millis();

    if (xSemaphoreTake(g_data_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
        if (current_time - g_last_sensor_read_time >= SENSOR_READ_INTERVAL) {
            float temp = dht_read_temperature();
            float hum = dht_read_humidity();

            if (dht_is_valid_read(temp, hum)) {
                g_sensor_data.temperature = temp;
                g_sensor_data.humidity = hum;
                g_sensor_data.is_valid = true;

                Serial.printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temp, hum);

                // Chỉ gửi Firebase nếu có WiFi
                if (wifi_is_connected()) {
                    firebase_send_sensor_data(temp, hum);
                }
            } else {
                Serial.println("Invalid sensor reading!");
                g_sensor_data.is_valid = false;
            }

            g_last_sensor_read_time = current_time;
        }
        xSemaphoreGive(g_data_mutex);
    }
}

void update_oled_display(void) {
    unsigned long current_time = millis();

    if (xSemaphoreTake(g_data_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
        if (current_time - g_last_display_update_time >= DISPLAY_UPDATE_INTERVAL_MS) {
            if (g_sensor_data.is_valid) {
                oled_display_data(g_sensor_data.temperature, g_sensor_data.humidity, 0, 0);
            } else {
                oled_display_message("Sensor Error!", 0, 0);
            }

            g_last_display_update_time = current_time;
        }
        xSemaphoreGive(g_data_mutex);
    }
}

// Tasks
void wifi_task(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        manager_wifi_status();
        vTaskDelay(pdMS_TO_TICKS(WIFI_CHECK_INTERVAL_MS));
    }
}

void sensor_task(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        read_and_upload_sensor_data();
        vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_INTERVAL));
    }
}

void display_task(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        update_oled_display();
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
    }
}