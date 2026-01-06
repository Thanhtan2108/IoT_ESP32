#

Đây là module phát triển cho cảm biến nhiệt độ độ ẩm DHT11 để đo nhiệt độ độ ẩm của môi trường

Khởi tạo 1 đối tượng cho cảm biến từ class DHT : `DHT dht_sensor(DHT_PIN, DHT_TYPE);`

Các API chính:

- Khởi tạo cảm biến:

  - Hàm `dht_init`

  - Khởi tạo kết nối cảm biến `dht_sensor.begin();`

- Đọc nhiệt độ từ cảm biến:

  - Hàm `dht_read_temperature`

  - Dùng API có sẵn của thư viện DHT để đọc nhiệt độ : `dht_sensor.readTemperature();`

  - Kiểm tra giá trị nhiệt độ có thỏa không : `if (isnan(temperature))`

- Đọc độ ẩm từ cảm biến:

  - Hàm `dht_read_humidity`

  - Dùng API có sẵn của thư viện DHT để đọc độ ẩm : `dht_sensor.readHumidity();`

  - Kiểm tra giá trị độ ẩm có thỏa không : `if (isnan(temperature))`

- Kiểm tra xem giá trị nhiệt độ độ ẩm đọc được có hợp lệ hay không

  - Hàm `dht_is_valid_read`

  - Dùng API của thư viện DHT : `isnan` kèm các điều kiện để kiểm tra
