import torch
import torch.nn as nn


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

File = "Seminar/cair/Henry/Python/"
model = NeuralNet(4, 5, 3)
#print(model.l1)
torch.save(model, File+'model.pth')
state = {"model": model.state_dict()}
torch.save(state, File+"state.json")

model.load_state_dict(torch.load(File+"state.json")["model"])
print(model)
for k, v in model.named_parameters():
    print(k, v)