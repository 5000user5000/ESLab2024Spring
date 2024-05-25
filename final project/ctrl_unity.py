import pynput.mouse
import socket
import json
import threading
import time
from queue import Queue

period = .05  # in sec
data_queue = Queue()
mouse_controller = pynput.mouse.Controller()
mouse_down = False

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
    while True:
        if data_queue:
            dx, dy = data_queue.get()
            # x, y = mouse_controller.position

            if dx == 0 and dy == 0:
                if mouse_down:
                    mouse_down = False
                    mouse_controller.release(pynput.mouse.Button.left)
            else:
                if not mouse_down:
                    mouse_down = True
                    mouse_controller.press(pynput.mouse.Button.left)
                mouse_controller.move(-dx, dy)

        time.sleep(period)

def socket_listener(HOST, PORT):
    global data_queue
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
                        break  # break connection
                    print("Received from socket server:", data)

                    # data parsing
                    if data.count('{') != 1:
                        continue  # skip incomplete data
                    obj = json.loads(data)

                    # data preprocessing
                    dx, dy = parse_command(obj)
                    # add to queue
                    data_queue.put([dx, dy])

                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue

def main():
    # 初始設定
    with open('./final project/setting.json') as f:
        config = json.load(f)

    HOST = config['host']
    PORT = config['port']

    # Start socket listener thread
    socket_thread = threading.Thread(target=socket_listener, args=(HOST, PORT))
    socket_thread.start()

    # Start act thread
    act_thread = threading.Thread(target=act)
    act_thread.start()

    socket_thread.join()
    act_thread.join()

if __name__ == '__main__':
    main()