#

## Cách tạo 1 project Realtime Database trên firebase và cách setup để lấy được 4 macro đưa vào firmware

Cấu hình cho Firebase trên ESP32 với Realtime Database gồm 4 macro chính, các macro này được config trong Common_Config.cpp

### 1. Create Firebase project + Realtime Database

- Vào trang Firebase Console: đăng nhập Google → chọn Add project / Create project.

- Đặt Project name (ví dụ: iot-system-monitor) → Continue → có thể tắt Google Analytics nếu không cần → Create project.

- Sau khi tạo xong, vào project đó:

  - Menu trái chọn Build → Realtime Database.

  - Bấm Create Database.

  - Chọn location (khu vực gần Việt Nam, ví dụ asia-southeast1 nếu có).

  - Chọn Start in test mode (để dễ thử, sau này sẽ chỉnh lại rules) → Enable.

### 2. Get FIREBASE_API_KEY

- Trong project Firebase, vào Project settings (icon bánh răng ở góc trên trái, cạnh Project Overview).

- Tab General.

- Kéo xuống phần Your apps (Web, Android, iOS). Nếu chưa có app Web:

  - Bấm icon </> (Web) → đặt tên app (ví dụ web-dashboard) → Register app → Continue tới phần có đoạn code JS.

- Ở dưới đoạn const firebaseConfig = { ... } sẽ thấy:

  ```cpp
   const firebaseConfig = {     apiKey: "xxxxxxxxxxxxxxxxxxxx",     ...   };
   ```

- Lấy giá trị apiKey đó gán vào trong file Common_Config.cpp:

  ```cpp
  const char* FIREBASE_API_KEY = "xxxxxxxxxxxxxxxxxxxx";
  ```

### 3. Lấy FIREBASE_DATABASE_URL

- Vào Build → Realtime Database → tab Data.

- Ở trên cùng sẽ có URL dạng:

```text
https://<project-id>-default-rtdb.<region>.firebasedatabase.app/
```

- Copy nguyên URL đó và dùng cho trong file Common_Config.cpp:

```cpp
const char* FIREBASE_DATABASE_URL = "https://<project-id>-default-rtdb.<region>.firebasedatabase.app/";
```

- Vào Build → Realtime Database → tab Rule.

- Chỉnh lại các quyền đọc ghi thành `true` -> Publish.

### 4. Bật đăng nhập Email/Password + tạo user

- Vào Build → Authentication.

- Tab Sign-in method:

  - Bật Email/Password → Save.

- Tab Users:

  - Bấm Add user.

  - Nhập Email (ví dụ `esp32-device@example.com`) và Password (mạnh, dễ nhớ) → Add user.

- Giờ bạn đã có:

```cpp
FIREBASE_USER_EMAIL = "esp32-device@example.com"
FIREBASE_USER_PASSWORD = "mật_khẩu_bạn_đặt"
```

- Điền vào Common_Config.cpp:

```cppcpp
const char* FIREBASE_USER_EMAIL = "esp32-device@example.com";const char* FIREBASE_USER_PASSWORD = "your_password_here";
```

### 5. Chỉ định đường dẫn gốc để lưu dữ liệu trên Realtime Database trong Common_Config.h

```cpp
#define FIREBASE_ROOT_PATH         "/sensorData"
```

## Dùng Thư viện

- mobizt/Firebase ESP32 Client@^4.4.17

```cpp
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
```

## Code

- Khởi tạo các đối tượng từ Class firebase

```cpp
static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;
```

- Muốn in lỗi khi kết nối Firebase : `Serial.println(fbdo.errorReason());`

- Khi bắt đầu kết nối Firebase, phải chắc rằng đã kết nối với WiFi, nếu mất kết nối cho phép kết nối lạilại : `Firebase.reconnectWiFi(true);`

- Cấu hình cho kết nối Firebase

```cpp
    // Cấu hình API key & URL Realtime Database
    config.api_key = FIREBASE_API_KEY;
    config.database_url = FIREBASE_DATABASE_URL;

    // Cấu hình tài khoản user (Email/Password)
    auth.user.email = FIREBASE_USER_EMAIL;
    auth.user.password = FIREBASE_USER_PASSWORD;

    // Callback để in trạng thái token (có sẵn trong thư viện)
    config.token_status_callback = tokenStatusCallback;
```

- Bắt đầu kết nối Firebase : `Firebase.begin(&config, &auth);`

- Kiểm tra trạng thái kết nối : `Firebase.ready()`

- Muốn gửi dữ liệu lên Firebase từ phần cứng:

```cpp
Firebase.setFloat(fbdo, tempPath, temperature);
```

- Ngoài ra còn có các kiểu dữ liệu khác, muốn gửi kiểu dữ liệu nào chỉ cần thay set<Kiểu dữ liệu> là gửi được

- **Lưu ý quan trọng**:

  - Xây dựng đường dẫn path để chỉ định gửi dữ liệu đến

  ```cpp
    // Tạo path theo timestamp: /sensorData/<millis()>/...
    // Dùng char[] buffer để tránh xung đột overload giữa String và MB_String
    // Tạo path trực tiếp: /sensorData/temperature và /sensorData/humidity
    char tempPath[128];
    char humiPath[128];
    snprintf(tempPath, sizeof(tempPath), "%s/temperature", FIREBASE_ROOT_PATH);
    snprintf(humiPath, sizeof(humiPath), "%s/humidity", FIREBASE_ROOT_PATH);

    Serial.print("[Firebase] Updating data at: ");
    Serial.println(FIREBASE_ROOT_PATH);
    ```

- **Vấn đề**: Firebase ESP32 Client library định nghĩa MB_String với các toán tử + riêng, gây xung đột khi bạn dùng String của Arduino để nối chuỗi.

- **Giải pháp**: Dùng char[] buffer và snprintf() để build path, sau đó truyền const char* trực tiếp vào Firebase API. Cách này tránh hoàn toàn việc dùng String và MB_String trong phép nối chuỗi.
