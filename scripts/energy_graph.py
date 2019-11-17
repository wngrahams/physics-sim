import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import math

# energy.txt
energy_data_file = "energy.txt"

fig, ax = plt.subplots()

count = 0
kinetic_energy = []
potential_energy = []
total_energy = []

# loop through file and plot each line
with open(energy_data_file) as f:
    for i, line in enumerate(f):
        if i == 0:
            kinetic_energy = line.split(',')
            kinetic_energy = [float(point) for point in kinetic_energy]
        elif i == 1:
            potential_energy = line.split(',')
            potential_energy = [float(point) for point in potential_energy]
        elif i == 2:
            total_energy = line.split(',')
            total_energy = [float(point) for point in total_energy]
print("Number of kinetic points: " + str(len(kinetic_energy)))
print("Number of potential points: " + str(len(potential_energy)))
print("Number of total points: " + str(len(total_energy)))

print('\n')

print(kinetic_energy)

ax.plot(kinetic_energy, c='red')
ax.plot(potential_energy, c='blue')
ax.plot(total_energy, c='black')

# format and display plot
kin = mpatches.Patch(color='red', label='Kinetic Energy')
pot = mpatches.Patch(color='blue', label='Potential Energy')
tot = mpatches.Patch(color='black', label='Total Energy')

plt.legend(handles=[kin, pot, tot])
plt.xlabel('Time (0.001 s)')
plt.ylabel('Newtons')
plt.title('Energy Plot')
plt.show()
