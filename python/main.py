import sys
import os
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.widgets import Slider

# імпорт с++ модуля
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(current_dir)

try:
    import navigator_module
    print("C++ модуль успішно завантажено!")
except ImportError as e:
    print(f"\nПОМИЛКА: Не вдалося завантажити 'navigator_module'.")
    sys.exit(1)

raw_data = None
trajectory_line = None
start_point = None
end_point = None
ax_3d = None

def load_data():
    global raw_data
    data_dir = "адреса"
    file_name = "1081_N_1.csv"
    csv_file_path = os.path.join(data_dir, file_name)

    if not os.path.exists(csv_file_path):
        print(f" Файл не знайдено: {csv_file_path}")
        return False

    try:
        data = pd.read_csv(csv_file_path, header=None)
        if len(data.columns) == 8:
            data.columns = ['time_a', 'ax', 'ay', 'az', 'time_g', 'gx', 'gy', 'gz']
            data['dt'] = 0.01
        elif len(data.columns) == 7:
            data.columns = ['dt', 'ax', 'ay', 'az', 'gx', 'gy', 'gz']
        else:
            print("Невідомий формат файлу")
            return False

        raw_data = data
        print(f"Файл завантажено: {file_name}")
        return True
    except Exception as e:
        print(f"Помилка: {e}")
        return False

def main():
    global ax_3d, trajectory_line, start_point, end_point

    if not load_data(): return

    # графік
    fig = plt.figure(figsize=(12, 11))

    plt.subplots_adjust(bottom=0.45)

    ax_3d = fig.add_subplot(111, projection='3d')
    ax_3d.set_title(f"Інтерактивна навігація (UKF)")
    ax_3d.set_xlabel("X (м)")
    ax_3d.set_ylabel("Y (м)")
    ax_3d.set_zlabel("Z (м)")

    trajectory_line, = ax_3d.plot([], [], [], linewidth=2, color='blue', label='Траєкторія')
    start_point = ax_3d.scatter([], [], [], color='green', s=100, label='Старт')
    end_point = ax_3d.scatter([], [], [], color='red', s=100, label='Фініш')
    ax_3d.legend()

    # ZUPT
    ax_win = plt.axes([0.25, 0.35, 0.50, 0.03])
    slider_window = Slider(
        ax=ax_win, label='ZUPT Window',
        valmin=5, valmax=50, valinit=25, valstep=1
    )

    ax_thresh = plt.axes([0.25, 0.31, 0.50, 0.03])
    slider_thresh = Slider(
        ax=ax_thresh, label='Acc Threshold',
        valmin=0.01, valmax=1.0, valinit=0.3
    )

    # Акселерометр
    ax_ap = plt.axes([0.25, 0.25, 0.50, 0.03])
    slider_acc_p = Slider(
        ax=ax_ap, label='Acc Process (Q)',
        valmin=0.001, valmax=1.0, valinit=0.1
    )

    ax_am = plt.axes([0.25, 0.21, 0.50, 0.03])
    slider_acc_m = Slider(
        ax=ax_am, label='Acc Measure (R)',
        valmin=0.01, valmax=2.0, valinit=0.5
    )

    # Гіроскоп
    ax_gp = plt.axes([0.25, 0.15, 0.50, 0.03])
    slider_gyro_p = Slider(
        ax=ax_gp, label='Gyro Process (Q)',
        valmin=0.0001, valmax=0.1, valinit=0.01
    )

    ax_gm = plt.axes([0.25, 0.11, 0.50, 0.03])
    slider_gyro_m = Slider(
        ax=ax_gm, label='Gyro Measure (R)',
        valmin=0.001, valmax=1.0, valinit=0.1
    )


    def update(val):
        w_size = int(slider_window.val)
        a_thresh = slider_thresh.val

        acc_p = slider_acc_p.val
        acc_m = slider_acc_m.val
        gyro_p = slider_gyro_p.val
        gyro_m = slider_gyro_m.val

        try:
            nav = navigator_module.Navigation(
                window_size=w_size,
                acc_thresh=a_thresh,
                acc_p_noise=acc_p,
                acc_m_noise=acc_m,
                gyro_p_noise=gyro_p,
                gyro_m_noise=gyro_m
            )
        except TypeError:
            print("ПОМИЛКА: C++ модуль очікує іншу кількість аргументів.")
            return

        for _, row in raw_data.iterrows():
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

        trajectory_line.set_data(xs, ys)
        trajectory_line.set_3d_properties(zs)

        if len(xs) > 0:
            start_point._offsets3d = ([xs[0]], [ys[0]], [zs[0]])
            end_point._offsets3d = ([xs[-1]], [ys[-1]], [zs[-1]])

            # масштабування осей
            ax_3d.set_xlim(min(xs), max(xs)); ax_3d.set_ylim(min(ys), max(ys)); ax_3d.set_zlim(min(zs), max(zs))


        fig.canvas.draw_idle()


    slider_window.on_changed(update)
    slider_thresh.on_changed(update)
    slider_acc_p.on_changed(update)
    slider_acc_m.on_changed(update)
    slider_gyro_p.on_changed(update)
    slider_gyro_m.on_changed(update)

    update(None)
    plt.show()

if __name__ == "__main__":
    main()