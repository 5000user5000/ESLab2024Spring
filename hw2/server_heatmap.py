import socket
import json
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns  # 導入seaborn

HOST = "192.168.1.126"  # 服務器地址
PORT = 8080             # 服務器端口

# 初始化繪圖
plt.ion()  # 啟用交互模式
sns.set_theme()  # 使用seaborn的默認配置增強繪圖美觀

data_buffer = {'s': [], 'x': [], 'y': [], 'z': []}

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Listening on {HOST}:{PORT}")
    conn, addr = s.accept()
    with conn:
        print("Connected by", addr)
        while True:
            try:
                data = conn.recv(1024).decode('utf-8')
                if not data:
                    break  # 斷開連接
                print("Received from socket server:", data)
                
                # 數據解析
                if (data.count('{') != 1):
                    continue  # 跳過不完整的數據
                obj = json.loads(data)
                
                # 更新數據緩存
                for key in data_buffer.keys():
                    data_buffer[key].append(obj[key])
                
                # 當收集到足夠的數據後，更新熱力圖
                if len(data_buffer['s']) % 10 == 0:  # 每收集到10個數據點，更新一次熱力圖
                    plt.clf()  # 清除之前的圖像
                    heatmap_data = np.array([data_buffer['x'], data_buffer['y'], data_buffer['z']])
                    sns.heatmap(heatmap_data, cmap='viridis', annot=False)
                    plt.draw()
                    plt.pause(0.05)  # 短暫暫停以更新圖表
                
            except json.JSONDecodeError:
                print("JSON Decode Error")
                continue

plt.ioff()  # 關閉交互模式
plt.show()  # 顯示圖表