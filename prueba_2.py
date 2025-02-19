from tkinter import *
import imultis
import cv2
from PIL import Image, ImageTk 
import numpy as np
#defino la ventana
pantalla = tk.Tk()
pantalla.title("Cloud Interface")#titulo
pantalla.config("1920x1080")#tamaño de la ventana

#Background
imagenF = photoimage(File = Fondo.png)
pantalla.config(bg = "Purple")
backbround = Lable(image = imagenF, text = "Fondo")
background.place(x = 0, y = 0, relwidth = 1, relheight = 1)

def finalizar():
    cap.release()
     cv2.DestroyAllWindows()
     print("Fin")

def iniciar():
global cap
#Elige la camara
cap = cv2.VideoCapture(2, cv2.Logitech_BRIO)
visualizar()
Print ("Camara Indicada")

#darle una funcion e imagen y posicion
imagenBI = PhotoImage(File = "inicio.png")
inicio = Button(pantalla, text = "iniciar", imagen = imagenBI, Height = "391", Width = "280", command = iniciar)
inicio.place(x = 100, y = 580)

#lo mismo pero con finaliazar
imagenBF = PhotoImage(File = "fin.png")
final = Button(pantalla, text = "fin", imagen = imagenBF, Height ="391", Width = "280", command = finalizar)
final.place(x = 980, y = 580)











#Interfaz
texto1 = Label(pantalla, Text = "Video En Tiempo Real")
texto1.place(x = 580, y = 10)
texto2 = Label(pantalla, Text = "Deteccion de movimiento")
texto2.place(x = 1010, y = 100)
