import numpy as np
from numpy import ma
import matplotlib.pyplot as plt
import matplotlib.patches as mp
from matplotlib.collections import PatchCollection

m_s = 4
m_e = 1
x_s, y_s = 0, 0
x_e, y_e = 2, 0
x_c, y_c = (m_s*x_s+m_e*x_e)/(m_s+m_e), (m_s*y_s+m_e*y_e)/(m_s+m_e)
gamma = 1.0

x = np.linspace(-2.5, 4, 1000)
y = np.linspace(-2.5, 2.5, 1000)
X, Y = np.meshgrid(x, y)

Omega = np.sqrt(gamma*((m_s+m_e)/(((x_s-x_e)**2+(y_s-y_e)**2)**(3/2))))

def f(X, Y):
    global x_s, y_s, m_s, x_e, y_e, m_e, x_c, y_c, Omega
    U = Omega**2*(X-x_c) - gamma*m_s*((X-x_s)/(((X-x_s)**2+(Y-y_s)**2)**(3/2))) \
        - gamma*m_e*((X-x_e)/(((X-x_e)**2+(Y-y_e)**2)**(3/2)))
    V = Omega**2*(Y-y_c) - gamma*m_s*((Y-y_s)/(((X-x_s)**2+(Y-y_s)**2)**(3/2))) \
        - gamma*m_e*((Y-y_e)/(((X-x_e)**2+(Y-y_e)**2)**(3/2)))
    return U, V

U, V = f(X, Y)

def findZero(i1, i2, f, I=50):
    rn = i1
    rp = i2
    for i in range(I):
        ra = np.array(rn) - np.array(f(*rn))*((np.array(rn)-np.array(rp))/(np.array(f(*rn))-np.array(f(*rp))))
        rp = rn
        rn = ra
    return rn

#U = gamma*( -(m_s/((X**2+Y**2)**(3/2)))*X - (m_e/(((X-d)**2+Y**2)**(3/2)))*(X-d) )
#V = gamma*( -(m_s/((X**2+Y**2)**(3/2)))*Y - (m_e/(((X-d)**2+Y**2)**(3/2)))*Y )

G_e = m_s/(((x_s-x_e)**2+(y_s+y_e)**2)**(3/2))
D = 0.1

A = np.sqrt(U**2+V**2)
#print(np.max(A))
sel = np.logical_and(A <= (G_e+D), A >= (G_e-D))
#print(np.sum(sel))

fig, ax = plt.subplots()

def limit(x, l=5):
    x[x > l] = l
    x[x < -l] = -l
    return x

def limit2d(x, y, l=5):
    m = np.hypot(x, y) > l
    x[m] = (x[m]/np.hypot(x[m], y[m]))*l
    y[m] = (y[m]/np.hypot(x[m], y[m]))*l
    return x, y

U, V = limit2d(U, V)
A = limit(A)

xmin, xmax, ymin, ymax = np.amin(x), np.amax(x), np.amin(y), np.amax(y)
extent = xmin, xmax, ymin, ymax

plt.imshow(5-A, cmap=plt.cm.jet, alpha=1, extent=extent)
#plt.colorbar()

plt.imshow(sel, cmap=plt.cm.jet, alpha=0.5, extent=extent)
#plt.colorbar()

stx = x.shape[0]/10
sty = y.shape[0]/10
Q = plt.quiver(X[::stx, ::sty], Y[::stx, ::sty], U[::stx, ::sty], V[::stx, ::sty],
               scale=100, width=.002,linewidth=1)

patches = []
colors = []
Sun = mp.Circle((x_s,y_s), 0.1, ec='none')
patches.append(Sun)
colors.append('#FFFF00')
Earth = mp.Circle((x_e,y_e), 0.1, ec='none')
patches.append(Earth)
colors.append('#0000FF')

p = [(0,0)] #make set
initial = [
    [[1, 1], [0.5, 0.5]],
    [[-1, -1], [-0.5, -0.5]],
]
for i1, i2 in initial:
    pn = findZero(np.array(i1), np.array(i2), f)
    print(pn)
    L = mp.Circle((pn[0], pn[1]), 0.1, ec='none')
    patches.append(L)
    colors.append('#FFFFFF')

collection = PatchCollection(patches, facecolors=colors)
ax.add_collection(collection)

plt.axis('off')
plt.savefig("lagrangePoints.png")
plt.axis('on')
plt.show()