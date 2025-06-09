import serial
import time


def send_byte(value):
    
    # 打开串口
    ser = serial.Serial('/dev/serial0', 9600, timeout=1)

    if not ser.isOpen():
        print("串口没打开")
        return
    
    if not (0 <= value <= 127):
        print("错误：请输入 0~127 之间的数字")
        return

    # 发送一个字节
    ser.write(bytes([value]))
    # print(f"已发送 0x{value:02X}")
        
    # 关闭串口
    ser.close()
    

