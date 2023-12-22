import torch
import random
import numpy as np
from collections import deque
#from game import TicTacTowGame, Player, Point
from model import Linear_QNet
import torch.optim as optim
import torch.nn as nn
import time
import os

MAX_MEMORY = 100_000
BATCH_SIZE = 1000
LR = 0.01

class Agent:

    def __init__(self):
        self.device = "cuda" if torch.cuda.is_available() else "cpu"
        print(f"Using {self.device} device")
        self.device = "cpu"
        
        self.n_games = 0
        self.epsilon = 0 # randomness
        self.gamma = 0.9 # discount rate
        #self.memory = deque(maxlen=MAX_MEMORY) # popleft()
        self.model = Linear_QNet(6, 50, 3) #9, 270, 9
        self.lr=LR
        
        self.optimizer = optim.Adam(self.model.parameters(), lr=self.lr)
        self.criterion = nn.MSELoss()
        #self.trainer = QTrainer(self.model, lr=LR, gamma=self.gamma)
        #self.model.load()
        self.reset()

    
    def reset(self):
        self.state = torch.tensor([0,0,0,0,0,0], dtype=torch.float)
        #self.pred = torch.tensor([0,0,0], dtype=torch.float)

    def get_state(self, game):
        State = [0,0,0,0,0,0]
        State[0]=game.Player.X/game.w
        State[1]=game.Player.Y/game.h
        State[2]=game.Ball.X/game.w
        #State[3]=game.Ball.Y/game.h
        State[3]=((game.Player.X+(game.Player.w/2))-game.Ball.X)/game.w
        State[4]=game.Ball.Direktion[0]
        State[5]=game.Ball.Direktion[1]
        
        State = torch.tensor(State)
        #print(game.clock.get_fps())
        return State

    def get_action(self, state):
        # random moves: tradeoff exploration / exploitation
        #self.epsilon = self.trainingGames - self.n_games
        #final_move = [0 for _ in range(9)]

        if random.randint(0,30) == 21:
            move = random.randint(0,2)
        else:
        
            self.model = self.model.to(device=self.device)
            
            state0 = torch.tensor(state, dtype=torch.float).to(device=self.device)
            prediction = self.model(state0)
            move = torch.argmax(prediction).item()
        
        if int(move) == 0:
            return -1
        elif int(move) == 1:
            return 0
        elif int(move) == 2:
            return 1
    
    def load(self, model_folder_path = './model',file_name='model.pth'):
        if os.path.isfile(model_folder_path+"/"+file_name):
            self.model = torch.load(model_folder_path+"/"+file_name)

    def TrainStep(self,reward,next_state, Aktion):
        tim = time.time()
        state = self.state
        state = state.to(device=self.device)
        next_state = next_state.to(device=self.device)
             
        if Aktion == -1:
            Aktion = 0
        elif Aktion == 0:
            Aktion = 1
        elif Aktion == 1:
            Aktion = 2
        
        target = reward
        #if not done:
        next_s = self.model(next_state)
        target = reward + (self.gamma * torch.max(next_s))
        #action = next_s.clone()
        pred = self.model(state)
        target_f = pred.clone()
        target_f[Aktion] = target
        loss = self.criterion(target_f, pred)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()
        
        #print(pred, reward, target_f)
        
        self.state = next_state.clone()
        #print(target_f)
        #self.pred = next_s.clone()
        #print(round(time.time()-tim, 5), torch.argmax(action).item(), end="\r")

def train():
    plot_scores = []
    plot_mean_scores = []
    total_score = 0
    record = 0
    #agent = Agent()
    #game = TicTacTowGame()
    '''
    while True:
        # get old state
        state_old = agent.get_state(game)

        # get move
        final_move = agent.get_action(state_old)

        # perform move and get new state
        reward, done, score = game.play_step(final_move, Player.X)
        state_new = agent.get_state(game)

        # train short memory
        agent.train_short_memory(state_old, final_move, reward, state_new, done)

        # remember
        agent.remember(state_old, final_move, reward, state_new, done)

        if done:
            # train long memory, plot result
            game.reset()
            agent.n_games += 1
            #agent.train_long_memory() #Live spiel Kein nachlernen (naja mal schauen)

            if score > record:
                record = score
                agent.model.save()

            print('Game', agent.n_games, 'Score', score, 'Record:', record)

            plot_scores.append(score)
            total_score += score
            mean_score = total_score / agent.n_games
            plot_mean_scores.append(mean_score)
            plot(plot_scores, plot_mean_scores)'''


if __name__ == '__main__':
    import main