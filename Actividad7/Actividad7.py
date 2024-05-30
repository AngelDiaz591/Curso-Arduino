import serial
import tkinter as tk
from collections import deque
import threading
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Configuración del puerto serial
serialPort = 'COM7'
baudRate = 9600

try:
    # Intenta establecer una conexión con el puerto serie
    serialConnection = serial.Serial(serialPort, baudRate)
    print('Connected to ' + serialConnection.portstr)
except:
    # En caso de fallo en la conexión, muestra un mensaje de error
    print('Failed to connect to ' + serialPort)
    exit() # Salir del programa si no se puede establecer la conexión

# Función para enviar comandos al Arduino para encender o apagar el LED
def toggle_led(command):
    serialConnection.write(command.encode())

# Configuración de la ventana de Tkinter
root = tk.Tk()
root.title("Real Time Serial Data")

# Tamaño máximo de la cola de datos
max_data_length = 100

# Cola para almacenar los datos
data = deque(maxlen=max_data_length)

# Crear una figura de matplotlib
fig = plt.Figure(figsize=(5, 4), dpi=100)
ax = fig.add_subplot(111)

# Añadir la figura de matplotlib a la ventana de tkinter
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.draw()
canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

# Crear un cuadro de texto para mostrar el último voltaje leído
voltage_text = tk.StringVar()
voltage_label = tk.Label(root, textvariable=voltage_text)
voltage_label.pack(side=tk.RIGHT)

# Crear un nuevo Label para mostrar el número de voltaje en la esquina superior derecha
last_voltage_label = tk.Label(root, text="Último voltaje: N/A")
last_voltage_label.pack(side=tk.TOP, anchor="ne")

# Función para graficar los datos
def plot_data():
    ax.clear() # Borrar la gráfica anterior
    ax.plot(data, color='blue') # Graficar los datos
    ax.set_title('Real Time Serial Voltage Data')
    ax.set_xlabel('Time')
    ax.set_ylabel('Voltage (V)')
    canvas.draw() # Dibujar la gráfica
    root.after(100, plot_data) # Actualizar la gráfica cada 100 ms

# Función para leer los datos del puerto serial
def read_serial():
    while serialConnection.isOpen():
        try:
            voltage = float(serialConnection.readline().strip())
            data.append(voltage)
            voltage_text.set(f"Último voltaje: {voltage} V") # Actualizar el cuadro de texto
            last_voltage_label.config(text=f"Último voltaje: {voltage} V") # Actualizar el Label de último voltaje
            print("Voltaje recibido del Arduino:", voltage)
        except ValueError:
            print("Error: No se pudo convertir el valor recibido a flotante.")
        except Exception as e:
            print("Error:", e)
            break

# Función para encender el LED cuando se presiona el botón correspondiente
def turn_on_led():
    toggle_led('1\n')

# Función para apagar el LED cuando se presiona el botón correspondiente
def turn_off_led():
    toggle_led('0\n')

# Botón para encender el LED
turn_on_button = tk.Button(root, text="Encender LED", command=turn_on_led)
turn_on_button.pack(side=tk.LEFT)

# Botón para apagar el LED
turn_off_button = tk.Button(root, text="Apagar LED", command=turn_off_led)
turn_off_button.pack(side=tk.LEFT)

# Iniciar el hilo para leer los datos del puerto serial
serial_thread = threading.Thread(target=read_serial)
serial_thread.start()

# Iniciar la gráfica de datos
plot_data()

# Iniciar el bucle principal de Tkinter
root.mainloop()

# Cierra la conexión con el puerto serie
serialConnection.close()