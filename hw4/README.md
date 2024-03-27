# 如何設定你的 BLE 專案

## 開始之前

首先，確保你已經安裝了 Mbed Studio。接下來，跟著以下步驟：

### 1. 建立專案

- 在 Mbed Studio 中建立一個新的空白專案。

### 2. 添加服務

- 把 hw4 底下的檔案，除 python 檔外全部複製到你的空專案中。
- 或是你也可以從將 `BLE_GattServer_AddService` 檔案夾底下的所有內容複製到你的空專案中。 (來自[BLE_GattServer_AddService](https://github.com/ARMmbed/mbed-os-example-ble/tree/development/BLE_GattServer_AddService))。並把專案中的 `./source` 資料夾裡的檔案換成這裡的四個新檔案， `mbed_app.json` 檔案也換成這裡的版本。

### 3. 編譯與燒錄

- 接下來，直接編譯並燒錄到你的 STM32 裝置中，然後運行它。

### 4. 驗證 BLE 信號

- 使用 Android BLE Scanner app 接收信號，確認 STM32 的 MAC 地址，並檢查是否能從 GATT 通知中收到按鈕訊息和心率資訊。

> [!NOTE]
> 這版STM32端的程式會在每次按鈕的時候更改一次狀態，  
> 初始狀態是`0`，按一下之後是`1`，再按變回`0`，以此規律不斷循環。  
> LED1會顯示目前的狀態，可以用於對照GATT的資料。

## 設定 Raspberry Pi

將 `ble_scan_connect.py` 檔案透過 SCP 或其他方法傳送到你的 Raspberry Pi 上：

1. 在 Raspberry Pi 上執行以下命令，並根據先前得到的 stm32 MAC 地址連接上 stm32：
   ```
   sudo python ble_scan_connect.py
   ```
2. 從 terminal 上觀察結果，可以按下 stm32 user button 觀察 button 訊息的變化。
