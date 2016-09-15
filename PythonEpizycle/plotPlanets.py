
import matplotlib.pyplot as plt
plt.rcdefaults()

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.path as mpath
import matplotlib.lines as mlines
import matplotlib.patches as mpatches
from matplotlib.collections import PatchCollection

import matplotlib.animation as ani

import math

def get_circle_pos(t, a, w):
    return a*math.cos(w*t), a*math.sin(w*t)

def get_circle_vil(t, a, w):
    factor = 0.5
    return -factor*a*w*math.sin(w*t), factor*a*w*math.cos(w*t)


big_r = 3
big_w = 2*math.pi/8
big_data = big_r, big_w
def get_big_pos(t):
    return get_circle_pos(t, *big_data)

def get_big_vil(t):
    return get_circle_vil(t, *big_data)

small_r = 1
small_w = ((big_w**2*big_r**3)/(small_r**3))**0.5
small_data = small_r, small_w
def get_small_pos(t):
    return get_circle_pos(t, *small_data)

def get_small_vil(t):
    return get_circle_vil(t, *small_data)


t = 500

#fig = plt.figure(facecolor='r', frameon=False)
#ax = fig.add_axes([0, 0, 1, 1])
fig, ax = plt.subplots()
fig.patch.set_visible(False)

def rgb(r, g, b):
    return "#%02x%02x%02x" % (r, g, b)

def draw(t):
    patches = []
    colors = []
    ecolors = []

    print(get_big_pos(t))

    # 0
    xy_big = get_big_pos(t)
    planete_big = mpatches.Circle(xy_big, 0.3, ec="none")
    patches.append(planete_big)
    colors.append(rgb(192, 0, 0))
    ecolors.append(rgb(0, 0, 0))

    # 1
    big_arrow = mpatches.Arrow(*xy_big, *get_big_vil(t), width=0.3)
    patches.append(big_arrow)
    colors.append(rgb(255, 0, 0))
    ecolors.append(rgb(0, 0, 0))

    # 2
    xy_small = get_small_pos(t)
    planete_small = mpatches.Circle(xy_small, 0.1, ec="none")
    patches.append(planete_small)
    colors.append(rgb(0, 32, 96))
    ecolors.append(rgb(0, 0, 0))

    # 3
    dxdy_small = get_small_vil(t)
    small_arrow = mpatches.Arrow(*xy_small, *dxdy_small, width=0.3)
    patches.append(small_arrow)
    colors.append(rgb(0, 176, 240))
    ecolors.append(rgb(0, 0, 0))

    # 4
    small_translated = mpatches.Arrow(*xy_big, *dxdy_small, width=0.3)
    patches.append(small_translated)
    colors.append(rgb(0, 176, 240))
    ecolors.append(rgb(0, 0, 0))

    # 5
    diffrence = mpatches.Arrow(0,0,0,0)
    patches.append(diffrence)
    colors.append(rgb(0, 0, 0))
    ecolors.append(rgb(0, 0, 0))

    # 6
    poject_arrow = mpatches.Arrow(0,0,0,0)
    patches.append(poject_arrow)
    colors.append(rgb(0, 176, 80))
    ecolors.append(rgb(0, 0, 0))

    # 7
    sun = mpatches.Circle((0,0), 0.5, ec="none")
    patches.append(sun)
    colors.append(rgb(255, 255, 0))
    ecolors.append(rgb(0, 0, 0))

    # 8
    conection_line = mlines.Line2D([0,0], [0,0])
    patches.append(conection_line)
    colors.append(rgb(0, 0, 0))
    ecolors.append(rgb(0, 0, 0))

    # 9
    big_T = (2*math.pi)/big_w
    big_t = np.arange(0, big_T+0.1, 0.1)
    xy_big_o = [get_big_pos(t) for t in big_t]
    x_big_o = [e[0] for e in xy_big_o]
    y_big_o = [e[1] for e in xy_big_o]
    big_orbit = mlines.Line2D(x_big_o, y_big_o)
    patches.append(big_orbit)
    colors.append('None')
    ecolors.append(rgb(255, 0, 0))

    # 10
    small_T = (2*math.pi)/small_w
    small_t = np.arange(0, small_T+0.02, 0.02)
    xy_small_o = [get_small_pos(t) for t in small_t]
    x_small_o = [e[0] for e in xy_small_o]
    y_small_o = [e[1] for e in xy_small_o]
    small_orbit = mlines.Line2D(x_small_o, y_small_o)
    patches.append(small_orbit)
    colors.append('None')
    ecolors.append(rgb(0, 0, 255))

    return patches, colors, ecolors

def projection(a, b):
    abs_b = math.sqrt(sum([x*x for x in b]))
    unit_b = [x/abs_b for x in b]
    abs_p = sum([a*b for a, b in zip(unit_b, a)])
    vec_p = [abs_p*x for x in unit_b]
    return vec_p


def update_all(num, p, c):
    t = float(num/800.0)
    xy_big = get_big_pos(t)
    p[0].center = xy_big
    dxdy_big = get_big_vil(t)
    p[1] = mpatches.Arrow(*xy_big, *dxdy_big, width=0.5)  # green
    xy_small = get_small_pos(t)
    p[2].center = xy_small
    dxdy_small = get_small_vil(t)
    p[3] = mpatches.Arrow(*xy_small, *dxdy_small, width=0.5)  # gray
    xy_t = [x1+x2 for x1, x2 in zip(xy_big, dxdy_big)]
    p[4] = mpatches.Arrow(*xy_t, *dxdy_small, width=0.5)  # gray
    xy_d = [x1+x2 for x1, x2 in zip(xy_t, dxdy_small)]
    dxdy_d = [(a+b) for a, b in zip(dxdy_big, dxdy_small)]
    p[5] = mpatches.Arrow(*xy_big, *dxdy_d, width=0.5)  # black
    conectP = [b-s for b, s in zip(xy_big, xy_small)]
    normP = [conectP[1], -conectP[0]]
    vec_p = projection(dxdy_d, normP)
    p[6] = mpatches.Arrow(*xy_big, *vec_p, width=0.5)  # res
    p[8] = mlines.Line2D([xy_big[0]-10*conectP[0], xy_small[0]+10*conectP[0]],
                           [xy_big[1]-10*conectP[1], xy_small[1]+10*conectP[1]])

    c.set_paths(p)
    return p

patches, colors, ecolors = draw(500.0)

print(patches)
print(colors)

collection = PatchCollection(patches, facecolors=colors, edgecolors=ecolors)
ax.add_collection(collection)
#collection.set_animated(True)
#ax.add_line(line)
lim_all = 4.5
ax.set_xlim([-lim_all, lim_all])
ax.set_ylim([-lim_all, lim_all])
ax.set_aspect('equal')

ani_plot = ani.FuncAnimation(fig, update_all, 100000, fargs=(patches, collection), interval=1)

#plt.axis('off')
plt.show()

