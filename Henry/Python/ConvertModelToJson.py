import torch
import torch.nn as nn
import json

class NeuralNet(nn.Module):
    def __init__(self, input_size, hidden_size, num_classes):
        super(NeuralNet, self).__init__()
        mitt = int(((input_size + num_classes)/2))
        self.l1 = nn.Linear(input_size, hidden_size) 
        self.l2 = nn.Linear(hidden_size, hidden_size) 
        self.l3 = nn.Linear(hidden_size, num_classes)
        self.relu = nn.ReLU()
    
    def forward(self, x):
        out = self.l1(x)
        out = self.relu(out)
        out = self.l2(out)
        out = self.relu(out)
        out = self.l3(out)
        out = self.relu(out)
        # no activation and no softmax at the end
        return out

File = "Seminar/cair/Henry/Python/TicTacTow.pth"

model = torch.load(File)

state = {"model": model.state_dict()}
#torch.save(state, File+"state.json")

#model.load_state_dict(torch.load(File+"state.json")["model"])
print(state)
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
OUT = json.dumps({"model": JsonModel}, indent=4)
with open(File+'.json', 'w') as outfile:
    print("Save")
    outfile.write(OUT)
print(OUT)