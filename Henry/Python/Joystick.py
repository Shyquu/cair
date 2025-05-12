import serial
import time
import pygame
import sys


try:
    bot = serial.Serial('/dev/rfcomm1',baudrate=9600)
except:
    try:
        bot = serial.Serial('/dev/rfcomm0',baudrate=9600)
    except:
        print("Dörp")
    
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

def Paint():
    display.fill((0,0,0))
    pygame.draw.line(display,(255,255,255),(100,0),(100,200))
    pygame.draw.line(display,(255,255,255),(0,100),(200,100))
    pygame.display.flip()

Paint()
bot.write(f"#".encode("ascii"))
while True:
    x,y = GetPos()
    #print()
    print(x,y)
    bot.write(f"j{x} {y}".encode("ascii"))
    
    #while bot.read(1) != "\n".encode("ascii"):
    #    print("Wait")
    #print(bot.readable().decode("ascii"))
    pygame.time.wait(500)
else:
    bot.write(f"#".encode("ascii"))
    
    