import socket
import json
import matplotlib.pyplot as plt
import select

HOST = "192.168.0.1"  # 服務器地址
PORT = 8081             # 服務器端口

# 初始化繪圖
plt.ion()  # 啟用交互模式
fig, ax = plt.subplots()  # 創建圖表和軸
ax.set_xlabel("Sample Num")
ax.set_ylabel("Gyro")

data_buffer = {'s': [], 'x': [], 'y': [], 'z': []}

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT))
server_socket.listen()
rlist = [server_socket]
while True:
    readable, _, _ = select.select(rlist, [], [])
    for s in readable:
        if s is server_socket:
            client_socket, addr = server_socket.accept()
            # print(f'Connected by {addr}')
            # print(f"connect by {client_socket}")
            client_socket.setblocking(False)
            rlist.append(client_socket)
        else:
            try:
                data = s.recv(1024)
                if not data:
                    break  # break connection
                s.sendall((data))

            except json.JSONDecodeError:
                print("JSON Decode Error")
                continue
