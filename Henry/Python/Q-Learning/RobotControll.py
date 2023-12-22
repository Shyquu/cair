import random
#from enum import Enum
import numpy as np
import torch
import os
import platform
import time
import sys
import agent
import serial


class BotControll:
    def __init__(self):
        print("Init")
        if not(self.Coneckt()):
            sys.exit()
        self.Agent = agent.Agent()
        self.time = time.time()
        self.reset()
        self.Sens = [0,0,0]
       
    def reset(self):
        
        self.Agent.reset()
    
    def Coneckt(self):
        T = False
        try:
            self.bot = serial.Serial('/dev/rfcomm1',baudrate=9600)
            T = True
        except:
            try:
                self.bot = serial.Serial('/dev/rfcomm0',baudrate=9600)
                T = True
            except:
                print("Robot not found")
                return False
            
        if T:
            print(self.bot.name)
            self.bot.write('#'.encode())
            print(self.bot.read_all())
            #self.Read()
            return True
        
      
    def checkDirectory(self):
        for F in ["/Seminar", "/cair", "/Henry", "/Python", "/Q-Learning"]:
            if not (F in os.getcwd()):
                if os.path.isdir("."+F):
                    os.chdir("."+F)
                else:
                    print("Not in right direktory")
                    return False
    
    def Read(self):
        tmp = self.bot.readline()
        tmp = str(tmp.decode()).replace("\r", "")
        tmp = tmp.replace("\n", "")
        numbers = tmp.split(",")
        for i in range(len(numbers)):
            try:
                self.Sens[i] = int(numbers[i])
            except:
                pass
        #tmp = tmp.strip()
        #tmp = tmp.decode()
        #print(numbers, (int(numbers[1]) <= 20))
        print(str(tmp))
        self.bot.read_all()
              
    def Run(self):
        print("Start")
        self.bot.write(f"o".encode("ascii"))
        self.Read()
        time.sleep(10)
        while True:
            if self.Sens[1] <= 20:
                self.bot.write(f"RR".encode("ascii"))
            else:
                self.bot.write(f"FF".encode("ascii"))
            self.Read()
            time.sleep(0.1)

Bot = BotControll()

Bot.Run()