import pyautogui
import socket
import json



def preprocess(data):
    # data preprocessing
    # Your algorithm here
    # 簡易範例
    x = int(data['x'])
    y = int(data['y'])
    threshold = 270
    command = []
    delta = 110
    if(x > threshold):
        print("left")
        command.append('left')
        delta *= (x-threshold)/threshold
    elif(x < -threshold ):
        print("right")
        command.append('right')
        delta *= (-x-threshold)/threshold
    elif(y < -threshold):
        print("up")
        command.append('up')
        delta *= (-y-threshold)/threshold
    elif(y > threshold):
        print("down")
        command.append('down')
        delta *= (y-threshold)/threshold
    else:
        command.append('stay')
    delta*=1.1
    command.append(delta)
    print(command)
    return command


def action(command):
    x, y = pyautogui.position() # 取得當前滑鼠位置
    # print(f"Move {command[0]} {delta} pixels")
    instr = command[0]
    delta = command[1]
    # action
    if instr != 'stay':
        pyautogui.mouseDown()

    if instr == 'left':
        pyautogui.moveTo( x - delta ,y,duration=0.25)  # 按住左鍵並拖曳滑鼠到指定位置
    elif instr == 'right':
        pyautogui.moveTo( x + delta ,y,duration=0.25)  
    elif instr == 'up':
        pyautogui.moveTo( x  , y - delta ,duration=0.25)  
    elif instr == 'down':
        pyautogui.moveTo( x  ,y + delta,duration=0.25)  
    # 維持原狀
    else:
        pass
    pyautogui.mouseUp()
    


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
                    command = preprocess(obj)
                    # action
                    action(command)
                    
                except json.JSONDecodeError:
                    print("JSON Decode Error")
                    continue


if __name__ == '__main__':
    main()
