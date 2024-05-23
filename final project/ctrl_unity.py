import pyautogui
import socket
import json

period = .05  # in sec
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
 
mouse = False
def act(dx, dy):
    global mouse
    x, y = pyautogui.position()
 
    if dx == 0 and dy == 0:
        if mouse:
            mouse = False
            pyautogui.mouseUp()
    else:
        if not mouse:
            mouse = True
            pyautogui.mouseDown()
        print(f"move: {dx}, {dy}")
        pyautogui.moveTo(x-dx, y+dy, duration=period-.01)

def main():
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
                    data = conn.recv(1024).decode('utf-8')
                    if not data:
                        break  # break connection
                    print("Received from socket server:", data)
                    
                    # data parsing
                    if (data.count('{') != 1):
                        continue  # skip incomplete data
                    obj = json.loads(data)
                    
                    # update data buffer
                    # for key in data_buffer.keys():
                    #     data_buffer[key].append(obj[key])
                    
                    # data preprocessing
                    dx, dy = parse_command(obj)
                    # action
                    act(dx, dy)
                    
                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue


if __name__ == '__main__':
    main()
