#include "Common_Config.h"

const char* SSID_WIFI = "Thanhtan";
const char* PASSWORD_WIFI = "12345678";

const uint32_t WIFI_CHECK_INTERVAL_MS = 2000;
const uint32_t DISPLAY_UPDATE_INTERVAL_MS = 1000;

// ================================
// Cấu hình Firebase (bạn sửa lại cho đúng với project của mình)
// ================================

// Lấy ở Firebase console: Project settings -> General -> Web API key
const char* FIREBASE_API_KEY = "AIzaSyCVYW7FzFaTzUeTvZF7HE-n9Qi1LXLYcfY";

// Lấy ở Firebase console: Realtime Database -> Data -> URL
// Ví dụ: "https://your-project-id-default-rtdb.asia-southeast1.firebasedatabase.app/"
const char* FIREBASE_DATABASE_URL = "https://system-monitor-temp-and-humi-default-rtdb.firebaseio.com/";

// Tài khoản đăng nhập (Email/Password) đã tạo trong Authentication -> Sign-in method -> Email/Password
const char* FIREBASE_USER_EMAIL = "systemmonitortempandhumi@gmail.com";
const char* FIREBASE_USER_PASSWORD = "Project01";
