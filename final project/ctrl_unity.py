import pyautogui
import socket
import json


def preprocess(data):
    # data preprocessing
    # Your algorithm here
    # 簡易範例
    command = []  # 初始化指令
    command[0] = 'left'  # 第一格放方位，'left', 'right', 'up', 'down'
    command[1] = sum(data['s']) / len(data['s']) # 第二格放位移量

    return command


def action(command):
    x, y = pyautogui.position() # 取得當前滑鼠位置
    delta = command[1] # 位移量
    # action
    if command[0] == 'left':
        pyautogui.dragTo( x - delta ,y,button='left')  # 按住左鍵並拖曳滑鼠到指定位置
    elif command[0] == 'right':
        pyautogui.dragTo( x + delta ,y,button='left')  
    elif command[0] == 'up':
        pyautogui.dragTo( x  , y - delta ,button='left')  
    elif command[0] == 'down':
        pyautogui.dragTo( x  ,y + delta ,button='left')  
    # 維持原狀
    else:
        pass


def main():
    # 初始設定
    with open('setting.json') as f:
        config = json.load(f)

    HOST = config['host']  # server address
    PORT = config['port']  # server port

    data_buffer = {'s': [], 'x': [], 'y': [], 'z': []}

    # 鼠標移動到初始遊戲位置
    pyautogui.moveTo(config['game_initX'], config['game_initY'])
    pyautogui.click()

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
                    for key in data_buffer.keys():
                        data_buffer[key].append(obj[key])
                    
                    # data preprocessing
                    command = preprocess(data_buffer)
                    # action
                    action(command)
                    
                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue


if __name__ == '__main__':
    main()