import serial
import time
import pygame
import sys

bot = serial.Serial('/dev/rfcomm1',baudrate=9600)
#bot.open()
print(bot.name)
#print(bot.write(b'10'))
#print(bot.read())

display = pygame.display.set_mode((200, 200))
pygame.display.set_caption('TicTacTow')
clock = pygame.time.Clock()

x = 0
y = 0

X = 0

def GetPos():
        x = 100
        y = 100
        #if pygame.mouse.get_focused():
        for event in pygame.event.get():
            x,y = pygame.mouse.get_pos()
            if event.type == pygame.QUIT: sys.exit()
        x,y = pygame.mouse.get_pos()
        return x-100,100-y

while True:
    display.fill((255,255,255))
    x,y = GetPos()
    #print()
    print(x,y)
    bot.write(f"J{x} {y}".encode("ascii"))
    
    while bot.read(1) != "\n":
        print("Wait")
    #print(bot.readable().decode("ascii"))
    pygame.time.wait(100)
    
    