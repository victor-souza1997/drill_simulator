
import matplotlib.pyplot as plt
import numpy as np
file = "tunnel_loss.txt"
f=open(file,"r")
lines=f.readlines()
x1=[]
x2=[]
data1 = np.array([[]])
data2 = np.array([[]])
data3 = np.array([[]])
data4 = np.array([[]])

for x in lines:
    x1.append(x.split(' ')[0])
    x2.append(x.split(' ')[1])
    data1 = np.append(data1, float(x.split(' ')[0]))
    data2 = np.append(data2, float(x.split(' ')[1]))
    data3 = np.append(data3, float(x.split(' ')[2]))

    data4 = np.append(data4, float(x.split(' ')[3]))
f.close()

print(data1)


fig, (ax1, ax2, ax3) = plt.subplots(3)

ax1.plot(data1,data2)
ax1.set(xlabel="Profundidade", ylabel="Perda de Carga")

ax2.plot(data1, data3)
ax2.set(xlabel="Profundidade", ylabel="Vazão/velocidade")

ax3.plot(data1, data4)
ax3.set(xlabel="Profundidade", ylabel="Diâmetro")
plt.show()


