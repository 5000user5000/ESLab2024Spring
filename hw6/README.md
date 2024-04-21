# P1 實驗步驟

### 1. 建立專案

- 建立一個新的空白專案。

### 2. 配置程式

- 打開專案中的 `main.cpp` 檔案並將其內容替換為這裡的 `p1_main.cpp`。
- 修改 `tim1Prescaler` 和 `tim1Period` 的值以改變 timer 的配置，從而觀察輸出速度的變化。
- 進行 `channelConfig.SamplingTime` 的參數調整，使用不同的採樣時間參數以測試其對輸出值大小的影響。記得使用 F12 快速跳轉至參數的定義處。

### 3. 實驗觀察

- 進行實驗並使用表格或清單的形式記錄不同參數設置下的輸出變化，以及觀察實驗值和理論值的差距。

# P2 實驗步驟

### 1. 配置程式

- 使用已經建立好的 P1 專案作為基礎。
- 將 `main.cpp` 的內容替換為 `p2_main.cpp`。

### 2. 編譯與執行

- 編譯並執行專案。

### 3. 實驗觀察

- 計時填滿半個 buffer(打印完 128 筆數據)的時長，得到實驗值，並觀察實驗值和理論值的差距。

# P3 實驗步驟

### 1. 配置程式

- Mbed Studio import audio program (https://github.com/janjongboom/b-l475e-iot01a-audio-mbed/tree/master)
- 將 `main.cpp` 的內容替換為 `p3_main.cpp`。

p.s. 也可以先 git clone audio program，並把 hw_patch 檔案放入這個 audio program，然後執行以下指令 `git apply hw_patch` 或是 `git apply hw_patch.txt` ，得到 p3_main.cpp。

### 2. 編譯與執行

- 編譯並執行專案

### 3. 實驗觀察

- 使用 logic analyzer，接上 D2、D3 腳位，觀察這兩個腳位的電壓變化(看起來應該會是規律的方波)，以及相對的關係。
- 根據方波的週期，去找出填滿半個 buffer 的時間長度，也就是測量值，以及計算出理論值，觀察這兩者是否存在著差異。
