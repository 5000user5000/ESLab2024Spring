# Mbed OS Example - Socket Communication and Sensor Data Collection

這是嵌入式系統實驗課程的Hw2，在Mbed OS上收集感測器資料，並透過無線網路傳送資料到socket伺服器上。

## 開始前的準備

- 安裝[Mbed Studio](https://os.mbed.com/studio/)，這是開發和管理 Mbed OS 專案的IDE。

## 建立專案

### 以Socket範例為基礎建立專案

1. **啟動 Mbed Studio**：打開 Mbed Studio。
1. **創建新專案**：選擇「File > Import Program」選項。
1. **設定專案來源**：
   - 在URL欄位填入`https://github.com/ARMmbed/mbed-os-example-sockets`，
   - 在Program name填入適當的名稱。

這會將`mbed-os-example-sockets`專案複製到電腦上。

### 設定網路環境

1. **打開`mbed_app.json`文件**：在打開專案中的`mbed_app.json`。
2. **設定Wi-Fi**：
   - 在`mbed_app.json`中找出`nsapi.default-wifi-ssid`和`nsapi.default-wifi-password`鍵。
   - 將對應的值改為無線網絡的SSID和密碼，要保留前後的引號和反斜線（例如`"nsapi.default-wifi-ssid": "\"My SSID\""`）。
3. **設定伺服器位置**：
   - 在`hostname`中輸入伺服器的網址或IP，以替換掉預設的`ifconfig.io`。

### 新增BSP Library

1. **匯入library**：選擇「File > Add Library to Active Program」選項。
1. **設定BSP來源**：
   - 在URL欄位填入`http://os.mbed.com/teams/ST/code/BSP_B-L475E-IOT01/`，
   - Name使用預設的`BSP_B-L475E-IOT01`。

### 更新主程式

1. **下載main.cpp**：從這個repo的hw2取得main.cpp。
1. **覆蓋main.cpp**：用下載的main.cpp，把專案中的source/main.cpp覆蓋掉。

### 編譯、燒錄

- 再次確認Active Program和Target設定無誤。
- 使用Mbed Studio的編譯工具編譯專案，然後燒錄到開發板上。

## 伺服器端設定

以下是本repo提供的伺服器程式：

### `server.py`

- **功能**：接收數據並繪製散布圖。

### `server2.py`

- **功能**：在`server.py`的基礎上增加了暫存機制，允許在進行數據處理之前收集多筆數據，適用於需要進行批量處理的場合。

### `server_heatmap.py`

- **功能**：生成數據的熱圖，適合於需要視覺化展示數據密集度或某些變量分布的應用。

### `server_linemap.py`

- **功能**：繪製數據隨時間變化的折線圖，特別適用於展示時間序列數據，如感測器讀數隨時間的變化。

### `server_web.py` (Flask-app)

- **功能**：提供一個Web界面，即時展示數據圖表。如果需要一個互動式的網頁介面來顯示數據，可以使用這個基於Flask的程式。

為了執行這些程式，電腦上需要有Python環境，並且有以下設定：

1. 安裝相關dependency。如果沒有，執行`pip install -r requirements.txt`即可安裝。
1. 在程式中設定您的伺服器的IP位址，確保開發板能夠傳送資料。

