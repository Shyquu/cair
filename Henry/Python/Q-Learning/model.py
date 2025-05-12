import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import os
import time

class Linear_QNet(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super().__init__()
        self.linearIN = nn.Linear(input_size, hidden_size)
        self.linear1 = nn.Linear(hidden_size, hidden_size)
        #self.linear2 = nn.Linear(hidden_size, hidden_size)
        #self.linear3 = nn.Linear(hidden_size, hidden_size)
        self.linearOUT = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        x = self.linearIN(x)
        x = F.relu(x)
        x = self.linear1(x)
        x = F.relu(x)
        #x = self.linear2(x)
        #x = F.relu(x)
        #x = self.linear3(x)
        #x = F.relu(x)
        x = self.linearOUT(x)
        return x

    def save(self, file_name,rename = "", model_folder_path = './model'):
        
        if not os.path.exists(model_folder_path):
            os.makedirs(model_folder_path)
        file_name = os.path.join(model_folder_path, file_name)
        print(file_name)
        torch.save(self.state_dict(), file_name)
        
        if rename != "":
            os.rename(file_name, os.path.join(model_folder_path, rename))
            
    def load(self,filename = "model.pth" , mode_folder = "./model"):
        if os.path.isfile(mode_folder +"/"+ filename):
            print("Load...")
            self.load_state_dict(torch.load(mode_folder +"/"+ filename))
            self.eval()
            print(f"Loaddet {filename}")
            return True
        else:
            return False
        
    

class QTrainer:
    def __init__(self, model, lr, gamma):
        self.device = "cuda" if torch.cuda.is_available() else "cpu"
        self.lr = lr
        self.gamma = gamma
        self.model = model
        self.optimizer = optim.Adam(model.parameters(), lr=self.lr)
        self.criterion = nn.MSELoss()
        

    def train_step(self, state, action, reward, next_state, done):
        '''
        state = torch.tensor(state, dtype=torch.float)
        #print(f"State1 = {state}")
        next_state = torch.tensor(next_state, dtype=torch.float)
        action = torch.tensor(action, dtype=torch.float)
        reward = torch.tensor(reward, dtype=torch.float)
        # (n, x)

        if len(state.shape) == 1:
            # (1, x)
            state = torch.unsqueeze(state, 0)
            next_state = torch.unsqueeze(next_state, 0)
            action = torch.unsqueeze(action, 0)
            reward = torch.unsqueeze(reward, 0)
            done = (done, )
        '''
        '''
        # 1: predicted Q values with current state
        pred = self.model(state)

        target = pred.clone()
        #print(f"pred = {pred}")
        for idx in range(len(done)):
            Q_new = reward[idx]
            if not done[idx]:
                Q_new = reward[idx] + self.gamma * torch.max(self.model(next_state)) #[0][int(torch.argmax(action[0]).item())]))

            target[0][torch.argmax(action[0]).item()] = Q_new
            #print(f"Index = {idx}")
            #print(f"Q_New = {Q_new}")
            
        '''
        #My Try to understand
        state0 = torch.tensor(state, dtype=torch.float).to(device=self.device)
        pred = self.model(state0)
        
        target = pred.clone()
        Q_new = reward + self.gamma * torch.max(state0) #[0][int(torch.argmax(action[0]).item())]))
        action = torch.tensor(action, dtype=torch.float)
        #print(action)
        target[torch.argmax(action).item()] = Q_new
        
        #print(pred)
        #print(target)
        #time.sleep(100)
        
        #print(f"Pr to T = {pred}")
        #print(f"Target = {target}")
        # 2: Q_new = r + y * max(next_predicted Q value) -> only do this if not done
        # pred.clone()
        # preds[argmax(action)] = Q_new
        #self.optimizer.zero_grad()
        target = target.to(device=self.device)
        self.optimizer.zero_grad()
        loss = self.criterion(target, pred)
        #print(f"Loss = {loss}")
        loss.backward()
        self.optimizer.step()
        
        #pred = self.model(state)
        #loss = self.criterion(target, pred)
        loss = 1
        while loss > 30:
            #self.optimizer.zero_grad()
            loss = self.criterion(target, pred)
            #print(f"Loss = {loss}")
            loss.backward()
            self.optimizer.step()
            
            pred = self.model(state)
            int(loss) 

        #self.optimizer.step()
        
if __name__ == '__main__':
    import main