import binascii

def hex_to_binary_file(filename, output_file):
    with open(filename) as f:
        a = f.read()
    hex_string = a  # "Hello World" 的十六進制表示

    # 將十六進制字符串轉換成二進制數據  
    binary_data = binascii.unhexlify(hex_string)
    
    # 將二進制數據寫入文件 
    with open(output_file, 'wb') as file:
        file.write(binary_data)

# 轉換並寫入檔案
hex_to_binary_file("raw.txt", "raw.wav")
hex_to_binary_file("filter.txt", "filter.wav")



