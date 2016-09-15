# import all libraries
import math  # basic mathematical functions sqrt, etc.
import numpy as np  # efficient array storage and manipulation
import matplotlib.pyplot as plt  # graphical output

# Define Initial conditions
mass = np.array([1.0, 0.1], dtype=np.float32)  # Masses of planets in the scene
x = np.array([
    [-1, 0, 0],  # First Planet Position
    [1, 0, 0],   # Second Planet Position
], dtype=np.float32)
dx = np.array([
    [0, -0.05, 0],  # Frist Planet Velocity
    [0, 0.5, 0],   # Second Planet Velocity
], dtype=np.float32)
planet_count = min([len(mass), len(x), len(dx)])  # Number of Planets in this scene

# Define all constants
gravitational_const = 1.0  # Gravitational Constant gamma
delta_time = 0.02  # The timesteps that will be used
time_steps_to_calculate = 5000  # The Number of time steps for witch a sol will be aproximpated

# Define all variables
time = 0 # The number of timesteps that passes since start
output_x = np.ndarray((time_steps_to_calculate, planet_count, 3),
            dtype=np.float32)  # Buffer for output position data
output_dx = np.ndarray((time_steps_to_calculate, planet_count, 3),
            dtype=np.float32)  # Buffer for output velocity data
dx_next = dx  # next velocity
x_next = x # next position

# Define constants following form other constants
time_int = np.arange(0, time_steps_to_calculate, 1, dtype=np.int32)
time_float = time_int.astype(np.float32)*delta_time

# define functions
def length(v):  # Find length of the vector v
    return math.sqrt(sum(map(lambda e: e**2, v)))

# Calculate position data
old_progress = 0
for t in range(time_steps_to_calculate):
    for m_c, x_c, dx_c, j in zip(mass, x, dx, range(planet_count)):
        gravitational_field = 0.0
        for x_k, m_k, k in zip(x, mass, range(planet_count)):
            if k == j: continue
            r = x_k - x_c
            gravitational_field += (m_k/length(r)**3)*r
        ddx = gravitational_const * gravitational_field
        dx_next[j] = dx_c + ddx*delta_time
        x_next[j] = x_c + dx_next[j]*delta_time
    dx = dx_next
    x = x_next
    output_dx[t, :, :] = dx
    output_x[t, :, :] = x
    #print("t = %s, x = %s" % (t, x))

    new_progress = int(t/time_steps_to_calculate*100 / 5)
    if(new_progress > old_progress):
        old_progress = new_progress
        print("progress: %s%%" % int(t/time_steps_to_calculate*100))

# Output the genrated data
x_component = 0; y_component = 1
for i in range(output_x.shape[1]):
    plt.plot(output_x[:, i, x_component], output_x[:, i, y_component])
plt.show()