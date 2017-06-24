import numpy as np
import math
def pos(theta, phi, r, O = np.zeros(shape=(1,3),dtype=np.float)):
    x = math.cos(theta) * math.cos(phi) * r
    y = math.sin(theta) * math.cos(phi) * r
    z = math.sin(phi) * r
    return np.array((x, y, z), dtype=np.float)

# print("%.10f %.10f %.10f"%pos(-math.pi/6, 85.0/180*math.pi, 6))
# print("%.10f %.10f %.10f"%pos(math.pi/2, 85.0/180*math.pi, 6))
C = (-2, 2, 0.5)
N = (0.6, 1, -0.6)
plane = -2
dis = (plane - C[2]) / N[2]
print("dis=%.3f\n"%dis)
C = np.array(C, dtype=np.float)
N = np.array(N, dtype=np.float)
O = C + dis * N
print("O=",O)
theta = math.atan2(-N[1], -N[0])
NC=pos(theta, 10.0/180*math.pi, 1)*dis+O
NN=-pos(theta, 10.0/180*math.pi, 1)
print("New N=", NN)
print("New camera=", NC)
print(O+N*dis/2)
# t=pos(math.pi/6/3, -1./180*math.pi, 1)
# print("%.10f %.10f %.10f"%t)
# dis=1.58
# s=(0-dis*t[0], 0-dis*t[1], 0.2-dis*t[2])
# print("%.10f %.10f %.10f"%s)
