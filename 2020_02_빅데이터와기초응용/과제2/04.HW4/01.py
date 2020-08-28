# import torch
# import torchvision
# import torchtext
# import numpy
# import matplotlib
# import sklearn


# print("torch", torch.__version__)
# print("torchtext", torchtext.__version__)
# print("numpy", numpy.__version__)
# print("matplotlib", matplotlib.__version__)
# print("sklearn", sklearn.__version__)
# print("torchvision", torchvision.__version__)
# print("torch", torch.__version__)


s = "04:00->37.5720164-127.0050075-0.0T4-0.019-0.029-0.8-95.0-69.0"
s1 = s.split("->")
print(s1[0], s1[1])
sL = s1[1].split("-")

print(sL[2][0:7])
for i in sL:
    T =+ float(i)
    
avg=T/len(sL)
print(avg)
print(sL[2])
v = float(sL[2])
print(v)

