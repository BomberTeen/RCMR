from tkinter import *
from PIL import Image, ImageTk
import cv2
import imutils
import numpy as np
import socket

cap = None
rgb = 1
server_host = "192.168.1.50"
server_port = 33333

previo = None


# Función para visualizar el video en tiempo real
def visualizar():
    global pantalla, frame, rgb, ret
    if cap is not None:
        ret, frame = cap.read()

        if ret == True:
            if(rgb == 1):
                frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

                # Rendimensionamos el video
                frame_rgb = imutils.resize(frame_rgb, width=640)

                # Convertimos el video a formato adecuado para Tkinter
                im = Image.fromarray(frame_rgb)
                img = ImageTk.PhotoImage(image=im)

                # Mostramos en la interfaz gráfica
                lblVideo.configure(image=img)
                lblVideo.image = img
                lblVideo.after(10, visualizar)  # Actualizamos el video en tiempo real

        else:
            cap.release()

# Función de detección de movimiento
def motion():
    global pantalla, frame, rgb, ret, previo

    if cap is not None:
        ret, frame = cap.read()

        if ret == True:
            if(rgb == 1):
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

                # Si es el primer fotograma, guardar como referencia
                if previo is None:
                    previo = gray

                # Flujo óptico de Farneback para detectar movimiento
                flujo = cv2.calcOpticalFlowFarneback(previo, gray, None, 0.5, 3, 15, 3, 5, 1.2, 0)

                # Magnitud del flujo óptico
                mag, ang = cv2.cartToPolar(flujo[..., 0], flujo[..., 1])

                # Máscara binaria para detectar movimiento
                mascara = np.uint8(mag > 2) * 255  # Magnitud > 2

                # Encontrar contornos en la máscara
                contours, _ = cv2.findContours(mascara, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                # Dibujar los contornos sobre el video
                for contour in contours:
                    area = cv2.contourArea(contour)
                    if area > 100:  # Filtrar ruido
                        x, y, w, h = cv2.boundingRect(contour)
                        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

                # Convertir la imagen a formato adecuado para Tkinter (BGR -> RGB)
                frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                im = Image.fromarray(frame_rgb)
                img = ImageTk.PhotoImage(image=im)

                # Mostrar la imagen con detección de movimiento
                lblMotion.configure(image=img)
                lblMotion.image = img

                # Actualizar la imagen previa para el flujo óptico
                previo = gray.copy()

                lblMotion.after(10, motion)  # Actualizar la detección de movimiento

        else:
            cap.release()

# Función para iniciar la cámara
def iniciar():
    global cap
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    if not cap.isOpened():
        print("No se pudo abrir la cámara")
        return
    visualizar()
    print("Cámara iniciada")

# Conexión al servidor
def conexion():
    intentos = 0
    max_intentos = 1
    while intentos < max_intentos:
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.settimeout(1)
            client_socket.connect((server_host, server_port))
            print("Conexión establecida con el servidor")
            return client_socket
        except (ConnectionRefusedError, OSError):
            intentos += 1
            print(f"Intentando reconectar... ({intentos}/{max_intentos})")

    print("No se pudo establecer conexión después de varios intentos.")
    return None

# Enviar teclas al servidor
tecla_presionada = None
client_socket = conexion()

def enviar_tecla():
    global tecla_presionada
    if tecla_presionada:
        try:
            client_socket.sendall(tecla_presionada.encode())
            print(f"Tecla enviada: {tecla_presionada}")
        except Exception as e:
            print(f"Error al enviar la tecla: {e}")
        pantalla.after(1000, enviar_tecla)

def tecla_presionada_event(event):
    global tecla_presionada
    tecla_presionada = event.keysym  # Almacena la tecla presionada
    command = enviar_tecla(100)

def tecla_suelta_event(event):
    global tecla_presionada
    tecla_presionada = client_socket.sendall('p'.encode())

# Configuración de la ventana principal
pantalla = Tk()
pantalla.grid_rowconfigure(0, weight=1)
pantalla.grid_columnconfigure(0, weight=1)
main_frame = Frame(pantalla)
main_frame.grid_rowconfigure(0, weight=1)
main_frame.grid_columnconfigure(0, weight=1)
pantalla.title("Cloud Interface")
pantalla.geometry("1920x1080")

# Background
imagenF = Image.open("cloud.png")
imagenF = imagenF.resize((1920, 1080), Image.LANCZOS)
imagenF_tk = ImageTk.PhotoImage(imagenF)
background = Label(pantalla, image=imagenF_tk)
background.pack()
background.place(x=0, y=0, relwidth=1, relheight=1)

# Definición de botones
class Botones:
    def __init__(self, nombre_imagen, texto_boton, funcion, archivo_imagen, x, y, pantalla, altura, ancho):
        imagen = Image.open(archivo_imagen)
        imagen_tk = ImageTk.PhotoImage(imagen)
        boton = Button(pantalla, text=texto_boton, relief="solid", image=imagen_tk, height=altura, width=ancho, command=funcion)
        boton.image = imagen_tk
        boton.place(x=x, y=y)
        boton.grid(row=y, column=x)

boton_inicio = Botones("imagenBI", "Iniciar", iniciar, "inicio.png", 1920, 5, pantalla, 151, 391)
boton_controles = Botones("imagenC", "Controles", None, "controles.png", 1920, 481, pantalla, 329, 289)
boton_motion = Botones("imagenMT", "Tracker", motion, "MT.png", 1550, 200, pantalla, 151, 391)

# Etiquetas de texto
texto1 = Label(pantalla, text="Video en Tiempo Real")
texto1.place(x=280, y=10)
texto2 = Label(pantalla, text="Detección de Movimiento")
texto2.place(x=580, y=560)

# Etiquetas para mostrar el video y la detección de movimiento
lblVideo = Label(pantalla)
lblVideo.place(x=20, y=50)

lblMotion = Label(pantalla)
lblMotion.place(x=680, y=50)

pantalla.bind("<KeyPress>", tecla_presionada_event)
pantalla.bind("<KeyRelease>", tecla_suelta_event)
pantalla.mainloop()

print("Conectado al servidor.")
