#

## **CÁCH LÀM THẾ NÀO để xem được raw HTTP response**

### ✅ CÁCH 1 (QUAN TRỌNG NHẤT): Chrome DevTools – Network

Đây là cách chuẩn dev web và bạn nên dùng thường xuyên.

Các bước:

- Mở trang web ESP32

```cpp
http://<IP_ESP32>/
```

- Nhấn F12

- Chọn tab Network

- Reload trang (Ctrl + R)

#### Xem raw HTTP response

- Click vào request `/`

- Chọn tab `Headers`

Bạn sẽ thấy:

Response Headers

```yaml
HTTP/1.1 200 OK
Cache-Control: no-cache, no-store, must-revalidate
Pragma: no-cache
Expires: -1
Content-Type: text/html

Response (tab Response)
<html>...</html>
```

📌 Đây chính là thứ bạn hỏi

### ✅ CÁCH 2: Dùng curl (nhìn toàn bộ HTTP)

Trên Windows / Linux / Mac

```bash
curl -i http://<IP_ESP32>/
```

Output

```yaml
HTTP/1.1 200 OK
Cache-Control: no-cache, no-store, must-revalidate
Pragma: no-cache
Expires: -1
Content-Type: text/html

<html>...
```

📌 `-i` = include headers

## 0. phân biệt được Client – Browser – Server, sau này học thêm REST, MQTT, WebSocket, Flutter, Home Assistant…

### 1️⃣ Trước tiên: ĐỊNH NGHĨA CHUẨN (bất biến)

🔹 Server là gì?

> Server = Thực thể lắng nghe request và trả response

Trong project của bạn:

```cpp
WebServer server(80);
```

👉 ESP32 = SERVER

- Lắng nghe cổng 80

- Nhận HTTP request

- Xử lý

- Trả dữ liệu

🔹 Client là gì?

> Client = Thực thể chủ động gửi request đến server

⚠️ Client không phải là “web”

⚠️ Client không nhất thiết là trình duyệt

Client chỉ cần:

- Gửi HTTP request

- Nhận HTTP response

🔹 Browser là gì?

> Browser = một loại client đặc biệt

- Có UI

- Chạy HTML / CSS / JS

- Tự động gửi HTTP request khi:

  - Gõ URL

  - Bấm nút

  - Gọi `fetch()`

👉 Browser ⊂ Client

### 2️⃣ ÁP DỤNG TRỰC TIẾP VÀO PROJECT CỦA BẠN

Bây giờ ta gắn nhãn đúng cho từng thành phần.

🟢 ESP32 (WebServer)

```cpp
WebServer server(80);
```

- Vai trò: SERVER

- Nhiệm vụ:

  - Chờ request

  - Route

  - Điều khiển GPIO

  - Trả HTML / JSON

👉 ESP32 KHÔNG BAO GIỜ là client trong project này

🔵 Browser (Chrome / Edge / Firefox)

Khi bạn mở, ví dụ:

```cpp
http://192.168.1.100/
```

👉 Browser làm:

```sql
GET /
```

👉 Browser = CLIENT

Browser vừa là:

- HTTP Client

- HTML Renderer

- JavaScript Runtime

🟡 JavaScript trong HTML

```js
fetch('/status')
```

📌 Ai gọi `fetch()`?

- JavaScript chạy trong Browser

📌 Gửi request từ đâu?

- Từ Browser

👉 JS KHÔNG phải server
👉 JS KHÔNG phải ESP32
👉 JS là client-side code

🟠 Fetch có phải là client không?

> ❗ Không

`fetch()` chỉ là API của Browser

👉 Browser mới là client

### 3️⃣ BẢNG PHÂN BIỆT RÕ RÀNG

| Thành phần    | Là client? | Là server? | Ghi chú            |
| ------------- | ---------- | ---------- | ------------------ |
| ESP32         | ❌         | ✅         | Web server         |
| Browser       | ✅         | ❌         | Client             |
| JS trong HTML | ❌         | ❌         | Chạy trong browser |
| fetch()       | ❌         | ❌         | API gửi request    |
| App Flutter   | ✅         | ❌         | Client             |
| Postman       | ✅         | ❌         | Client             |
| Node-RED      | ✅         | ❌         | Client             |

### 4️⃣ NHẬN DIỆN ĐÚNG CLIENT TRONG CÁC TÌNH HUỐNG

#### Ví dụ 1: Mở trang web

```sql
User → Browser → ESP32
```

Client = Browser

Server = ESP32

#### Ví dụ 2: JavaScript auto refresh

```js
setInterval(fetch('/status'), 2000);
```

Client = Browser

Server = ESP32

#### Ví dụ 3: Gửi lệnh từ app Flutter

```bash
Flutter App → HTTP POST /control → ESP32
```

Client = Flutter App

Server = ESP32

#### Ví dụ 4: MQTT (sau này)

```nginx
ESP32 → MQTT Broker
```

👉 Lúc này:

- ESP32 = Client

- MQTT Broker = Server

📌 Vai trò PHỤ THUỘC NGỮ CẢNH

### 5️⃣ QUY TẮC VÀNG ĐỂ KHÔNG BAO GIỜ NHẦM

- 🔑 Ai là người gửi request → người đó là CLIENT

- 🔑 Ai lắng nghe và trả lời → người đó là SERVER

Không quan tâm:

- Thiết bị là gì

- Có UI hay không

### 6️⃣ MÔ HÌNH HOÀN CHỈNH CỦA PROJECT BẠN

```pgsql
┌────────────┐     HTTP GET /status     ┌────────────┐
│  Browser   │ ─────────────────────▶ │   ESP32    │
│ (Client)   │                          │  (Server)  │
│            │ ◀───────────────────── │            │
│            │     JSON / HTML         │            │
└────────────┘                          └────────────┘
```

### 7️⃣ Vì sao bạn PHẢI hiểu điều này?

Nếu không:

- Bạn sẽ nhầm lẫn khi học REST

- Nhầm khi học WebSocket

- Nhầm khi học MQTT

- Nhầm khi ESP32 vừa làm client vừa làm server

Nếu hiểu:

- Bạn tự thiết kế hệ thống

- Tự debug được request

- Tự mở rộng được project

### 8️⃣ TÓM TẮT SIÊU NGẮN (ghi vào vở)

- ESP32 (WebServer) = SERVER

- Browser = CLIENT

- JS chạy trong Browser

- fetch() = API gửi request

- Client = bên gửi request

- Server = bên nhận & trả response

- Vai trò thay đổi theo ngữ cảnh

## 1. Phân tích từng API (chi tiết) để xây dựng Web Server

### 1️⃣ `WebServer server(port)`

📌 Chức năng

- Tạo một HTTP server

- Lắng nghe request trên cổng `port`

📌 Cú pháp

```cpp
WebServer server(80);
```

📌 Tham số

| Tham số | Kiểu       | Ý nghĩa                       |
| ------- | ---------- | ----------------------------- |
| `port`  | `uint16_t` | Cổng HTTP (80 = mặc định web) |

📌 Runtime

- Chưa chạy server

- Chỉ tạo object

- Chưa chiếm tài nguyên mạng

### 2️⃣ `server.begin()`

📌 Chức năng

- Khởi động HTTP server

- Bind socket TCP

- Bắt đầu lắng nghe client

📌 Cú pháp

```cpp
server.begin();
```

📌 Runtime

- Sau khi gọi → ESP32 sẵn sàng nhận HTTP request

- Nếu chưa gọi `begin()` → server chết

### 3️⃣ `server.on(uri, method, handler)`

📌 Chức năng

- Đăng ký route (URL (http://[ip]+uri) → function (handler))

- Tạo bảng ánh xạ URI → handler

📌 Cú pháp

```cpp
server.on("/status", HTTP_GET, handleStatus);
```

📌 Tham số

| Tham số   | Kiểu          | Ý nghĩa                        |
| --------- | ------------- | ------------------------------ |
| `uri`     | `const char*` | Đường dẫn URL                  |
| `method`  | `HTTPMethod`  | HTTP_GET / POST / PUT / DELETE |
| `handler` | `void (*)()`  | Hàm callback                   |

📌 Runtime

- Không gọi handler ngay

- Chỉ lưu thông tin route

📌 Khi client gửi request (Khi tương tác trên Browser):

```bash
GET /status
```

Server sẽ:

- So URI

- So method

- Gọi handler phù hợp

### 4️⃣ `server.handleClient()`

📌 Chức năng

>🔥 TRÁI TIM CỦA WEB SERVER

- Kiểm tra có client (có request HTTP nào từ Browser) mới không

- Parse HTTP request

- Gọi handler tương ứng

📌 Cú pháp

```cpp
server.handleClient();
```

📌 Runtime

- Phải gọi liên tục trong `loop()`

- Nếu không → server không phản hồi

📌 Liên kết

```text
loop()
  ↓
handleClient()
  ↓
server.on(...)
  ↓
handler()
```

### 5️⃣ `server.send(status, contentType, payload)`

📌 Chức năng

- Gửi HTTP response từ ESP32 (server) về client (Browser)

📌 Cú pháp

```cpp
server.send(200, "text/html", html);
```

📌 Tham số

| Tham số       | Kiểu          | Ý nghĩa                                                |
| ------------- | ------------- | ------------------------------------------------------ |
| `status code` | `int`         | HTTP status code                                       |
| `contentType` | `const char*` | MIME type (Cho client biết: Dữ liệu trả về là loại gì) |
| `payload`     | `String`      | Nội dung response                                      |

#### Chi tiết từng tham số

🔹 Tham số 1: `200` → HTTP Status Code

##### 200 nghĩa là gì?

- 200 OK

→ Request hợp lệ

→ Server xử lý thành công

→ Có dữ liệu trả về

📦 Gửi cho client:

```text
HTTP/1.1 200 OK
```

##### Một số status code thường dùng

| Status Code | Ý nghĩa               | Khi dùng                        |
| ----------- | --------------------- | ------------------------------- |
| 200         | OK                    | Trả HTML / JSON bình thường     |
| 201         | Created               | Tạo tài nguyên mới              |
| 204         | No Content            | Thành công nhưng không trả data |
| 301         | Moved Permanently     | Redirect vĩnh viễn              |
| 302         | Found                 | Redirect tạm                    |
| 303         | See Other             | Redirect sau hành động          |
| 400         | Bad Request           | Client gửi sai                  |
| 401         | Unauthorized          | Chưa xác thực                   |
| 403         | Forbidden             | Bị cấm                          |
| 404         | Not Found             | Không có route                  |
| 500         | Internal Server Error | Lỗi server                      |

👉 Bạn hoàn toàn có thể đổi `200` sang giá trị khác tùy mục đích

🔹 Tham số 2: `"text/html"` → Content-Type (MIME type)

##### Content-Type là gì?

- Cho client biết: Dữ liệu trả về là loại gì

```http
Content-Type: text/html
```

#### Vì sao là "text/html"?

Vì bạn đang trả:

```html
<!DOCTYPE html>
<html>...</html>
```

👉 Browser đọc Content-Type:

- `text/html` → render thành web page

#### Một số Content-Type thường dùng

| Content-Type           | Dùng khi      |
| ---------------------- | ------------- |
| text/html              | Trả trang web |
| text/plain             | Trả text      |
| application/json       | Trả JSON      |
| application/xml        | Trả XML       |
| image/png              | Trả ảnh       |
| text/css               | Trả CSS       |
| application/javascript | Trả JS        |

📌 Ví dụ:

```cpp
server.send(200, "application/json", payload);
```

🔹 Tham số 3: `html` → Body (nội dung response)

#### Đây là gì?

- Dữ liệu thực tế server trả về

Có thể là:

- HTML

- JSON

- Text

- XML

📌 Ví dụ thực tế

```cpp
String html = "<h1>Hello ESP32</h1>";
server.send(200, "text/html", html);
```

ESP32 sẽ gửi về  nguyên văn gần như thế này:

```php-template
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234

<!DOCTYPE html>
<html>
  <head>...</head>
  <body>...</body>
</html>
```

Browser sẽ:

- Nhận HTML

- Render UI

📌 Ví dụ thực tế

```cpp
server.send(303, "text/plain", "");
```

ESP32 sẽ gửi về client nguyên văn gần như thế này:

```php-template
HTTP/1.1 303 See Other
Content-Type: text/plain
Content-Length: 0
```

> ❗ Không có body

📌 Ví dụ thực tế

```cpp
server.send(200, "application/json", "{\"led\":true}");
```

**Lưu ý: Vì sao phải có \ ?**

- `" "` là ký tự đặc biệt trong C/C++

- Muốn viết dấu `"` trong chuỗi → phải escape bằng `\`

-> ESP32 gửi JSON, không gửi C++ string

ESP32 sẽ gửi về client nguyên văn gần như thế này:

```php-template
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 12

{"led":true}
```

> Body là chuỗi C++ `{"led":true}` nhưng JSON thật là `{"led":true}`

#### Client nhận và xử lý như thế nào?

Trường hợp 1: Browser mở URL `/status`

- Browser thấy `application/json`

- KHÔNG render HTML

- Hiển thị raw JSON

📷 Bạn sẽ thấy:

```js
{"led":true}
```

Trường hợp 2: JavaScript fetch()

```js
const res = await fetch('/status');
const data = await res.json();
console.log(data.led);
```

Quá trình:

- Browser gửi HTTP GET `/status`

- ESP32 trả JSON

- JS đọc header → biết là JSON

- `res.json()` → parse (phân tích cú pháp) thành object

📌 `data = { led: true }`

#### Tại sao JSON lại quan trọng?

JSON là ngôn ngữ chung giữa:

- ESP32

- Web

- Mobile App

- Server

- Cloud

📌 ESP32 chỉ cần gửi JSON, client muốn làm gì thì làm

#### Liên hệ trực tiếp với code của bạn

Trong ESP32:

```cpp
void handleStatus() {
    server.send(200, "application/json",
                "{\"led\":true}");
}
```

Trong HTML:

```js
const r = await fetch('/status');
const j = await r.json();
document.getElementById('led').textContent =
    j.led ? 'ON' : 'OFF';
```

📌 ESP32 không biết HTML

📌 HTML không biết ESP32

📌 JSON là cầu nối

❓ `j.led` sẽ nhận giá trị là gì?

👉 `j.led` nhận giá trị kiểu `boolean` của JavaScript

Cụ thể:

```js
j.led === true
```

hoặc

```js
j.led === false
```

##### Tại sao lại như vậy?

ESP32 gửi:

```cpp
server.send(200, "application/json", "{\"led\":true}");
```

HTTP body là:

```json
{"led":true}
```

##### Trình duyệt xử lý như thế nào?

```js
const r = await fetch('/status');
const j = await r.json();
```

👉 `r.json()`:

- Đọc body

- Parse JSON

- Chuyển thành JavaScript Object

##### Kết quả thực tế trong JS

```js
j = {
  led: true
}
```

👉 Nên:

```js
j.led        // true
typeof j.led // "boolean"
```

##### So sánh rất quan trọng (đừng nhầm!)

✅ ĐÚNG – JSON boolean

```json
{"led":true}
```

```js
j.led === true      // true
```

❌ SAI – string "true"

```json
{"led":"true"}
```

```js
j.led === true      // false
j.led === "true"   // true
typeof j.led       // "string"
```

📌 Khác nhau hoàn toàn

##### Vì sao bạn dùng được toán tử ngôi ba `? :`

```js
j.led ? 'ON' : 'OFF'
```

Vì:

- `true` → ON

- `false` → OFF

##### Thử debug để “nhìn thấy tận mắt”

```js
console.log(j);
console.log(j.led);
console.log(typeof j.led);
```

Output:

```console
{led: true}
true
boolean
```

##### Nếu ESP32 gửi nhiều trường?

ESP32:

```json
{"led":true,"fan":false,"pump":true}
```

JS:

```js
j.led   // true
j.fan   // false
j.pump // true
```

##### Ghi nhớ một câu (rất quan trọng)

- JSON boolean → JavaScript boolean. Không phải string

#### Mô hình tư duy chuẩn (rất quan trọng)

```scss
[ Browser / App ]  <--HTTP+JSON-->  [ ESP32 ]
        ↑                                   ↑
   fetch(), UI                         server.send()
```

#### Nếu bạn gửi nhiều dữ liệu thì sao?

ESP32

```cpp
server.send(200, "application/json",
    "{\"led\":true,\"fan\":false,\"temp\":27.5}");
```

Client

```js
j.led
j.fan
j.temp
```

📌 JSON = key – value

#### Vì sao không điều khiển trực tiếp trong HTML?

❌ Sai tư duy:

```html
<button onclick="ledOn()">ON</button>
```

✔ Đúng:

```html
<button onclick="fetch('/led/on')">ON</button>
```

📌 HTML chỉ gửi request

📌 ESP32 quyết định logic

📌 JSON trả trạng thái

#### Ghi nhớ 1 câu duy nhất

- `server.send(200, "application/json", "...")`
= ESP32 đang đóng vai trò API server

### 6️⃣ `server.sendHeader(name, value, first)`

📌 Chức năng

- Thêm HTTP header trước khi `send()`

📌 Cú pháp

```cpp
server.sendHeader("Location", "/", true);
```

📌 Tham số

| Tham số | Ý nghĩa             |
| ------- | ------------------- |
| `name`  | Tên header          |
| `value` | Giá trị             |
| `first` | Gửi header đầu tiên |

📌 Dùng để redirect hoặc auth

#### Ví dụ 1 – Redirect về trang chính (thực tế nhất)

ESP32 code

```cpp
void handleLedOn() {
    digitalWrite(LED_PIN, HIGH);

    server.sendHeader("Location", "/", true);
    server.send(303, "text/plain", "");
}
```

ESP32 gửi về client

```json
HTTP/1.1 303 See Other
Location: /
Content-Type: text/plain
Content-Length: 0
```

Browser làm gì?

- Thấy `303`

- Đọc `Location: /`

- Tự động GET /

📌 Không cần JS

#### Nếu KHÔNG dùng sendHeader?

```cpp
server.send(303, "text/plain", "");
```

ESP32 gửi:

```json
HTTP/1.1 303 See Other
Content-Type: text/plain
```

🚨 Browser:

- Không biết đi đâu

- Không redirect

### 7️⃣ `server.onNotFound(handler)`

📌 Chức năng

- Bắt request không match route

📌 Cú pháp

```cpp
server.onNotFound(handleNotFound);
```

📌 Runtime

- Khi URI không tồn tại

- Rất hữu ích debug

### 8️⃣ `server.uri()`

📌 Chức năng

- Lấy URI client gửi

📌 Ví dụ

```cpp
String path = server.uri();
```

### 9️⃣ `server.method()`

📌 Chức năng

- Lấy HTTP method

📌 Giá trị

```cpp
HTTP_GET
HTTP_POST
HTTP_PUT
HTTP_DELETE
```

### 🔟 `server.arg(name)` / `server.arg("plain")`

📌 Chức năng

- Lấy tham số request

- Dùng cho GET query hoặc POST body

📌 Ví dụ

```cpp
server.arg("pin");      // ?pin=16
server.arg("plain");    // POST JSON
```

## 2. CÁC API LIÊN KẾT VỚI NHAU NHƯ THẾ NÀO?

Khi Browser gọi `/status`

```bash
GET /status
```

Flow:

```text
loop()
 ↓
handleClient()
 ↓
match "/status"
 ↓
handleStatus()
 ↓
server.send(200, "application/json", payload)
```

Khi bấm nút Toggle LED

```scss
fetch('/led/toggle')
```

Flow:

```text
loop()
 ↓
handleClient()
 ↓
match "/led/toggle"
 ↓
handleLedToggle()
 ↓
digitalWrite()
 ↓
server.send(303)
```

## 3. MÔ HÌNH TƯ DUY ĐÚNG (CỰC QUAN TRỌNG)

> 🧠 WebServer = Event-driven state machine

- `server.on()` = đăng ký sự kiện

- `server.handleClient()` = polling sự kiện

- `handler` = xử lý sự kiện

- GPIO = side effect

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
