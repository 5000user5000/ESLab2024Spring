import socket
import json
import matplotlib.pyplot as plt

HOST = "192.168.1.126"  # 服務器地址
PORT = 8080             # 服務器端口

# 初始化繪圖
plt.ion()  # 啟用交互模式
fig, ax = plt.subplots()  # 創建圖表和軸
ax.set_xlabel("Sample Num")
ax.set_ylabel("Gyro")

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
                
                # 繪製數據
                ax.clear()
                ax.scatter(data_buffer['s'], data_buffer['x'], c='blue', label='X-axis')
                ax.scatter(data_buffer['s'], data_buffer['y'], c='red', label='Y-axis')
                ax.scatter(data_buffer['s'], data_buffer['z'], c='green', label='Z-axis')
                ax.legend()
                plt.pause(0.05)  # 短暫暫停以更新圖表
                
            except json.JSONDecodeError:
                print("JSON Decode Error")
                continue

plt.ioff()  # 關閉交互模式
plt.show()  # 顯示圖表
