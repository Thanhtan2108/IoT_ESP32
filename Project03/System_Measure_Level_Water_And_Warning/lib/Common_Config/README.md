#

Đây là module cấu hình dùng chung cho toàn bộ các file có trong project

Vì là module cấu hình dùng chung cho toàn bộ các file project nên:

- Các biến có chứa giá trị được sử dụng trong các file code khác nên:

  - Trong file.h các biến chỉ được khai báo với từ khóa `extern`

  - Các biến này sẽ được khởi tạo giá trị trong file.cpp để khi dùng trong các file code khác tránh lỗi định nghĩa lại biến này quá nhiều lần.

Bản chất khi các biến dùng chung trong file.h được include và sử dụng lại trong file code khác thì file code đó ẽ tạo lại thêm 1 biến bản sao đó nữa, dẫn đến trong bộ nhớ sẽ tồn tại nhiều biến giống nhau => khi linker sẽ lỗi
