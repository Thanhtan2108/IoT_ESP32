#

## 1. Các khái niệm quan trọng

`WebServer server(80)` — thư viện `ESP8266WebServer` / `WebServer` cho ESP32: lắng nghe HTTP port 80, nếu có request thì `server.handleClient()` xử lý.

`server.on(path, method, handler)` — đăng một route; handler là hàm callback chạy khi có request match.

`server.send(code, contentType, payload)` — gửi response. Bạn có thể thêm header bằng `server.sendHeader(...)`.

`PROGMEM` + `FPSTR()` — để lưu chuỗi HTML vào Flash (ROM) tránh tốn RAM. `String(FPSTR(MAIN_page))` copy chuỗi từ Flash về RAM.

`server.sendHeader("Location", "/", true); server.send(303, "text/plain", "");` — redirect (sau khi bấm /on hoặc /off, browser được chuyển về trang chính). 303 là status code Redirect.

## 2. Những vấn đề bạn nên biết

Không dùng JSON library: bạn build JSON bằng String thủ công — ok cho 1-2 trường, nhưng khi nhiều trường hoặc parse POST, dùng `ArduinoJson` sẽ sạch và an toàn hơn.

## 3. Cải tiến nhỏ — ví dụ thực tế bạn có thể áp dụng ngay

Thêm route để tắt/bật relay pump/fan.

Trả JSON chứa nhiều trường: `{"led":true,"fan":false,"pump":true,"ip":"192.168.1.10"}`

Thêm route `POST /control` nhận form/json để điều khiển nhiều thiết bị cùng lúc.

Dùng AJAX (fetch) ở trang web để cập nhật realtime.

## 4. Làm rõ 1 số điểm nghẽn trong suy nghĩ logic code

### 1️⃣ %LED%, %FAN%, %PUMP% KHÔNG phải HTML

Trước hết, cần khắc vào đầu điều này:

  ❌ %LED% không phải cú pháp HTML

  ❌ Browser không hiểu %LED%
  
  ❌ JavaScript cũng không hiểu %LED%

👉 %LED% chỉ là một chuỗi ký tự bình thường do bạn tự đặt ra.

### 2️⃣ Vậy %LED% dùng để làm gì?

Nó được dùng làm placeholder (chỗ trống) để ESP32 thay thế nội dung trước khi gửi HTML cho trình duyệt.

📌 Nghĩ theo đúng luồng xử lý:

```scss
ESP32 (C++)  --->  Browser (HTML)
```

Browser không hề biết `%LED%` tồn tại.

ESP32 phải xử lý nó trước.

### 3️⃣ Toàn bộ cơ chế hoạt động (cực kỳ quan trọng)

Xem lại đoạn code này:

```cpp
String generateMain() {
  String s = String(FPSTR(MAIN_PAGE));
  s.replace("%LED%", ledState ? "ON" : "OFF");
  s.replace("%FAN%", fanState ? "ON" : "OFF");
  s.replace("%PUMP%", pumpState ? "ON" : "OFF");
  return s;
}
```

📌 Phân tích từng bước:

Bước 1: HTML gốc trong Flash

```html
LED: <span id="led">%LED%</span>
```

Bước 2: ESP32 copy HTML vào RAM

```cpp
String s = String(FPSTR(MAIN_PAGE));
```

Lúc này s vẫn là:

```html
LED: <span id="led">%LED%</span>
```

Bước 3: ESP32 thay `%LED%`

```cpp
s.replace("%LED%", "ON");
```

Sau khi replace:

```html
LED: <span id="led">ON</span>
```

Bước 4: Gửi sang trình duyệt

```cpp
server.send(200, "text/html", s);
```

👉 Browser chỉ nhận HTML đã hoàn chỉnh, không thấy `%LED%` nữa.

### 4️⃣ Vì sao không viết luôn ON / OFF trong HTML?

Vì trạng thái LED là biến động, phụ thuộc vào firmware:

- LED có thể ON / OFF

- Fan có thể ON / OFF

- Pump có thể ON / OFF

- Sensor có thể thay đổi liên tục

👉 HTML trong Flash là template

👉 ESP32 là engine render HTML

Cách này giống PHP / JSP / Django template, nhưng bạn đang làm thủ công.

### 5️⃣ So sánh để bạn hiểu sâu (rất quan trọng)

| Cách                 | Ai xử lý    | Khi nào            |
| -------------------- | ----------- | ------------------ |
| `%LED%`              | ESP32 (C++) | Trước khi gửi HTML |
| JavaScript `fetch()` | Browser     | Sau khi page load  |
| HTML thuần           | Browser     | Không có logic     |

Bạn đang dùng 2 tầng:

1️⃣ Server-side rendering (ESP32 replace %LED%)

2️⃣ Client-side update (fetch('/status'))

### 6️⃣ Tại sao cần cả %LED% và JavaScript fetch?

Bạn có đoạn JS này:

```js
async function updateStatus() {
  const res = await fetch('/status');
  const data = await res.json();
  document.getElementById('led').textContent =
      data.led ? 'ON' : 'OFF';
}
```

📌 Vai trò từng phần:

`%LED%`

- Hiển thị trạng thái ban đầu

- Ngay khi mở trang, không cần đợi JS

`fetch('/status')`

- Cập nhật realtime

- Khi LED thay đổi, UI tự cập nhật

👉 Nếu không có `%LED%`:

- Trang load → trống

- Chờ JS chạy mới thấy trạng thái

👉 Nếu không có fetch:

- Trang chỉ đúng lúc load

- Không realtime

### 7️⃣ `%LED%` có thể đổi thành bất kỳ chuỗi nào

Ví dụ tất cả đều hợp lệ:

```html
{{LED}}
__LED__
@@LED@@
###LED###
<%LED%>
```

Chỉ cần C++ replace đúng chuỗi:

```cpp
s.replace("{{LED}}", "ON");
```

👉 `%LED%` chỉ là quy ước do bạn chọn, không phải chuẩn web.

### 8️⃣ Tư duy đúng khi viết Web Server trên ESP32

Hãy nghĩ như sau:

  🧠 ESP32 không chạy HTML

  🧠 ESP32 chỉ tạo ra chuỗi HTML

  🧠 Browser mới là thứ chạy HTML + JS

ESP32 chỉ làm:

```cpp
String html = "...";
html.replace("XXX", value);
server.send(html);
```
