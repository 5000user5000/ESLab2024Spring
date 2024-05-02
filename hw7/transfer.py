import binascii

def hex_to_binary_file(filename, output_file):
    with open(filename) as f:
        a = f.read()
    hex_string = a  # "Hello World" 的十六进制表示

    # 将十六进制字符串转换为二进制数据
    binary_data = binascii.unhexlify(hex_string)
    
    # 将二进制数据写入文件
    with open(output_file, 'wb') as file:
        file.write(binary_data)

# 示例十六进制字符串j

# 输出的文件名
output_file = "filter.wav"
output1 = "raw.wav"
# 转换并写入文件


hex_to_binary_file("raw.txt", "raw.wav")
hex_to_binary_file("filter.txt", "filter.wav")



