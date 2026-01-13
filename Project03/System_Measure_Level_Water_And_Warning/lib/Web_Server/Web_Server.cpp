#include "Web_Server.h"
#include <Preferences.h>

Preferences prefs;

int threshold_cm = 5;

// tạo đối tượng làm việc với web server qua port 80 (http)
WebServer server(80);

// tạo 1 mảng String code html css js cho web server và lưu trong Flash để tiết kiệm RAM
const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>Hệ Thống Giám Sát Mực Nước Và Cảnh Báo</title>

        <style>
            * {
                box-sizing: border-box;
            }

            body {
                margin: 0;
                padding: 0;
                font-family: Arial, Helvetica, sans-serif;
                background: #f2f4f8;
                color: #333;
                text-align: center;
            }

            h1 {
                margin: 0;
                padding: 16px 10px;
                background: #1976d2;
                color: white;
                font-size: 20px;
            }

            p {
                background: #ffffff;
                margin: 16px auto;
                padding: 14px;
                width: 90%;
                max-width: 360px;
                border-radius: 10px;
                box-shadow: 0 4px 10px rgba(0,0,0,0.08);
                font-size: 16px;
            }

            p b {
                display: block;
                font-size: 14px;
                color: #666;
                margin-bottom: 6px;
            }

            #water,
            #th_value {
                font-size: 32px;
                font-weight: bold;
                color: #333;
            }

            input[type=range] {
                width: 90%;
                max-width: 360px;
                margin: 24px auto;
                display: block;
                -webkit-appearance: none;
                height: 6px;
                background: #ddd;
                border-radius: 5px;
                outline: none;
            }

            input[type=range]::-webkit-slider-thumb {
                -webkit-appearance: none;
                width: 22px;
                height: 22px;
                border-radius: 50%;
                background: #1976d2;
                cursor: pointer;
                box-shadow: 0 2px 6px rgba(0,0,0,0.3);
            }

            input[type=range]::-moz-range-thumb {
                width: 22px;
                height: 22px;
                border-radius: 50%;
                background: #1976d2;
                cursor: pointer;
            }
        </style>
    </head>

    <body>
        <h1>Hệ Thống Giám Sát Mực Nước</h1>

        <p>
            <b>Mực nước hiện tại:</b>
            <span id="water">--</span> cm
        </p>

        <p>
            <b>Ngưỡng cảnh báo:</b>
            <span id="th_value">--</span> cm
        </p>

        <input type="range"
            min="0"
            max="10"
            value="5"
            id="slider"
            oninput="setThreshold(this.value)">
       
        <script>
            async function refresh() {
                try {
                    const r = await fetch('/status');
                    const j = await r.json();
                    document.getElementById('water').textContent = j.water;
                    document.getElementById('th_value').textContent = j.threshold;
                    document.getElementById('slider').value = j.threshold;
                } catch(e) {
                    console.error(e);
                }
            }

            function setThreshold(v) {
                document.getElementById('th_value').textContent = v;
                fetch('/set_threshold?value=' + v);
            }

            setInterval(refresh, 1000);
            refresh();
        </script>
    </body>
</html>
)rawliteral";

// Lấy dữ liệu web trong Flash ra RAM để chạy web server
String generateMainPage() {
    String s = String(FPSTR(MAIN_PAGE));
    return s;
}

// handler xử lý cho root (<ip>/)
void handleRoot() {
    server.send(200, "text/html", generateMainPage());
}

// handler xử lý cho route (<ip>/status)
void handleStatus() {
    String json = "{";
    json += "\"water\":" + String(distanceCM, 1) + ",";
    json += "\"threshold\":" + String(threshold_cm);
    json += "}";
    server.send(200, "application/json", json);
}

// handler xử lý cho route thiết lập ngưỡng
void handleSetThreshold() {
    if (server.hasArg("value")) {
        int v = server.arg("value").toInt();
        if (v >= 0 && v <= 10) {
            threshold_cm = v;
            prefs.putInt("threshold", threshold_cm); // 🔥 LƯU FLASH
        }
    }
    server.send(200, "text/plain", "OK");
}

// handler xử lý cho route không phù hợp
void handleNotFound() {
    String msg = "Not Found\n\n";
    msg += "URI: ";
    msg += server.uri();
    msg += "\nMethod: ";
    msg += (server.method() == HTTP_GET) ? "GET" : "OTHER";
    server.send(404, "text/plain", msg);
}

// Đăng ký route lên web, gắn handler với route tương ứng bằng phương thức http thích hợp
void register_for_route_web_server() {
    prefs.begin("water_cfg", false);

    // load ngưỡng đã lưu, nếu chưa có thì lấy 5
    threshold_cm = prefs.getInt("threshold", 5);

    server.on("/", HTTP_GET, handleRoot);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/set_threshold", HTTP_GET, handleSetThreshold);
    server.onNotFound([](){ server.send(404, "text/plain", "Not Found"); });

    server.begin();
    Serial.println("HTTP server started");
}

// web chạy và luôn on stream để luôn lắng nghe http
void run_web_server() {
    server.handleClient();
}
