import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class Visualizer:
    def __init__(self, figure):
        self.fig = figure
        self.ax = self.fig.add_subplot(111, projection='3d')

        self.trajectory_line, = self.ax.plot([], [], [], linewidth=2, color='blue', label='Траєкторія')
        self.start_point = self.ax.scatter([], [], [], color='green', s=100, label='Старт')
        self.end_point = self.ax.scatter([], [], [], color='red', s=100, label='Фініш')

        self.setup_axes()

    def setup_axes(self):
        self.ax.set_title("Навігація (UKF)")
        self.ax.set_xlabel("X (м)")
        self.ax.set_ylabel("Y (м)")
        self.ax.set_zlabel("Z (м)")
        self.ax.legend()

    def update_plot(self, xs, ys, zs):
        if not xs: return

        self.trajectory_line.set_data(xs, ys)
        self.trajectory_line.set_3d_properties(zs)

        self.start_point._offsets3d = ([xs[0]], [ys[0]], [zs[0]])
        self.end_point._offsets3d = ([xs[-1]], [ys[-1]], [zs[-1]])

        # межі
        self.ax.set_xlim(min(xs), max(xs))
        self.ax.set_ylim(min(ys), max(ys))
        self.ax.set_zlim(min(zs), max(zs))

        # пропорції
        rx = max(xs) - min(xs)
        ry = max(ys) - min(ys)
        rz = max(zs) - min(zs)


        self.ax.set_box_aspect([rx, ry, rz])