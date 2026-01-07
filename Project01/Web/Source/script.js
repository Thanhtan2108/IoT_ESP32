// ============================================
// Firebase Configuration
// ============================================
const firebaseConfig = {
  apiKey: "AIzaSyCVYW7FzFaTzUeTvZF7HE-n9Qi1LXLYcfY",
  authDomain: "system-monitor-temp-and-humi.firebaseapp.com",
  databaseURL:
    "https://system-monitor-temp-and-humi-default-rtdb.firebaseio.com",
  projectId: "system-monitor-temp-and-humi",
  storageBucket: "system-monitor-temp-and-humi.firebasestorage.app",
  messagingSenderId: "751593601722",
  appId: "1:751593601722:web:e0d0ef36a01162f378e083",
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();

// ============================================
// Global Variables
// ============================================
let temperatureData = [];
let humidityData = [];
let temperatureChart = null;
let humidityChart = null;
let lastTemperature = null;
let lastHumidity = null;
let maxDataPoints = 50; // Số điểm dữ liệu tối đa trên biểu đồ

// Alert thresholds
let alertThresholds = {
  tempMin: 15,
  tempMax: 30,
  humidityMin: 40,
  humidityMax: 75,
};

// ============================================
// Firebase Authentication
// ============================================
async function authenticateFirebase() {
  try {
    const email = "systemmonitortempandhumi@gmail.com";
    const password = "Project01";

    await firebase.auth().signInWithEmailAndPassword(email, password);
    console.log("Firebase authentication successful");
    // Không cập nhật trạng thái ở đây - chỉ cập nhật khi có dữ liệu thực sự
    return true;
  } catch (error) {
    console.error("Firebase authentication error:", error);
    updateConnectionStatus(false);
    console.error("❌ Lỗi xác thực Firebase:", error.message);
    return false;
  }
}

// ============================================
// Initialize Charts
// ============================================
function initializeCharts() {
  const tempCanvas = document.querySelectorAll(".chart-placeholder canvas")[0];
  const humiCanvas = document.querySelectorAll(".chart-placeholder canvas")[1];

  if (!tempCanvas || !humiCanvas) {
    console.error("Canvas elements not found");
    return;
  }

  const tempCtx = tempCanvas.getContext("2d");
  const humiCtx = humiCanvas.getContext("2d");

  // Temperature Chart
  temperatureChart = new Chart(tempCtx, {
    type: "line",
    data: {
      labels: [],
      datasets: [
        {
          label: "Nhiệt độ (°C)",
          data: [],
          borderColor: "#ef4444",
          backgroundColor: "rgba(239, 68, 68, 0.1)",
          borderWidth: 2,
          fill: true,
          tension: 0.4,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: {
          display: true,
          position: "top",
        },
        tooltip: {
          mode: "index",
          intersect: false,
        },
      },
      scales: {
        y: {
          beginAtZero: false,
          title: {
            display: true,
            text: "Nhiệt độ (°C)",
          },
        },
        x: {
          title: {
            display: true,
            text: "Thời gian",
          },
        },
      },
    },
  });

  // Humidity Chart
  humidityChart = new Chart(humiCtx, {
    type: "line",
    data: {
      labels: [],
      datasets: [
        {
          label: "Độ ẩm (%)",
          data: [],
          borderColor: "#3b82f6",
          backgroundColor: "rgba(59, 130, 246, 0.1)",
          borderWidth: 2,
          fill: true,
          tension: 0.4,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: {
          display: true,
          position: "top",
        },
        tooltip: {
          mode: "index",
          intersect: false,
        },
      },
      scales: {
        y: {
          beginAtZero: true,
          max: 100,
          title: {
            display: true,
            text: "Độ ẩm (%)",
          },
        },
        x: {
          title: {
            display: true,
            text: "Thời gian",
          },
        },
      },
    },
  });
}

// ============================================
// Firebase Realtime Database Listeners
// ============================================
function setupFirebaseListeners() {
  const sensorDataRef = database.ref("/sensorData");
  let hasReceivedData = false;

  // Listen for temperature changes
  sensorDataRef.child("temperature").on(
    "value",
    (snapshot) => {
      const temperature = snapshot.val();
      // Kiểm tra chặt chẽ: chỉ chấp nhận số hợp lệ
      if (
        temperature !== null &&
        typeof temperature === "number" &&
        !isNaN(temperature) &&
        isFinite(temperature)
      ) {
        // Chỉ cập nhật trạng thái "Đã kết nối" khi thực sự nhận được dữ liệu lần đầu
        if (!hasReceivedData) {
          hasReceivedData = true;
          updateConnectionStatus(true);
          console.log(
            "✅ Đã nhận dữ liệu từ thiết bị - Trạng thái: Đã kết nối"
          );
        }
        updateTemperature(temperature);
        addToHistory("temperature", temperature);
      } else {
        // Nếu không có dữ liệu hoặc dữ liệu không hợp lệ, đặt lại trạng thái
        if (hasReceivedData) {
          hasReceivedData = false;
          updateConnectionStatus(false);
          resetAllValues();
        } else {
          // Nếu chưa từng nhận được dữ liệu, đảm bảo hiển thị "--"
          resetAllValues();
        }
      }
    },
    (error) => {
      console.error("Temperature listener error:", error);
      console.error("❌ Lỗi đọc dữ liệu: Không thể đọc nhiệt độ từ Firebase");
      updateConnectionStatus(false);
      resetAllValues();
    }
  );

  // Listen for humidity changes
  sensorDataRef.child("humidity").on(
    "value",
    (snapshot) => {
      const humidity = snapshot.val();
      // Kiểm tra chặt chẽ: chỉ chấp nhận số hợp lệ
      if (
        humidity !== null &&
        typeof humidity === "number" &&
        !isNaN(humidity) &&
        isFinite(humidity)
      ) {
        // Chỉ cập nhật trạng thái "Đã kết nối" khi thực sự nhận được dữ liệu lần đầu
        if (!hasReceivedData) {
          hasReceivedData = true;
          updateConnectionStatus(true);
          console.log(
            "✅ Đã nhận dữ liệu từ thiết bị - Trạng thái: Đã kết nối"
          );
        }
        updateHumidity(humidity);
        addToHistory("humidity", humidity);
      } else {
        // Nếu không có dữ liệu hoặc dữ liệu không hợp lệ, đặt lại trạng thái
        if (hasReceivedData) {
          hasReceivedData = false;
          updateConnectionStatus(false);
          resetAllValues();
        } else {
          // Nếu chưa từng nhận được dữ liệu, đảm bảo hiển thị "--"
          resetAllValues();
        }
      }
    },
    (error) => {
      console.error("Humidity listener error:", error);
      console.error("❌ Lỗi đọc dữ liệu: Không thể đọc độ ẩm từ Firebase");
      updateConnectionStatus(false);
      resetAllValues();
    }
  );

  // Listen for connection status
  database.ref(".info/connected").on("value", (snapshot) => {
    const connected = snapshot.val();
    // Chỉ cập nhật trạng thái offline nếu mất kết nối Firebase
    // Không cập nhật online ở đây vì phải đợi có dữ liệu thực sự
    if (!connected) {
      updateConnectionStatus(false);
      resetAllValues();
      console.warn("⚠️ Mất kết nối Firebase: Đang cố gắng kết nối lại...");
    }
  });
}

// ============================================
// Update UI Functions
// ============================================
function updateTemperature(value) {
  const tempCard = document.querySelector(".reading-card.temperature");
  if (!tempCard) return;

  const valueElement = tempCard.querySelector(".main-value .value");
  const trendElement = tempCard.querySelector(".card-trend");

  // Update main value
  if (valueElement) valueElement.textContent = value.toFixed(1);

  // Update trend
  if (lastTemperature !== null && trendElement) {
    const diff = value - lastTemperature;
    const trendIndicator = trendElement.querySelector(".trend-indicator");
    const trendText = trendElement.querySelector(".trend-text");

    if (trendIndicator && trendText) {
      if (Math.abs(diff) > 0.1) {
        if (diff > 0) {
          trendIndicator.className = "trend-indicator up";
          trendText.textContent = `Tăng ${Math.abs(diff).toFixed(1)}°C`;
        } else {
          trendIndicator.className = "trend-indicator down";
          trendText.textContent = `Giảm ${Math.abs(diff).toFixed(1)}°C`;
        }
      } else {
        trendText.textContent = "Ổn định";
      }
    }
  }

  // Update min/max
  updateMinMax("temperature", value);

  // Update status
  updateTemperatureStatus(value);

  // Update chart
  addToChart("temperature", value);

  lastTemperature = value;
  updateLastUpdateTime();
}

function updateHumidity(value) {
  const humiCard = document.querySelector(".reading-card.humidity");
  if (!humiCard) return;

  const valueElement = humiCard.querySelector(".main-value .value");
  const trendElement = humiCard.querySelector(".card-trend");

  // Update main value
  if (valueElement) valueElement.textContent = Math.round(value);

  // Update trend
  if (lastHumidity !== null && trendElement) {
    const diff = value - lastHumidity;
    const trendIndicator = trendElement.querySelector(".trend-indicator");
    const trendText = trendElement.querySelector(".trend-text");

    if (trendIndicator && trendText) {
      if (Math.abs(diff) > 0.1) {
        if (diff > 0) {
          trendIndicator.className = "trend-indicator up";
          trendText.textContent = `Tăng ${Math.abs(diff).toFixed(1)}%`;
        } else {
          trendIndicator.className = "trend-indicator down";
          trendText.textContent = `Giảm ${Math.abs(diff).toFixed(1)}%`;
        }
      } else {
        trendText.textContent = "Ổn định";
      }
    }
  }

  // Update min/max
  updateMinMax("humidity", value);

  // Update status
  updateHumidityStatus(value);

  // Update chart
  addToChart("humidity", value);

  lastHumidity = value;
  updateLastUpdateTime();
}

function updateMinMax(type, value) {
  const card = document.querySelector(`.reading-card.${type}`);
  if (!card) return;

  const maxElement = card.querySelector(".info-item:first-child .info-value");
  const minElement = card.querySelector(".info-item:last-child .info-value");

  if (!maxElement || !minElement) return;

  // Get current min/max from data array
  const dataArray = type === "temperature" ? temperatureData : humidityData;
  if (dataArray.length > 0) {
    const values = dataArray.map((d) => d.value);
    const max = Math.max(...values, value);
    const min = Math.min(...values, value);

    if (type === "temperature") {
      maxElement.textContent = `${max.toFixed(1)}°C`;
      minElement.textContent = `${min.toFixed(1)}°C`;
    } else {
      maxElement.textContent = `${Math.round(max)}%`;
      minElement.textContent = `${Math.round(min)}%`;
    }
  } else {
    // Nếu chưa có dữ liệu, hiển thị "--"
    maxElement.textContent = "--";
    minElement.textContent = "--";
  }
}

function updateTemperatureStatus(value) {
  const card = document.querySelector(".reading-card.temperature");
  if (!card) return;

  const statusElement = card.querySelector(
    ".status-indicator.normal .status-text"
  );
  const statusDot = card.querySelector(".status-indicator.normal .status-dot");

  if (!statusElement || !statusDot) return;

  if (value < alertThresholds.tempMin || value > alertThresholds.tempMax) {
    statusElement.textContent = "Cảnh báo";
    statusDot.style.backgroundColor = "#ef4444";
    console.warn(
      `⚠️ Cảnh báo nhiệt độ: ${value.toFixed(1)}°C nằm ngoài ngưỡng cho phép!`
    );
  } else {
    statusElement.textContent = "Bình thường";
    statusDot.style.backgroundColor = "#3b82f6";
  }
}

function updateHumidityStatus(value) {
  const card = document.querySelector(".reading-card.humidity");
  if (!card) return;

  const statusElement = card.querySelector(
    ".status-indicator.normal .status-text"
  );
  const statusDot = card.querySelector(".status-indicator.normal .status-dot");

  if (!statusElement || !statusDot) return;

  if (
    value < alertThresholds.humidityMin ||
    value > alertThresholds.humidityMax
  ) {
    statusElement.textContent = "Cảnh báo";
    statusDot.style.backgroundColor = "#ef4444";
    console.warn(
      `⚠️ Cảnh báo độ ẩm: ${Math.round(value)}% nằm ngoài ngưỡng cho phép!`
    );
  } else {
    statusElement.textContent = "Bình thường";
    statusDot.style.backgroundColor = "#3b82f6";
  }
}

function updateComfortIndex() {
  // Chỉ cập nhật khi có đủ cả nhiệt độ và độ ẩm
  if (lastTemperature === null || lastHumidity === null) {
    // Reset về "--" nếu thiếu dữ liệu
    const comfortCard = document.querySelector(".reading-card.comfort");
    if (comfortCard) {
      const comfortLevelElement = comfortCard.querySelector(".comfort-level");
      const comfortScoreElement = comfortCard.querySelector(".comfort-score");
      const heatIndexElement = comfortCard.querySelector(
        ".detail-item:first-child .detail-value"
      );
      const dewPointElement = comfortCard.querySelector(
        ".detail-item:last-child .detail-value"
      );
      const recommendation = comfortCard.querySelector(
        ".comfort-recommendation"
      );

      if (comfortLevelElement) comfortLevelElement.textContent = "--";
      if (comfortScoreElement) comfortScoreElement.textContent = "--";
      if (heatIndexElement) heatIndexElement.textContent = "--";
      if (dewPointElement) dewPointElement.textContent = "--";
      if (recommendation)
        recommendation.textContent = "Chờ dữ liệu từ thiết bị";
    }
    return;
  }

  const comfortCard = document.querySelector(".reading-card.comfort");
  if (!comfortCard) return;

  // Calculate heat index (feels like temperature)
  const heatIndex = calculateHeatIndex(lastTemperature, lastHumidity);

  // Calculate comfort level
  const comfortScore = calculateComfortScore(lastTemperature, lastHumidity);
  const comfortLevel = getComfortLevel(comfortScore);

  const comfortLevelElement = comfortCard.querySelector(".comfort-level");
  const comfortScoreElement = comfortCard.querySelector(".comfort-score");
  const heatIndexElement = comfortCard.querySelector(
    ".detail-item:first-child .detail-value"
  );
  const dewPointElement = comfortCard.querySelector(
    ".detail-item:last-child .detail-value"
  );

  if (comfortLevelElement) comfortLevelElement.textContent = comfortLevel;
  if (comfortScoreElement)
    comfortScoreElement.textContent = `${comfortScore.toFixed(1)}/10`;
  if (heatIndexElement)
    heatIndexElement.textContent = `${heatIndex.toFixed(1)}°C`;

  // Calculate dew point
  const dewPoint = calculateDewPoint(lastTemperature, lastHumidity);
  if (dewPointElement) dewPointElement.textContent = `${dewPoint.toFixed(1)}°C`;

  // Update recommendation
  const recommendation = comfortCard.querySelector(".comfort-recommendation");
  if (recommendation)
    recommendation.textContent = getComfortRecommendation(comfortScore);
}

function calculateHeatIndex(temp, humidity) {
  // Heat index calculation (simplified, works for temp > 27°C and humidity > 40%)
  if (temp < 27 || humidity < 40) {
    return temp; // Return temperature if conditions not met
  }

  const T = temp;
  const R = humidity;
  const HI =
    -8.78469475556 +
    1.61139411 * T +
    2.33854883889 * R -
    0.14611605 * T * R -
    0.012308094 * T * T -
    0.0164248277778 * R * R +
    0.002211732 * T * T * R +
    0.00072546 * T * R * R -
    0.000003582 * T * T * R * R;
  return Math.max(temp, HI); // Return at least the temperature
}

function calculateDewPoint(temp, humidity) {
  const a = 17.27;
  const b = 237.7;
  const alpha = (a * temp) / (b + temp) + Math.log(humidity / 100.0);
  return (b * alpha) / (a - alpha);
}

function calculateComfortScore(temp, humidity) {
  // Comfort score based on temperature and humidity
  let score = 10;

  // Temperature scoring (ideal: 20-26°C)
  if (temp < 18 || temp > 28) score -= 3;
  else if (temp < 20 || temp > 26) score -= 1.5;

  // Humidity scoring (ideal: 40-60%)
  if (humidity < 30 || humidity > 70) score -= 2;
  else if (humidity < 40 || humidity > 60) score -= 1;

  return Math.max(0, Math.min(10, score));
}

function getComfortLevel(score) {
  if (score >= 8) return "Rất thoải mái";
  if (score >= 6) return "Thoải mái";
  if (score >= 4) return "Chấp nhận được";
  return "Không thoải mái";
}

function getComfortRecommendation(score) {
  if (score >= 8) return "Điều kiện lý tưởng cho sinh hoạt";
  if (score >= 6) return "Điều kiện tốt";
  if (score >= 4) return "Có thể cần điều chỉnh";
  return "Cần điều chỉnh nhiệt độ hoặc độ ẩm";
}

function updateConnectionStatus(isConnected) {
  const statusIndicator = document.querySelector(
    ".status-section .status-indicator"
  );
  const statusValue = document.querySelector(".status-section .status-value");

  if (isConnected) {
    statusIndicator.className = "status-indicator online";
    statusValue.textContent = "Đã kết nối";
  } else {
    statusIndicator.className = "status-indicator offline";
    statusValue.textContent = "Chưa kết nối";
  }
}

// Reset tất cả giá trị về "--" khi mất kết nối
function resetAllValues() {
  // Reset nhiệt độ
  const tempCard = document.querySelector(".reading-card.temperature");
  if (tempCard) {
    const valueElement = tempCard.querySelector(".main-value .value");
    const maxElement = tempCard.querySelector(
      ".info-item:first-child .info-value"
    );
    const minElement = tempCard.querySelector(
      ".info-item:last-child .info-value"
    );
    const trendText = tempCard.querySelector(".card-trend .trend-text");
    const statusElement = tempCard.querySelector(
      ".status-indicator.normal .status-text"
    );

    if (valueElement) valueElement.textContent = "--";
    if (maxElement) maxElement.textContent = "--";
    if (minElement) minElement.textContent = "--";
    if (trendText) trendText.textContent = "--";
    if (statusElement) statusElement.textContent = "--";
  }

  // Reset độ ẩm
  const humiCard = document.querySelector(".reading-card.humidity");
  if (humiCard) {
    const valueElement = humiCard.querySelector(".main-value .value");
    const maxElement = humiCard.querySelector(
      ".info-item:first-child .info-value"
    );
    const minElement = humiCard.querySelector(
      ".info-item:last-child .info-value"
    );
    const trendText = humiCard.querySelector(".card-trend .trend-text");
    const statusElement = humiCard.querySelector(
      ".status-indicator.normal .status-text"
    );

    if (valueElement) valueElement.textContent = "--";
    if (maxElement) maxElement.textContent = "--";
    if (minElement) minElement.textContent = "--";
    if (trendText) trendText.textContent = "--";
    if (statusElement) statusElement.textContent = "--";
  }

  // Reset chỉ số thoải mái
  const comfortCard = document.querySelector(".reading-card.comfort");
  if (comfortCard) {
    const comfortLevel = comfortCard.querySelector(".comfort-level");
    const comfortScore = comfortCard.querySelector(".comfort-score");
    const heatIndex = comfortCard.querySelector(
      ".detail-item:first-child .detail-value"
    );
    const dewPoint = comfortCard.querySelector(
      ".detail-item:last-child .detail-value"
    );
    const recommendation = comfortCard.querySelector(".comfort-recommendation");

    if (comfortLevel) comfortLevel.textContent = "--";
    if (comfortScore) comfortScore.textContent = "--";
    if (heatIndex) heatIndex.textContent = "--";
    if (dewPoint) dewPoint.textContent = "--";
    if (recommendation) recommendation.textContent = "Chờ dữ liệu từ thiết bị";
  }

  // Reset thời gian
  const dataTime = document.querySelector(".data-time");
  const updateTime = document.querySelector(".update-time");
  const readTimes = document.querySelectorAll(".read-time");

  if (dataTime) dataTime.textContent = "--";
  if (updateTime) updateTime.textContent = "--";
  readTimes.forEach((el) => (el.textContent = "--"));

  // Reset chart stats
  const statItems = document.querySelectorAll(".stat-item");
  statItems.forEach((item) => {
    const strong = item.querySelector("strong");
    if (strong) strong.textContent = "--";
  });

  // Reset biến
  lastTemperature = null;
  lastHumidity = null;
  temperatureData = [];
  humidityData = [];
}

function updateLastUpdateTime() {
  // Cập nhật thời gian đo trong cards khi có dữ liệu mới
  const now = new Date();
  const timeString = now.toLocaleTimeString("vi-VN");
  const dateString = now.toLocaleDateString("vi-VN") + " " + timeString;

  const dataTimeEl = document.querySelector(".data-time");
  if (dataTimeEl) dataTimeEl.textContent = dateString;

  // Update read time in cards
  document.querySelectorAll(".read-time").forEach((el) => {
    el.textContent = timeString;
  });
}

// Cập nhật thời gian realtime theo giờ laptop
function startRealtimeClock() {
  // Cập nhật ngay lập tức
  updateRealtimeClock();

  // Cập nhật mỗi giây
  setInterval(() => {
    updateRealtimeClock();
  }, 1000);
}

function updateRealtimeClock() {
  const now = new Date();
  const timeString = now.toLocaleTimeString("vi-VN");
  const dateString = now.toLocaleDateString("vi-VN") + " " + timeString;

  const updateTimeEl = document.querySelector(".update-time");
  if (updateTimeEl) updateTimeEl.textContent = timeString;

  // Chỉ cập nhật data-time khi có dữ liệu
  if (lastTemperature !== null || lastHumidity !== null) {
    const dataTimeEl = document.querySelector(".data-time");
    if (dataTimeEl) dataTimeEl.textContent = dateString;
  }
}

// ============================================
// Chart Functions
// ============================================
function addToChart(type, value) {
  const chart = type === "temperature" ? temperatureChart : humidityChart;
  if (!chart) return;

  const dataArray = type === "temperature" ? temperatureData : humidityData;

  const now = new Date();
  const timeLabel = now.toLocaleTimeString("vi-VN", {
    hour: "2-digit",
    minute: "2-digit",
  });

  // Add new data point
  dataArray.push({
    time: now,
    value: value,
    label: timeLabel,
  });

  // Limit data points
  if (dataArray.length > maxDataPoints) {
    dataArray.shift();
  }

  // Update chart
  chart.data.labels = dataArray.map((d) => d.label);
  chart.data.datasets[0].data = dataArray.map((d) => d.value);

  // Update chart stats
  updateChartStats(type, dataArray);

  chart.update("none"); // 'none' for smooth animation
}

function updateChartStats(type, dataArray) {
  if (dataArray.length === 0) return;

  const values = dataArray.map((d) => d.value);
  const avg = values.reduce((a, b) => a + b, 0) / values.length;
  const min = Math.min(...values);
  const max = Math.max(...values);
  const range = max - min;

  const chartWrappers = document.querySelectorAll(".chart-wrapper");
  if (chartWrappers.length < 2) return;

  const chartWrapper =
    type === "temperature" ? chartWrappers[0] : chartWrappers[1];

  const stats = chartWrapper.querySelector(".chart-stats");
  if (!stats) return;

  const statItems = stats.querySelectorAll(".stat-item");
  if (statItems.length < 2) return;

  if (type === "temperature") {
    statItems[0].innerHTML = `Trung bình: <strong>${avg.toFixed(1)}°C</strong>`;
    statItems[1].innerHTML = `Dao động: <strong>${range.toFixed(1)}°C</strong>`;
  } else {
    statItems[0].innerHTML = `Trung bình: <strong>${Math.round(avg)}%</strong>`;
    statItems[1].innerHTML = `Dao động: <strong>${Math.round(range)}%</strong>`;
  }
}

// ============================================
// History Functions
// ============================================
function addToHistory(type, value) {
  // Update comfort index when both values are available
  if (lastTemperature !== null && lastHumidity !== null) {
    updateComfortIndex();
  }
}

// ============================================
// Event Listeners
// ============================================
document.addEventListener("DOMContentLoaded", async () => {
  // Khởi tạo trạng thái là "Chưa kết nối"
  updateConnectionStatus(false);

  // Replace chart placeholders with canvas elements
  replaceChartPlaceholders();

  // Wait a bit for DOM to update
  setTimeout(() => {
    // Initialize charts
    initializeCharts();

    // Authenticate Firebase
    authenticateFirebase().then((authSuccess) => {
      if (authSuccess) {
        setupFirebaseListeners();
        console.log(
          "✅ Đã xác thực Firebase - Đang chờ dữ liệu từ thiết bị..."
        );
      } else {
        updateConnectionStatus(false);
        resetAllValues();
      }
    });

    // Setup UI event listeners
    setupEventListeners();

    // Bắt đầu cập nhật thời gian realtime
    startRealtimeClock();
  }, 100);
});

function replaceChartPlaceholders() {
  const placeholders = document.querySelectorAll(".chart-placeholder");
  placeholders.forEach((placeholder) => {
    const canvas = document.createElement("canvas");
    canvas.style.width = "100%";
    canvas.style.height = "300px";
    placeholder.innerHTML = "";
    placeholder.appendChild(canvas);
    placeholder.style.height = "300px";
    placeholder.style.padding = "0";
    placeholder.style.background = "transparent";
    placeholder.style.border = "none";
  });
}

function setupEventListeners() {
  // Refresh button
  const refreshBtn = document.querySelector(".refresh-button");
  if (refreshBtn) {
    refreshBtn.addEventListener("click", () => {
      location.reload();
    });
  }

  // Dark mode toggle
  const darkModeSwitch = document.getElementById("dark-mode-switch");
  const darkModeIcon = document.getElementById("dark-mode-icon");
  if (darkModeSwitch) {
    // Load saved preference
    const savedDarkMode = localStorage.getItem("darkMode") === "true";
    if (savedDarkMode) {
      document.body.classList.add("dark-mode");
      darkModeSwitch.checked = true;
      if (darkModeIcon) {
        darkModeIcon.className = "fas fa-moon";
      }
    } else {
      if (darkModeIcon) {
        darkModeIcon.className = "fas fa-sun";
      }
    }

    darkModeSwitch.addEventListener("change", (e) => {
      if (e.target.checked) {
        document.body.classList.add("dark-mode");
        localStorage.setItem("darkMode", "true");
        if (darkModeIcon) {
          darkModeIcon.className = "fas fa-moon";
        }
      } else {
        document.body.classList.remove("dark-mode");
        localStorage.setItem("darkMode", "false");
        if (darkModeIcon) {
          darkModeIcon.className = "fas fa-sun";
        }
      }
    });
  }

  // Chart time range selector
  const timeRangeSelector = document.querySelector(".time-range-selector");
  if (timeRangeSelector) {
    timeRangeSelector.addEventListener("change", (e) => {
      const range = e.target.value;
      switch (range) {
        case "1h":
          maxDataPoints = 60;
          break;
        case "6h":
          maxDataPoints = 360;
          break;
        case "24h":
          maxDataPoints = 288;
          break;
        case "7d":
          maxDataPoints = 168;
          break;
      }
      // Reset charts
      temperatureData = [];
      humidityData = [];
      if (temperatureChart) temperatureChart.update();
      if (humidityChart) humidityChart.update();
    });
  }

  // Export buttons
  const chartExportBtn = document.querySelector(".chart-export");
  if (chartExportBtn) {
    chartExportBtn.addEventListener("click", exportChartData);
  }
}

function exportChartData() {
  const data = {
    temperature: temperatureData.map((d) => ({
      time: d.time.toISOString(),
      value: d.value,
    })),
    humidity: humidityData.map((d) => ({
      time: d.time.toISOString(),
      value: d.value,
    })),
  };

  const blob = new Blob([JSON.stringify(data, null, 2)], {
    type: "application/json",
  });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = `sensor-data-${new Date().toISOString()}.json`;
  a.click();
  URL.revokeObjectURL(url);

  console.log("✅ Đã xuất dữ liệu biểu đồ thành công");
}
