import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import atexit

SERIAL_PORT = 'COM6'
BAUD_RATE = 115200

positions, setpoints, errors, time_points = [], [], [], []
max_points = 200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.5)
    ser.reset_input_buffer()
    print(f"Połączono z {SERIAL_PORT}")
except Exception as e:
    print(f"Błąd: {e}")
    exit()

fig, ax = plt.subplots()
line_pos, = ax.plot([], [], 'b-', lw=2, label='Pozycja [cm]')
line_set, = ax.plot([], [], 'r--', lw=2, label='Zadana [cm]')
line_err, = ax.plot([], [], 'g:', label='Uchyb [cm]')

ax.set_ylim(-15, 35)
ax.set_xlim(0, max_points)
ax.set_xlabel("Próbki")
ax.set_ylabel("Wartość [cm]")
ax.legend(loc='upper right')
ax.grid(True)

def update_plot(frame):
    try:
        # Szukaj znaku synchronizacji 'S'
        header = ser.readline().decode('utf-8').strip()
        
        if header == 'S':
            # Czytaj 3 kolejne linie
            p = float(ser.readline().decode('utf-8').strip())
            s = float(ser.readline().decode('utf-8').strip())
            e = float(ser.readline().decode('utf-8').strip())

            positions.append(p)
            setpoints.append(s)
            errors.append(e)
            time_points.append(len(time_points))

            if len(time_points) > max_points:
                ax.set_xlim(len(time_points) - max_points, len(time_points))

            line_pos.set_data(time_points, positions)
            line_set.set_data(time_points, setpoints)
            line_err.set_data(time_points, errors)
            
            print(f"Poz: {p:5.1f} | Ref: {s:5.1f} | Err: {e:5.1f}")

    except Exception:
        pass # Ignoruj błędy pojedynczych ramek

    return line_pos, line_set, line_err

atexit.register(lambda: ser.close())
ani = animation.FuncAnimation(fig, update_plot, interval=40, blit=True, cache_frame_data=False)
plt.show()