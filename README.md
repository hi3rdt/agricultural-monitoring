# Hệ Thống Giám Sát Nông Nghiệp IoT & Chẩn Đoán Bệnh Bằng AI 🌿

Một hệ sinh thái IoT end-to-end (từ phần cứng đến đám mây) được xây dựng để giám sát điều kiện môi trường, tự động hóa tưới tiêu và phát hiện bệnh cây trồng theo thời gian thực bằng Trí tuệ nhân tạo.

## 🚀 Bản Demo Trực Tuyến (Live Demo)

* **Frontend (Dashboard):** [https://agricultural-frontend.onrender.com/](https://agricultural-frontend.onrender.com/)
* **Backend (API Status):** [https://agricultural-backend.onrender.com/](https://agricultural-backend.onrender.com/)

---

## 🔗 Kho Lưu Trữ (Project Repositories)

Dự án này được chia thành ba thành phần (microservices). Code cho mỗi thành phần được lưu trữ tại các kho lưu trữ riêng biệt:

1.  **Frontend (Next.js):**
    * **Link:** [https://github.com/hi3rdt/agricultural-frontend.git](https://github.com/hi3rdt/agricultural-frontend.git)
    * Giao diện dashboard cho người dùng, xây dựng bằng Next.js và Shadcn UI.

2.  **Backend (FastAPI):**
    * **Link:** [https://github.com/hi3rdt/agricultural-backend.git](https://github.com/hi3rdt/agricultural-backend.git)
    * API server xử lý logic, chạy model AI (YOLO, Gemini) và giao tiếp với database.

3.  **Embedded (PlatformIO):**
    * **Link:** [https://github.com/hi3rdt/agricultural-embedded.git](https://github.com/hi3rdt/agricultural-monitoring.git)
    * Chứa code C++ cho cả ESP32 Master (cảm biến/bơm) và ESP32-CAM (camera).

---

## 🏛️ Kiến Trúc Hệ Thống (System Architecture)



Hệ thống sử dụng kiến trúc tách biệt:

1.  **Embedded (ESP32/ESP32-CAM):** Các thiết bị này hoạt động như client.
    * **ESP32 Master** đọc cảm biến (DHT22, Soil) và gửi (POST) lên Backend. Sau đó, nó lấy (GET) trạng thái điều khiển (chế độ, ngưỡng) để vận hành bơm.
    * **ESP32-CAM** liên tục hỏi (GET) Backend xem có lệnh chụp không. Khi có lệnh, nó chụp ảnh (với flash) và POST ảnh lên Backend.
2.  **Backend (FastAPI):** Là bộ não trung tâm.
    * Nhận dữ liệu từ các ESP32, lưu vào SQLite.
    * Nhận ảnh, kích hoạt **YOLO** để phát hiện bệnh.
    * Gọi **Gemini API** để phân tích bệnh và đề xuất điều trị.
    * Gửi cảnh báo qua **Telegram**.
    * Cung cấp API cho Frontend.
3.  **Frontend (Next.js):**
    * Hiển thị dữ liệu từ Backend API.
    * Gửi lệnh (thay đổi chế độ, bật/tắt bơm, yêu cầu chụp) đến Backend.

---

## 🛠️ Công Nghệ Sử Dụng (Tech Stack)

* **Backend:** FastAPI (Python), Uvicorn, YOLO, Google Gemini API, SQLite, HTTPX, `python-dotenv`
* **Frontend:** Next.js, React, TypeScript, Tailwind CSS, Shadcn UI, Recharts
* **Embedded:** C++, PlatformIO, ESP32, ESP32-CAM, WiFiManager, ArduinoJson
* **DevOps & Dịch vụ:** Render (CI/CD), Git, GitHub, Telegram Bot API, OpenWeatherMap API

---

## ⚙️ Hướng Dẫn Cài Đặt & Chạy (Local)

Để chạy dự án này trên máy local, bạn cần khởi chạy đồng thời cả 3 thành phần (Backend, Frontend, và Embedded).

### 1. Backend (FastAPI)

1.  **Clone repo:**
    ```bash
    git clone [https://github.com/hi3rdt/agricultural-backend.git](https://github.com/hi3rdt/agricultural-backend.git)
    cd agricultural-backend
    ```
2.  **Tạo môi trường ảo & Kích hoạt:**
    ```bash
    python -m venv venv
    .\venv\Scripts\activate  # (Trên Windows)
    # source venv/bin/activate # (Trên macOS/Linux)
    ```
3.  **Cài đặt thư viện:**
    ```bash
    pip install -r requirements.txt
    ```
4.  **Cấu hình:**
    * Tạo file `.env` trong thư mục gốc.
    * Sao chép nội dung từ `README.md` của repo backend (phần hướng dẫn `.env`) và điền các API keys (Gemini, Telegram, v.v.).
    * Đặt file model YOLO (`.pt`) của bạn vào thư mục gốc.
5.  **Chạy Server:** (Rất quan trọng: dùng `0.0.0.0` để ESP32 có thể kết nối)
    ```bash
    uvicorn main:app --reload --host 0.0.0.0 --port 8080
    ```

### 2. Frontend (Next.js)

1.  **Clone repo:** (Mở một terminal mới)
    ```bash
    git clone [https://github.com/hi3rdt/agricultural-frontend.git](https://github.com/hi3rdt/agricultural-frontend.git)
    cd agricultural-frontend
    ```
2.  **Cài đặt Node.js & pnpm:** Tải [Node.js](https://nodejs.org/) (khuyên dùng 18+). Sau đó chạy `npm install -g pnpm`.
3.  **Cài đặt thư viện:**
    ```bash
    pnpm install
    ```
4.  **Cấu hình:**
    * Tạo file `.env.local` trong thư mục gốc.
    * Thêm URL trỏ đến backend local của bạn (thay `192.168.x.x` bằng IP của máy bạn, tìm bằng lệnh `ipconfig`):
        ```dotenv
        NEXT_PUBLIC_API_BASE_URL="[http://192.168.](http://192.168.)x.x:8080"
        ```
5.  **Chạy Server:**
    ```bash
    pnpm dev
    ```
6.  Mở `http://localhost:3000` trên trình duyệt.

### 3. Embedded (ESP32 & ESP32-CAM)

1.  **Clone repo:** 
    ```bash
    git clone [https://github.com/hi3rdt/agricultural-monitoring.git](https://github.com/hi3rdt/agricultural-monitoring.git)
    ```
2.  **Mở dự án:** Mở thư mục `agricultural-monitoring` bằng **VS Code** đã cài đặt tiện ích **PlatformIO**.
3.  **Cấu hình (Nếu cần):** Kiểm tra file `lib/Config/Config.h`. Đảm bảo các `DEFAULT_..._URL` (cho ESP32-CAM) trỏ đúng đến IP và cổng của máy tính đang chạy FastAPI (ví dụ: `http://192.168.x.x:8080`).
4.  **Build & Upload:**
    * Kết nối ESP32 Master, chọn môi trường `esp32master` ở thanh trạng thái VS Code, và nhấn nút **Upload (➡️)**.
    * Kết nối ESP32-CAM, chọn môi trường `esp32cam`, và nhấn **Upload (➡️)**.
5.  **Thiết lập WiFi:**
    * Dùng điện thoại, kết nối vào Access Point (AP) do ESP32 tạo ra (ví dụ: `ESP32-Agri-Setup` hoặc `ESP32CamSetupAP`).
    * Một trang cấu hình sẽ mở ra. Chọn mạng WiFi nhà bạn, nhập mật khẩu.
    * (Đối với CAM) Xác nhận hoặc sửa lại các URL của Backend nếu cần. Nhấn **Save**.

Sau khi cả 3 thành phần đều chạy, hệ thống sẽ hoạt động.

---

## 👤 Tác Giả (Author)

* **Tên:** [Tên của bạn]
* **GitHub:** [@hi3rdt](https://github.com/hi3rdt)
* **LinkedIn:** (Thêm link LinkedIn của bạn)
