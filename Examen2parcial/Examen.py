import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Configurar conexión serial
ser = serial.Serial('COM11', 9600)  # Reemplaza 'COM11' con tu puerto serial correspondiente

# Inicializar listas para almacenar datos
temperaturas = deque(maxlen=100)  # Almacena los últimos 100 datos
humedades = deque(maxlen=100)
tiempos = deque(maxlen=100)
contador_tiempo = 0

# Configurar gráfica
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
linea_temp, = ax1.plot([], [], lw=2, label='Temperatura (°C)')
linea_hum, = ax2.plot([], [], lw=2, label='Humedad (%)')
ax1.set_ylim(0, 40)  # Establecer límite del eje y para temperatura
ax2.set_ylim(0, 100)  # Establecer límite del eje y para humedad
ax1.set_title('Temperatura en función del tiempo')
ax1.set_xlabel('Tiempo (s)')
ax1.set_ylabel('Temperatura (°C)')
ax1.legend(loc='upper left')
ax2.set_title('Humedad en función del tiempo')
ax2.set_xlabel('Tiempo (s)')
ax2.set_ylabel('Humedad (%)')
ax2.legend(loc='upper left')

# Función para actualizar gráfica
def actualizar_grafica(frame):
    global contador_tiempo
    while ser.in_waiting:
        linea = ser.readline().decode().strip()
        datos = linea.split(',')

        if datos[0] == 'T':
            temperatura = float(datos[1])
            temperaturas.append(temperatura)
            if len(humedades) == 0:
                humedades.append(None)
            print(f"Tiempo: {contador_tiempo}s - Temperatura: {temperatura}°C")
        elif datos[0] == 'H':
            humedad = float(datos[1])
            humedades.append(humedad)
            if len(temperaturas) == 0:
                temperaturas.append(None)
            print(f"Tiempo: {contador_tiempo}s - Humedad: {humedad}%")

        tiempos.append(contador_tiempo)
        contador_tiempo += 1

    # Filtrar valores None para evitar problemas de graficado
    temp_data = [(t, temp) for t, temp in zip(tiempos, temperaturas) if temp is not None]
    hum_data = [(t, hum) for t, hum in zip(tiempos, humedades) if hum is not None]

    if temp_data:
        t_tiempos, t_temperaturas = zip(*temp_data)
        linea_temp.set_data(t_tiempos, t_temperaturas)
    if hum_data:
        h_tiempos, h_humedades = zip(*hum_data)
        linea_hum.set_data(h_tiempos, h_humedades)

    # Actualizar límites de las gráficas
    ax1.relim()
    ax1.autoscale_view(True, True, True)
    ax2.relim()
    ax2.autoscale_view(True, True, True)

    return linea_temp, linea_hum

# Iniciar animación
ani = animation.FuncAnimation(fig, actualizar_grafica, frames=None, interval=100, blit=True)
plt.tight_layout()
plt.show()
