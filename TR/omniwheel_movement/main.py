import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.transforms as transforms
import math
import numpy as np

angle = math.pi / 6
radius = 3

wheel_centers = [[radius * math.cos(angle), radius * math.sin(angle)], [radius * math.cos(angle + (2 * math.pi) / 3), radius * math.sin(angle + (2 * math.pi) / 3)], [radius * math.cos(angle + (4 * math.pi) / 3), radius * math.sin(angle + (4 * math.pi) / 3)]]

fig, ax = plt.subplots()
ax.axis([-6, 6, -6, 6])
ax.set_aspect("equal")

triangle = patches.RegularPolygon((0, 0), numVertices=3, radius=3, color="red")
triangle.set_transform(transforms.Affine2D().rotate_deg(180) + ax.transData)

frontR = patches.Rectangle((wheel_centers[0][0] - 1, wheel_centers[0][1] - 0.5), 2, 1, color="blue")
frontR.set_transform(transforms.Affine2D().rotate_deg_around(wheel_centers[0][0], wheel_centers[0][1], 120) + ax.transData)

frontL = patches.Rectangle((wheel_centers[1][0] - 1, wheel_centers[1][1] - 0.5), 2, 1, color="blue")
frontL.set_transform(transforms.Affine2D().rotate_deg_around(wheel_centers[1][0], wheel_centers[1][1], 240) + ax.transData)

back = patches.Rectangle((wheel_centers[2][0] - 1, wheel_centers[2][1] - 0.5), 2, 1, color="blue")

ax.add_patch(triangle)
ax.add_patch(frontR)
ax.add_patch(frontL)
ax.add_patch(back)

direction = patches.Arrow(0, 0, 1 * radius, 0 * radius, width=0.8, color="black")
a = ax.add_patch(direction)

frontR_direction = patches.Arrow(wheel_centers[0][0], wheel_centers[0][1], wheel_centers[0][0], wheel_centers[0][1], width=0.8, color="black")
frontL_direction = patches.Arrow(wheel_centers[1][0], wheel_centers[1][1], wheel_centers[1][0], wheel_centers[1][1], width=0.8, color="black")
back_direction = patches.Arrow(wheel_centers[2][0], wheel_centers[2][1], wheel_centers[2][0], wheel_centers[2][1], width=0.8, color="black")

b = ax.add_patch(frontR_direction)
c = ax.add_patch(frontL_direction)
d = ax.add_patch(back_direction)

plt.draw()

current_angle = 0.0
direction = 0

while True:
    a.remove()
    b.remove()
    c.remove()
    d.remove()

    x = math.cos(math.radians(current_angle))
    y = math.sin(math.radians(current_angle))

    direction = patches.Arrow(0, 0, x * radius, y * radius, width=0.8, color="black")

    a = ax.add_patch(direction)

    frontL_rpm = x * 0.5 + y * math.sqrt(3) / 2
    frontR_rpm = x * 0.5 - y * math.sqrt(3) / 2
    back_rpm = x

    if (abs(frontL_rpm) > 1.0 or abs(frontR_rpm) > 1.0 or abs(back_rpm) > 1.0):
        frontL_rpm /= max(abs(frontL_rpm), abs(frontR_rpm), abs(back_rpm))
        frontR_rpm /= max(abs(frontL_rpm), abs(frontR_rpm), abs(back_rpm))
        back_rpm /= max(abs(frontL_rpm), abs(frontR_rpm), abs(back_rpm))

    frontL_direction = patches.Arrow(wheel_centers[1][0], wheel_centers[1][1], 0, frontL_rpm, width=0.8, color="black")
    frontL_direction.set_transform(transforms.Affine2D().rotate_deg_around(wheel_centers[1][0], wheel_centers[1][1], -30) + ax.transData)

    frontR_direction = patches.Arrow(wheel_centers[0][0], wheel_centers[0][1], 0, -frontR_rpm, width=0.8, color="black")
    frontR_direction.set_transform(transforms.Affine2D().rotate_deg_around(wheel_centers[0][0], wheel_centers[0][1], 30) + ax.transData)

    back_direction = patches.Arrow(wheel_centers[2][0], wheel_centers[2][1], 0, back_rpm, width=0.8, color="black")
    back_direction.set_transform(transforms.Affine2D().rotate_deg_around(wheel_centers[2][0], wheel_centers[2][1], -90) + ax.transData)

    b = ax.add_patch(frontR_direction)
    c = ax.add_patch(frontL_direction)
    d = ax.add_patch(back_direction)

    fig.canvas.draw_idle()
    plt.pause(0.01)

    if (current_angle + 1 <= 360):
        current_angle += 1.0
    else:
        current_angle = 0.0

plt.show()