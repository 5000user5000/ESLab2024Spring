# Raspberry Pi 藍牙連接指南

本指南將透過簡單步驟引導您如何設置 Raspberry Pi 以進行藍牙掃描和連接，並與 Android 手機上的 GATT 服務互動。

## 前提條件

- Raspberry Pi（已安裝 Raspbian OS）
- 有藍牙功能的 Android 手機
- 確保 Raspberry Pi 已連接至網路

## 步驟 1：準備 Raspberry Pi

### 1.1 將腳本傳輸至 Raspberry Pi

使用以下 SCP 指令將`ble_scan_connect.py`檔案傳輸至您的 Raspberry Pi：

```
scp ble_scan_connect.py pi@<RaspberryPi的IP位址>:/路徑/到/欲放置/檔案
```

### 1.2 安裝必要套件

在 Raspberry Pi 上安裝`bluepy`套件：

```
pip install bluepy
```

## 步驟 2：設置 Android GATT 服務

### 2.1 下載並安裝 App

在您的 Android 手機上，安裝[Nordic Semiconductor 的 nRF Connect for Mobile](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=zh_TW&gl=US)。

### 2.2 配置 GATT 伺服器

1. 打開 app，並在左側工作欄選擇`Configure GATT server`。
2. 確定是預設的`Sample configuration`。

### 2.3 設定與啟動廣播

在 app 中的`advertiser`，新增一個`advertising packet`，注意`advertising data`那邊`ADD RECORD`，選擇`Complete Local Name`，如此才能方便找到您的手機。設定完成後便直接開始廣播您的 GATT 服務。

## 步驟 3：從 Raspberry Pi 掃描和連接

把 ble_scan_connect.py 傳到您的 Raspberry Pi 上，並執行以下指令來掃描並嘗試連接到您的 Android GATT 服務：

```
python ble_scan_connect.py
```

如果首次連接失敗，請再次執行此指令。成功後，您將看到一個包含可連接裝置的列表。根據 MAC 地址或 local name 連接到您的手機。

## 觀察和驗證

連接成功後，請在 Android 手機上檢查 GATT 服務的值是否有變動，並在 Raspberry Pi 上查看 log 檔案以確認是否成功接收到來自 client 的資料。
`ble_scan_connect.py` 預設是開啟`Notification`，所以您可以看到資料從手機傳到 Raspberry Pi。
