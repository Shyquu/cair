import serial
import time
import sys

bot = serial.Serial('/dev/rfcomm1',baudrate=9600)
#bot.open()
print(bot.name)
#print(bot.write(b'10'))
#print(bot.read())


Test = "11 33"

Time = time.time()

print(Test.encode("ascii"))
bot.write(Test.encode("ascii"))
    
#print(bot.read(len(Test)))
tmp = ""
while not('\n' in tmp):
    print(tmp)
    tmp = tmp + str(bot.read(1))
    
print(f"{(time.time()-Time):.4f}s")

#print(bot.readable().decode("ascii"))

    
    