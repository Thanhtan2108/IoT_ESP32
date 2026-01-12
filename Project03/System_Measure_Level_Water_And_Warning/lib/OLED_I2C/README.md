#

Đây là module phát triển cho chức năng hiển thị lên màn hình OLED 0.96 inch

Khởi tạo 1 đối tượng cho màn hình OLED từ Adafruit_SSD1306 class : `Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);`

Các API chính:

- Khởi tạo màn hình oled:

  - Hàm `oled_init`

  - Sử dụng I2C để kết nối tới màn hình : `Wire.begin(SDA_PIN, SCL_PIN);`

  - Sau đó kiểm tra xem việc khởi tạo màn hình thành công chưa qua API có sẵn từ thư viện : `if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))`

  - Tiếp theo là đặt các trạng thái hiển thị ban đầu của màn hình :

  ```cpp
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System Starting...");
  display.display();
  ```

- Ghi dữ liệu lên màn hình:

  - Hàm `oled_display_data`

  - Hiển thị data lên màn hình

  - Truyền vào data cần hiển thị, vị trí hàng và cột của con trỏ trên màn hình để hiển thị

  - Đầu tiên làm sạch màn hình trước khi hiển thị : `display.clearDisplay();`

  - Chỉ định vị trí hiển thị trên màn hình : `display.setCursor(row, col);`

  - Chỉ định kích thước của nội dung hiển thị lên màn hình : `display.setTextSize(size);`

  - Dùng `display.print("msg");` hoặc `display.println("msg");` để hiển thị nội dung lên màn

  - Sau khi thiết lập xong, hiển thị nội dung lên màn : `display.display();`

- Hiển thị message lên màn hình:

  - Hàm `oled_display_message`

  - Tương tự cách làm như hiển thị data

- Xóa màn hình:

  - Hàm `oled_clear();`

  - Xóa màn bằng : `display.clearDisplay();`

  - Hiển thị lại màn hình : `display.display();`
