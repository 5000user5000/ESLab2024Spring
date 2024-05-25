import pynput.mouse
import socket
import json
import threading
import time
from datetime import datetime
from queue import Queue
from concurrent.futures import ThreadPoolExecutor

period = .05  # in sec
data_queue = Queue()
time_log = []  # 用于记录延迟时间
mouse_controller = pynput.mouse.Controller()
mouse_down = False

def parse_command(data):
    x = int(data['x'])
    y = int(data['y'])
    threshold, cap = 20, 500
    scale = 1/cap
    pixel_per_second = 200
    mag = pixel_per_second * period * scale  # maximum pixel per datapoint * input data normalizer
    dx, dy = [0, 0]

    if x > threshold:
        dx = mag * max(x-threshold, cap)
    elif x < -threshold:
        dx = mag * min(x+threshold, -cap)

    if y > threshold:
        dy = mag * max(y-threshold, cap)
    elif y < -threshold:
        dy = mag * min(y+threshold, -cap)

    return dx, dy

def act():
    global mouse_down, time_log
    while True:
        dx, dy, timestamp = data_queue.get()
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

        # 记录当前时间并计算延迟
        end_time = datetime.now()
        delay = (end_time - timestamp).total_seconds()
        time_log.append(delay)

        if len(time_log) >= 100:
            break

def socket_listener(HOST, PORT):
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
                    timestamp = datetime.now()
                    if not data:
                        break  # break connection

                    # data parsing
                    if data.count('{') != 1:
                        continue  # skip incomplete data
                    obj = json.loads(data)

                    # data preprocessing
                    dx, dy = parse_command(obj)
                    # add to queue
                    data_queue.put([dx, dy, timestamp])
                    if len(time_log) >= 100:
                        conn.close()
                        break
                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue

def main():
    # 初始設定
    with open('./final project/setting.json') as f:
        config = json.load(f)

    HOST = config['host']
    PORT = config['port']

    with ThreadPoolExecutor(max_workers=2) as executor:
        executor.submit(act)
        executor.submit(socket_listener, HOST, PORT)

    # Print delay statistics
    if time_log:
        avg_delay = sum(time_log) / len(time_log)
        max_delay = max(time_log)
        min_delay = min(time_log)
        print(f"Average delay: {avg_delay:.6f} seconds")
        print(f"Max delay: {max_delay:.6f} seconds")
        print(f"Min delay: {min_delay:.6f} seconds")

if __name__ == '__main__':
    main()