from flask import Flask, render_template, jsonify
import socket
import json
import threading

app = Flask(__name__)

HOST = "192.168.1.126"  # 服务器地址
PORT = 8080             # 服务器端口

data_buffer = {'s': [], 'x': [], 'y': [], 'z': []}

def socket_thread():
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
                        break
                    print("Received from socket server:", data)
                    
                    if data.count('{') != 1:
                        continue
                    obj = json.loads(data)
                    
                    # 限制data_buffer的大小以避免内存溢出
                    max_length = 50
                    for key in data_buffer.keys():
                        if len(data_buffer[key]) >= max_length:
                            data_buffer[key].pop(0)
                        data_buffer[key].append(obj[key])
                
                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def get_data():
    return jsonify(data_buffer)

if __name__ == '__main__':
    threading.Thread(target=socket_thread).start()
    app.run(debug=True, use_reloader=False)
