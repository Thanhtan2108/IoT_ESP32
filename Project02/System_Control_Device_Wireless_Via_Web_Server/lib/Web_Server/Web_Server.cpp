#include "Web_Server.h"

// initial webserver with port 80
WebServer server(80);

// Variable global
bool ledState = false;
bool fanState = true;
bool pumpState = false;

// code html on esp32
// Note: using `PROGMEM` to save data data type const in memory Flash instead of memory RAM
/*
- R"..." là cú pháp raw string literal: cho phép viết chuỗi mà không cần escape ký tự đặc biệt (\n, \", \\, …).
- Phần trong ngoặc "rawliteral" gọi là delimiter. Nó giúp compiler biết đâu là điểm bắt đầu và kết thúc của chuỗi, tránh nhầm lẫn nếu trong nội dung có dấu )".
- Nội dung giữa ( và ) sẽ được giữ nguyên y như bạn viết, kể cả xuống dòng, dấu " hay ký tự đặc biệt.
*/
// HTML CSS JS
const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>ESP32 Dashboard</title>

        <style>
            body {
                margin: 0;
                font-family: Arial, Helvetica, sans-serif;
                background: #f2f4f8;
                color: #333;
            }

            h1 {
                text-align: center;
                padding: 16px;
                margin: 0;
                background: #2b6cb0;
                color: white;
            }

            .container {
                max-width: 480px;
                margin: 20px auto;
                padding: 10px;
            }

            .card {
                background: white;
                border-radius: 10px;
                padding: 16px;
                margin-bottom: 16px;
                box-shadow: 0 4px 10px rgba(0,0,0,0.08);
                display: flex;
                align-items: center;
                justify-content: space-between;
            }

            .device-info {
                display: flex;
                flex-direction: column;
            }

            .device-name {
                font-size: 18px;
                font-weight: bold;
                margin-bottom: 4px;
            }

            .status {
                font-size: 14px;
                font-weight: bold;
            }

            .status.on {
                color: #38a169;
            }

            .status.off {
                color: #e53e3e;
            }

            button {
                border: none;
                padding: 10px 16px;
                border-radius: 6px;
                font-size: 14px;
                cursor: pointer;
                color: white;
            }

            button.toggle {
                background: #3182ce;
            }

            button.toggle:hover {
                background: #2b6cb0;
            }

            @media (max-width: 480px) {
                .card {
                    flex-direction: column;
                    align-items: flex-start;
                    gap: 10px;
                }

                button {
                    width: 100%;
                }
            }
        </style>
    </head>
    <body>
        <h1>ESP32 Dashboard</h1>

        <div class="container">

            <div class="card">
                <div class="device-info">
                    <div class="device-name">LED</div>
                    <div id="led" class="status">%LED%</div>
                </div>
                <button class="toggle" onclick="fetch('/led/toggle')">Toggle</button>
            </div>

            <div class="card">
                <div class="device-info">
                    <div class="device-name">FAN</div>
                    <div id="fan" class="status">%FAN%</div>
                </div>
                <button class="toggle" onclick="fetch('/fan/toggle')">Toggle</button>
            </div>

            <div class="card">
                <div class="device-info">
                    <div class="device-name">PUMP</div>
                    <div id="pump" class="status">%PUMP%</div>
                </div>
                <button class="toggle" onclick="fetch('/pump/toggle')">Toggle</button>
            </div>

        </div>
        
        <script>
            async function refresh() {
                try {
                    const r = await fetch('/status');
                    const j = await r.json();
                    document.getElementById('led').textContent = j.led ? 'ON' : 'OFF';
                    document.getElementById('fan').textContent = j.fan ? 'OFF' : 'ON';
                    document.getElementById('pump').textContent = j.pump ? 'ON' : 'OFF';
                } catch(e){ 
                    console.error(e);
                }
            }
            setInterval(refresh, 2000);
            refresh();
        </script>
    </body>
</html>
)rawliteral";

// Tạo trang chính từ template PROGMEM, thay %STATE% bằng ON/OFF
String generateMainPage() {
    String s = String(FPSTR(MAIN_PAGE));
    s.replace("%LED%", ledState ? "ON" : "OFF");
    s.replace("%FAN%", fanState ? "OFF" : "ON");
    s.replace("%PUMP%", pumpState ? "ON" : "OFF");
    return s;
}

// Route handlers
void handleRoot() {
    server.send(200, "text/html", generateMainPage());
}

void setPinAndState(int pin, bool on){
    digitalWrite(pin, on ? HIGH : LOW);
}

void handleLedToggle() {
    ledState = !ledState;
    setPinAndState(LED_PIN, ledState);
    server.send(303, "text/plain", ""); // redirect back
}

void handleFanToggle() {
    fanState = !fanState;
    setPinAndState(RELAY_FAN_PIN, fanState);
    server.send(303, "text/plain", "");
}

void handlePumpToggle() {
    pumpState = !pumpState;
    setPinAndState(RELAY_PUMP_PIN, pumpState);
    server.send(303, "text/plain", "");
}

// /status trả JSON nhiều trường
void handleStatus() {
    String payload = "{";
    payload += "\"led\":"; payload += ledState ? "true" : "false"; payload += ",";
    payload += "\"fan\":"; payload += fanState ? "true" : "false"; payload += ",";
    payload += "\"pump\":"; payload += pumpState ? "true" : "false"; payload += ",";
    payload += "\"ip\":\""; payload += WiFi.localIP().toString(); payload += "\"";
    payload += "}";
    server.send(200, "application/json", payload);
}

void handleNotFound() {
    String msg = "Not Found\n\n";
    msg += "URI: ";
    msg += server.uri();
    msg += "\nMethod: ";
    msg += (server.method() == HTTP_GET) ? "GET" : "OTHER";
    server.send(404, "text/plain", msg);
}

void register_for_route_web_server() {
    // Đăng ký route (để gọi callback tương ứng)
    server.on("/", HTTP_GET, handleRoot);
    server.on("/led/toggle", HTTP_GET, handleLedToggle);
    server.on("/fan/toggle", HTTP_GET, handleFanToggle);
    server.on("/pump/toggle", HTTP_GET, handlePumpToggle);
    server.on("/status", HTTP_GET, handleStatus);
    server.onNotFound([](){ server.send(404, "text/plain", "Not Found"); });
    server.begin();
    Serial.println("HTTP server started");
}

void run_web_server() {
    server.handleClient();
}
