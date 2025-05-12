import torch
import torch.nn as nn
import torch.nn.functional as F
import os
import json

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
        

File = "Seminar/cair/Henry/Python/"
File = "./Python/TicTacTowAI/model/"
model = Linear_QNet(9, 270, 9)
#print(model.l1)
#model = torch.load(File+'TicTacTow.pth')
#print(model.state_dict())
#state = {"model": model.state_dict()}
#torch.save(state, File+"state.json")
#model.load_state_dict(torch.load(File+"TicTacTow.pth"))
#model.eval()
model.load(filename="modelLast.pth", mode_folder=File)
#model.load_state_dict(torch.load(File+"TicTacTow.pth")["model"])
print(model)

Layers = []
i = 0
for k, v in model.named_parameters():
    if "weight" in k:
        Layer = {
            "ID": i,
            "IN": len(list(v[1])),
            "OUT": len(v),
            "weights":[ [float(d) for d in x] for x in list(v)

            ]
        }
        Layers.append(Layer)
        #JsonModel.append(f"'P{i}': {len(list(v[1]))}")
        #JsonModel.append(f"'P{i+1}': {len(v)}")
        i += 1

LayerSize = []
for x in Layers:
    LayerSize.append(x["IN"])
    LayerSize.append(x["OUT"])
print(LayerSize)
for i in LayerSize:
    if i == LayerSize[-1]: continue
    if LayerSize.index(i)%2: LayerSize.pop(LayerSize.index(i))
print(LayerSize)

JsonModel = {
    "LayerCount": len(Layers)+1,
    "LayerSizes": LayerSize,
    "Layers": Layers
}
File = "Seminar/cair/Henry/Python/TicTacTow"       
OUT = json.dumps({"model": JsonModel}, indent=4)
with open(File+'.json', 'w') as outfile:
    print("Save")
    outfile.write(OUT)
print(OUT)