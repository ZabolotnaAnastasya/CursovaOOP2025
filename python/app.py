import sys
import os
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

from data_loader import DataLoader
from visualizer import Visualizer

current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(current_dir)

try:
    import navigator_module
except ImportError:
    print("ПОМИЛКА: 'navigator_module' не знайдено.")
    sys.exit(1)

class NavigationApp:
    def __init__(self):
        self.loader = DataLoader()

        self.fig = plt.figure(figsize=(12, 13))
        plt.subplots_adjust(bottom=0.50)

        self.viz = Visualizer(self.fig)
        self.sliders = {}

    def init_sliders(self):
        # (key, label, min, max, init, step, [x, y, w, h])
        configs = [
            # ZUPT
            ('window', 'ZUPT Window', 5, 50, 25, 1, [0.25, 0.44, 0.50, 0.02]),
            ('thresh', 'Acc Threshold', 0.01, 1.0, 0.3, 0.01, [0.25, 0.40, 0.50, 0.02]),

            # акселерометр
            ('acc_p', 'Acc Process (Q)', 0.001, 1.0, 0.1, 0.001, [0.25, 0.34, 0.50, 0.02]),
            ('acc_m', 'Acc Measure (R)', 0.01, 2.0, 0.5, 0.01, [0.25, 0.30, 0.50, 0.02]),

            # гіроскоп
            ('gyro_p', 'Gyro Process (Q)', 0.0001, 0.1, 0.01, 0.0001, [0.25, 0.24, 0.50, 0.02]),
            ('gyro_m', 'Gyro Measure (R)', 0.001, 1.0, 0.1, 0.001, [0.25, 0.20, 0.50, 0.02]),

            # початкова швидкість
            ('vx', 'Init Vel X', -2.0, 2.0, 0.0, 0.01, [0.25, 0.14, 0.50, 0.02]),
            ('vy', 'Init Vel Y', -2.0, 2.0, 0.0, 0.01, [0.25, 0.10, 0.50, 0.02]),
            ('vz', 'Init Vel Z', -2.0, 2.0, 0.0, 0.01, [0.25, 0.06, 0.50, 0.02])
        ]

        for key, label, vmin, vmax, vinit, step, pos in configs:
            ax = plt.axes(pos)
            self.sliders[key] = Slider(ax, label, vmin, vmax, valinit=vinit, valstep=step)
            self.sliders[key].on_changed(self.on_update)

    def on_update(self, val=None):
        if self.loader.get_data() is None: return

        try:
            nav = navigator_module.Navigation(
                window_size=int(self.sliders['window'].val),
                acc_thresh=self.sliders['thresh'].val,
                acc_p_noise=self.sliders['acc_p'].val,
                acc_m_noise=self.sliders['acc_m'].val,
                gyro_p_noise=self.sliders['gyro_p'].val,
                gyro_m_noise=self.sliders['gyro_m'].val,
                v_x0=self.sliders['vx'].val,
                v_y0=self.sliders['vy'].val,
                v_z0=self.sliders['vz'].val
            )
        except TypeError:
            print("ПОМИЛКА: Не співпадають аргументи C++!")
            return

        df = self.loader.get_data()
        for _, row in df.iterrows():
            nav.process_reading(
                float(row['ax']), float(row['ay']), float(row['az']),
                float(row['gx']), float(row['gy']), float(row['gz']),
                float(row['dt'])
            )

        vecs = nav.get_trajectory()
        if not vecs: return

        xs = [v.x for v in vecs]
        ys = [v.y for v in vecs]
        zs = [v.z for v in vecs]

        self.viz.update_plot(xs, ys, zs)
        self.fig.canvas.draw_idle()

    def run(self):
        print("Запуск...")
        if self.loader.select_file():
            if self.loader.load():
                self.init_sliders()
                self.on_update()
                plt.show()
        else:
            print("Файл не обрано")