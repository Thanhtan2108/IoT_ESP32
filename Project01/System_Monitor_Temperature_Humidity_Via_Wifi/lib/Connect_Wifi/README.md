#

Đây là module phát triển cho chức năng kết nối WiFi

Gồm 3 API chính:

- Khởi tạo kết nối WiFi:

  - Hàm `wifi_init`

  - Set chế độ kết nối Router/Điện thoại : `WiFi.mode(WIFI_STA);`

  - Tiến hành kết nối với WiFi thông qua SSID, PASSWORD : `WiFi.begin(SSID, PASSWORD);`

  - **Lưu ý**:

    - `SSID` và `PASSWORD` đều phải là kiểu `char*`
  
    - Nên dùng `const char*` để set đây là giá trị hằng

  - Dùng `WiFi.status()` để quản lý trạng thái kết nối WiFi : `WiFi.status() == WL_CONNECTED;` nghĩa là kết nối thành công. `WiFi.status() != WL_CONNECTED;` nghĩa là kết nối không thành công.

- Kiểm tra wifi đã được kết nối chưa:

  - Hàm `wifi_is_connected`

  - Hàm kiểm tra xem wifi đã được kết nối chưa.

- Kết nối lại với wifi khi bị mất kết nối

  - Hàm `wifi_reconnect`

  - `WiFi.disconnect();` để ngắt kết nối với wifi đang mất kết nối

  - `WiFi.reconncet();` để kết nối lại với mạng wifi đó
