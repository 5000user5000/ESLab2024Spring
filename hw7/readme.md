# 1.無錄音版本

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
- 用這裡的 `main.cpp` 替換專案原有的 `main.cpp` 檔案。

### 步驟 5: 編譯與執行

- 編譯完成後，執行專案。

# 2.錄音版本

### 1.沿用專案

- 沿用無錄音版的專案

- 把這裡的 `MP34DT01` 檔案夾下載，並放置於專案中。

### 2.替換檔案

- 把這裡的 `record_main.cpp` 複製挑上到專案原有的 `main.cpp` 檔案。

### 3.錄音前準備

- 先編譯好程式

- 錄音前放置兩個音樂，[音樂 1](https://www.researchgate.net/publication/293335234_Low-Pass_Filter_audio_example_wav_Hey_Jude)，[音樂 2](https://www.youtube.com/watch?v=qNf9nzvnd1k)。音樂一請你到該網站去找 File，下載並撥放 Hey Jude_LP.wav；音樂 2 請播放頻率較高的地方，目的是讓過濾後的頻譜較易觀察。

### 4.錄音與轉換

- 按下藍色的 user button，開始錄音，時長 1 秒

- 終端會顯示 hex 值，分成上下兩段，上段是尚未濾波，下段是濾波後。

- 把 hex 值貼到記事本上，並分別命名成 `raw.txt`、`filter.txt` 當中。(這裡我們有提供範本)

- 執行 `transfer.py` 來將 txt 檔轉換成 wav 檔。

### 5.視覺化

- 執行 `show.ipynb` 把 wav 音檔視覺化，上兩個 cell 是轉成波形圖，下兩個 cell 是轉成頻譜圖。

- 觀察尚未濾波以及濾波後的頻譜圖差異。

- 理論上，大約 2k Hz 以上的聲波會被濾掉。
