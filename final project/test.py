import pynput.mouse
import socket
import json
import time
from datetime import datetime

period = .05  # in sec
latency_list = []
mouse_controller = pynput.mouse.Controller()
mouse_down = False
data_queue = []

def parse_command(data):
    x = int(data['x'])
    y = int(data['y'])
    threshold, cap = 20, 500
    scale = 1 / cap
    pixel_per_second = 200
    mag = pixel_per_second * period * scale  # maximum pixel per datapoint * input data normalizer
    dx, dy = [0, 0]

    if x > threshold:
        dx = mag * max(x - threshold, cap)
    elif x < -threshold:
        dx = mag * min(x + threshold, -cap)

    if y > threshold:
        dy = mag * max(y - threshold, cap)
    elif y < -threshold:
        dy = mag * min(y + threshold, -cap)

    return dx, dy

def act():
    global mouse_down
    if data_queue:
        dx, dy, start_time = data_queue.pop(0)
        x, y = mouse_controller.position

        if dx == 0 and dy == 0:
            if mouse_down:
                mouse_down = False
                mouse_controller.release(pynput.mouse.Button.left)
        else:
            if not mouse_down:
                mouse_down = True
                mouse_controller.press(pynput.mouse.Button.left)
            mouse_controller.move(-dx, dy)
        
        end_time = datetime.now()
        delay = (end_time - start_time).total_seconds()
        print(f"Latency: {delay:.6f} seconds")
        latency_list.append(delay)

    time.sleep(period)

def main():
    global latency_list, data_queue

    # 初始設定
    with open('./final project/setting.json') as f:
        config = json.load(f)

    HOST = config['host']
    PORT = config['port']

    # 連接 socket server
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Listening on {HOST}:{PORT}")
        conn, addr = s.accept()
        with conn:
            print("Connected by", addr)
            while True:
                try:
                    start_time = datetime.now()
                    data = conn.recv(1024).decode('utf-8')
                    if not data:
                        break  # break connection
                    print("Received from socket server:", data)
                    
                    # data parsing
                    if data.count('{') != 1:
                        continue  # skip incomplete data
                    obj = json.loads(data)
                    
                    # data preprocessing
                    dx, dy = parse_command(obj)
                    data_queue.append([dx, dy, start_time])
                    # action
                    act()

                    if len(latency_list) >= 100:
                        conn.close()
                        max_latency = max(latency_list)
                        min_latency = min(latency_list)
                        avg_latency = sum(latency_list) / len(latency_list)
                        print(f"Max latency: {max_latency} seconds")
                        print(f"Min latency: {min_latency} seconds")
                        print(f"Avg latency: {avg_latency} seconds")
                        break

                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue

if __name__ == '__main__':
    main()