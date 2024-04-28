### 步驟 1: 建立專案

- 開啟 Mbed Studio。
- 建立一個新的空白專案（empty project）。

### 步驟 2: 添加庫

- 使用 `add library` 的方式，將 mbed-dsp 庫添加到專案中。庫的連結為：[mbed-dsp](https://os.mbed.com/teams/mbed-official/code/mbed-dsp)

### 步驟 3: 修改檔案

- 找到檔案 `arm_bitreversal2.S`，在原第 43 行前加入以下代碼：
  ```c
  #define __CC_ARM
  ```

### 步驟 4: 替換和添加檔案

- 下載 `arm_fir_data.cpp`，並將其添加到專案中。
- 用這裡的 `main.cpp` 替換原有的 `main.cpp` 檔案。

### 步驟 5: 編譯與執行

- 編譯完成後，執行專案。
