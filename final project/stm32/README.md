# STM32端程式

## Mbed Studio專案建立方法

1. 以內建的**MBED OS 6 / mbed-os-example-sockets**為基礎建立專案
1. 安裝BSP，網址如下：

   ```
   http://os.mbed.com/teams/ST/code/BSP_B-L475E-IOT01/
   ```
1. 把這個資料夾的`.h`和`.cpp`檔案全部複製到專案的`source`資料夾內
   - 如果有正確複製，應該會覆蓋掉原有的`source/main.cpp`
1. 在`mbed_app.json`修改設定：

   ```diff
    {
      "target_overrides": {
        "*": {
   +      "target_printf_lib": "std",
   ```

## 參數調整

- `mbed_app.json`中可以設定Wi-Fi連線
  - `nsapi.default-wifi-ssid`是SSID
  - `nsapi.default-wifi-password`是密碼
  - 參數前後的`\"`要保留，程式才能正確編譯
- `main.cpp`中可以設定電腦的IP位址，預設值如下：

  ```cpp
  constexpr char HOSTNAME[] = "192.168.50.200";
  ```
- `main.cpp`中可以設定發送資料的週期，預設值如下：

  ```cpp
  constexpr milliseconds EVENT_INTERVAL = 100ms;
  ```

## 資料格式

程式會定時以JSON格式發送三軸加速度計的讀數，數值都是有號16位元整數。

```json
{
  "x": 0,
  "y": 0,
  "z": 0
}
```
