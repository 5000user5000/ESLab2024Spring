# Mbed OS Example - Socket Communication and Sensor Data Collection

本指南將帶領您通過設置一個在 Mbed OS 上實現 socket 通信和感測器數據收集的專案步驟。這將涉及到配置設備以連接到您的無線網絡，收集感測器數據，並將這些數據通過網絡傳輸到一個伺服器。

## 開始前的準備

- 確保您已經安裝了 [Mbed Studio](https://os.mbed.com/studio/)，這是開發和管理 Mbed OS 專案的集成開發環境（IDE）。

## 建立您的專案

### 步驟 1: 創建 Socket 通信專案

1. **啟動 Mbed Studio**：打開 Mbed Studio。
2. **創建新專案**：選擇「新建程式」選項。
3. **設置專案源**：在出現的對話框中，將 URL `https://github.com/ARMmbed/mbed-os-example-sockets` 輸入作為代碼庫的源地址，然後點擊「添加」。這會將 `mbed-os-example-sockets` 專案複製到您的本地工作環境。

### 步驟 2: 配置網絡設置

1. **打開 `mbed_app.json` 文件**：在專案結構中尋找並打開 `mbed_app.json`。
2. **設置 WiFi 連接**：
   - 在 `mbed_app.json` 中尋找 `ssid` 和 `password` 鍵。
   - 將對應的值改為您無線網絡的 SSID 和密碼，注意保留引號和反斜杠（例如 `"\"your_wifi_ssid\""` 和 `"\"your_wifi_password\""`）。
3. **設定 IP 地址**：
   - 在 `hostname` 鍵的值中輸入您希望連接的服務器 IP 地址，以替換掉默認的 `ifconfig.io` 地址。

### 步驟 3: 整合感測器數據

1. **獲取感測器板支持包**：
   - 通過 Mbed Studio 創建另一個新專案，這次使用 URL `https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01-Sensors-BSP/` 獲取感測器板支持包。
2. **導入感測器庫**：
   - 在下載的感測器板支持包專案中，找到 `DISCO_L475VG_IOT01A/Drivers/BSP` 目錄並打開。
   - 將其底下的 `B-L475E-IOT01` 和 `Components` 兩個文件夾複製到 `mbed-os-example-sockets` 專案的 `include` 目錄下。
3. **更新主程序**：
   - 從此 repo 的 `main.cpp` 內容複製到 `mbed-os-example-sockets` 專案的 `source/main.cpp` 文件中。

### 步驟 4: 編譯並燒錄專案

- 在 Mbed Studio 中，確保 `mbed-os-example-sockets` 專案被選中作為當前專案。
- 使用 Mbed Studio 的編譯工具編譯專案，然後燒錄到您的開發板上。

## 服務器端配置與運行

在完成開發板端的設置後，您需要選擇一個服務器腳本來處理和展示從開發板接收到的數據。以下是您可以選擇運行的腳本及其功能：

### `server.py`

- **功能**: 接收數據並使用 GUI 介面呈現散狀圖。這是最直觀的方式來查看從您的開發板發送的數據。

### `server2.py`

- **功能**: 在 `server.py` 的基礎上增加了數據緩存機制，允許在進行數據處理之前收集多筆數據，適用於需要進行批量數據處理的場景。

### `server_heatmap.py`

- **功能**: 生成數據的熱力圖，適合於需要視覺化展示數據密集度或某些變量分布的應用。

### `server_linemap.py`

- **功能**: 繪製數據隨時間變化的線圖，特別適用於展示時間序列數據，如感測器讀數隨時間的變化。

### `server_web.py` (Flask-app)

- **功能**：提供一個 Web 界面，實時展示數據圖表。如果您需要一個交互式的網頁介面來監控和展示數據，這個基於 Flask 的腳本將非常適合。

為了運行這些腳本，您需要在一台裝有 Python 的電腦上進行如下設定：

1. 確保您的 Python 環境已安裝所需的依賴包。如沒有，請執行 `pip install -r requirements.txt` 命令即可安裝這些依賴。
2. 在腳本中設定您的服務器或開發機的 IP 地址，確保開發板能夠通過網絡與其通信。
3. 選擇並運行上述的任一腳本，根據您的需求來處理和展示數據。

## 結語

恭喜您！現在您應該已經成功設置了一個基於 Mbed OS 的專案，該專案可以通過 socket 通信從感測器收集數據並將數據發送到服務器。這個專案為物聯網應用提供了一個實用的起點，您可以根據需要進行調整和擴展。

如果在執行過程中遇到任何問題，請參考相關文檔或尋求社區的幫助。祝您開發愉快！
