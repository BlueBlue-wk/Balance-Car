import serial
import pygame
import sys


def check_keyboard_events(event, ser):
    """
    监视键盘和鼠标事件
    """
    if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_RIGHT:
            ser.write(bytes.fromhex('CE 01 0A'))
        if event.key == pygame.K_LEFT:
            ser.write(bytes.fromhex('CE 02 0A'))
        if event.key == pygame.K_UP:
            ser.write(bytes.fromhex('CF 02 0A'))
        if event.key == pygame.K_DOWN:
            ser.write(bytes.fromhex('CF 01 0A'))

    if event.type == pygame.KEYUP:
        if event.key == pygame.K_RIGHT:
            ser.write(bytes.fromhex('CE 01 00'))
        if event.key == pygame.K_LEFT:
            ser.write(bytes.fromhex('CE 01 00'))
        if event.key == pygame.K_UP:
            ser.write(bytes.fromhex('CF 01 00'))
        if event.key == pygame.K_DOWN:
            ser.write(bytes.fromhex('CF 01 00'))


def check_event(ser):
    """
        监视键盘和鼠标事件
    """
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        # 监视键盘
        check_keyboard_events(event, ser)


def run_game():
    # 初始化游戏并创建一个屏幕对象
    pygame.init()
    pygame.display.set_mode((50, 50))
    ser = serial.Serial('COM14', 115200)  # 此处串口号需与电脑的设备管理器中的端口号一致
    # ser.open()  # 会打开这个串口
    while True:
        # 监视键盘和鼠标事件
        check_event(ser)


run_game()
