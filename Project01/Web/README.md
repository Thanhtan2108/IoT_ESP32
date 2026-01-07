# IoT Monitoring Dashboard - Hướng Dẫn Sử Dụng

## Tổng Quan

Website dashboard này được thiết kế để hiển thị dữ liệu nhiệt độ và độ ẩm từ ESP32 thông qua Firebase Realtime Database. Website tự động cập nhật dữ liệu theo thời gian thực và hiển thị các biểu đồ, thống kê, và cảnh báo.

## Tính Năng

- ✅ **Hiển thị dữ liệu realtime**: Nhiệt độ và độ ẩm được cập nhật tự động từ Firebase

- ✅ **Biểu đồ thời gian thực**: Chart.js hiển thị diễn biến nhiệt độ và độ ẩm

- ✅ **Chỉ số thoải mái**: Tính toán và hiển thị chỉ số nhiệt cảm giác, điểm sương

- ✅ **Cảnh báo tự động**: Thông báo khi giá trị vượt ngưỡng cho phép

- ✅ **Lịch sử dữ liệu**: Bảng hiển thị lịch sử các lần đo

- ✅ **Xuất dữ liệu**: Export dữ liệu dạng JSON hoặc CSV

- ✅ **Giao diện responsive**: Tối ưu cho mọi thiết bị

## Cấu Trúc File

```text
Web/
├── Source/
│   ├── index.html      # File HTML chính
│   ├── styles.css      # File CSS styling
│   └── script.js       # File JavaScript xử lý Firebase và UI
├── image/              # Thư mục chứa hình ảnh (nếu có)
└── README.md           # File hướng dẫn này
```

## Cài Đặt và Sử Dụng

### 1. Cấu Hình Firebase

File `script.js` đã được cấu hình với thông tin Firebase từ firmware:

- **API Key**: `AIzaSyCVYW7FzFaTzUeTvZF7HE-n9Qi1LXLYcfY`

- **Database URL**: `https://system-monitor-temp-and-humi-default-rtdb.firebaseio.com/`

- **Authentication**: Email/Password đã được cấu hình sẵn

Nếu bạn muốn thay đổi cấu hình Firebase, chỉnh sửa phần đầu file `script.js`:

```js
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_AUTH_DOMAIN",
    databaseURL: "YOUR_DATABASE_URL",
    // ...
};
```

### 2. Chạy Website

#### Cách 1: Mở trực tiếp file HTML

- Mở file `index.html` bằng trình duyệt web

- **Lưu ý**: Một số trình duyệt có thể chặn Firebase do CORS, nên dùng cách 2

#### Cách 2: Sử dụng Local Server (Khuyến nghị)

**Với Python:**

```bash
cd Web/Source
python -m http.server 8000
```

Truy cập: `http://localhost:8000`

**Với Node.js (http-server):**

```bash
npm install -g http-server
cd Web/Source
http-server -p 8000
```

Truy cập: `http://localhost:8000`

**Với VS Code:**

- Cài extension "Live Server"

- Click chuột phải vào `index.html` → "Open with Live Server"

### 3. Cấu Trúc Dữ Liệu Firebase

Website đọc dữ liệu từ Firebase Realtime Database với cấu trúc:

```texttext
/sensorData
  ├── temperature: 25.5
  └── humidity: 65.0
```

Dữ liệu được cập nhật tự động khi ESP32 gửi lên Firebase.

## Các Tính Năng Chi Tiết

### Hiển Thị Dữ Liệu Realtime

- **Nhiệt độ**: Hiển thị giá trị hiện tại, xu hướng (tăng/giảm), min/max

- **Độ ẩm**: Tương tự như nhiệt độ

- **Chỉ số thoải mái**: Tính toán dựa trên nhiệt độ và độ ẩm

### Biểu Đồ

- **Chart.js**: Sử dụng thư viện Chart.js để vẽ biểu đồ đường

- **Tùy chọn thời gian**: 1 giờ, 6 giờ, 24 giờ, 7 ngày

- **Tự động cập nhật**: Biểu đồ tự động cập nhật khi có dữ liệu mới

### Cảnh Báo

- **Ngưỡng cảnh báo**: Có thể điều chỉnh qua các slider trong phần "Điều khiển & Cài đặt"

- **Thông báo tự động**: Hiển thị thông báo khi giá trị vượt ngưỡng

- **Trạng thái**: Màu sắc thay đổi theo trạng thái (bình thường/cảnh báo)

### Lịch Sử Dữ Liệu

- **Bảng dữ liệu**: Hiển thị các lần đo gần nhất

- **Phân trang**: Hỗ trợ phân trang và tùy chọn số dòng hiển thị

- **Xuất CSV**: Export dữ liệu ra file CSV để phân tích

## Tùy Chỉnh

### Thay Đổi Ngưỡng Cảnh Báo

Sửa trong phần "Điều khiển & Cài đặt" hoặc chỉnh sửa giá trị mặc định trong `script.js`:

```js
let alertThresholds = {
    tempMin: 15,
    tempMax: 30,
    humidityMin: 40,
    humidityMax: 75
};
```

### Thay Đổi Tần Suất Cập Nhật

Chọn trong dropdown "Tần suất cập nhật" hoặc chỉnh sửa trong `script.js`:

```javascript
let updateInterval = 5000; // 5 giây
```

### Tùy Chỉnh Giao Diện

Chỉnh sửa file `styles.css` để thay đổi màu sắc, font chữ, kích thước, v.v.

## Xử Lý Lỗi

### Lỗi Kết Nối Firebase

- Kiểm tra kết nối internet

- Kiểm tra cấu hình Firebase trong `script.js`

- Kiểm tra quyền truy cập Firebase Database Rules

### Không Hiển Thị Dữ Liệu

- Kiểm tra console trình duyệt (F12) để xem lỗi

- Đảm bảo ESP32 đang gửi dữ liệu lên Firebase

- Kiểm tra đường dẫn `/sensorData` trong Firebase Database

### Biểu Đồ Không Hiển Thị

- Kiểm tra xem Chart.js đã load chưa

- Kiểm tra console để xem lỗi JavaScript

- Đảm bảo canvas elements đã được tạo đúng

## Bảo Mật

⚠️ **Lưu ý quan trọng:**

- File `script.js` chứa thông tin đăng nhập Firebase.

- **KHÔNG** commit file này lên public repository.

- Sử dụng Firebase Rules để bảo vệ dữ liệu.

- Xem xét sử dụng Firebase Admin SDK cho production.

## Hỗ Trợ

Nếu gặp vấn đề, vui lòng kiểm tra:

1. Console trình duyệt (F12) để xem lỗi
2. Firebase Console để kiểm tra dữ liệu
3. Kết nối internet và Firebase service status

## Phiên Bản

- **Version**: 1.0.0

- **Ngày phát hành**: 2026

- **Tác giả**: Thanh Tan

## License

Dự án này được phát triển cho mục đích học tập và nghiên cứu.
