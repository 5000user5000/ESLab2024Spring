# server socket
import socket
import json
import matplotlib.pyplot as plot

HOST = "192.168.1.126"
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print("Connected at", addr)
        while True:
            try:
                data = conn.recv(1024).decode('utf-8')
                print("Received from socket server:", data)
                if (data.count('{') != 1):
                # Incomplete data are received.
                    choose = 0
                    buffer_data = data.split('}')
                    while buffer_data[choose][0] != '{':
                        choose += 1
                    data = buffer_data[choose] + '}'
                obj = json.loads(data)
                t = obj['s']
                plot.scatter(t, obj['x'], c='blue') # x, y, z, gx, gy, gz
                plot.scatter(t, obj['y'], c='red')
                plot.scatter(t, obj['z'], c='green')
                plot.xlabel("sample num")
                plot.ylabel("gyro")
                plot.pause(0.0001)
            except (RuntimeError):
                break