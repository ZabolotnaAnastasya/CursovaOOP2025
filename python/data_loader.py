import os
import pandas as pd
import subprocess

class DataLoader:
    def __init__(self, default_path=""):
        self.raw_data = None
        self.file_path = default_path

    def select_file(self):
        print("Відкриваю системний діалог вибору файлу...")
        try:
            # AppleScript команда: choose file
            script = """
            try
                set theFile to choose file with prompt "Оберіть CSV файл з даними IMU:" of type {"csv"}
                POSIX path of theFile
            on error
                return ""
            end try
            """
            # запуск окремого процесу
            byte_output = subprocess.check_output(['osascript', '-e', script])
            path = byte_output.decode('utf-8').strip()

            if path:
                self.file_path = path
                return True
            return False
        except Exception as e:
            print(f"Помилка при виклику діалогу: {e}")
            return False

    def load(self):
        # парсить CSV файл у DataFrame
        if not self.file_path or not os.path.exists(self.file_path):
            if self.file_path:
                print(f"Файл не знайдено: {self.file_path}")
            return False

        try:
            data = pd.read_csv(self.file_path, header=None)

            # логіка визначення формату даних
            cols = len(data.columns)
            if cols == 8:
                # [time_a, ax, ay, az, time_g, gx, gy, gz]
                data.columns = ['time_a', 'ax', 'ay', 'az', 'time_g', 'gx', 'gy', 'gz']
                data['dt'] = 0.01
            elif cols == 7:
                # [dt, ax, ay, az, gx, gy, gz]
                data.columns = ['dt', 'ax', 'ay', 'az', 'gx', 'gy', 'gz']
            else:
                print(f"Невідомий формат файлу (колонок: {cols})")
                return False

            self.raw_data = data
            print(f"Дані успішно завантажено: {os.path.basename(self.file_path)}")
            return True
        except Exception as e:
            print(f"Критична помилка при читанні CSV: {e}")
            return False

    def get_data(self):
        return self.raw_data