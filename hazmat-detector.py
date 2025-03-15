import cv2
from ultralytics import YOLO
import cvzone

cap = cv2.VideoCapture(1)  
#frame = cv2.imread(r"C:\\Users\\marth\\OneDrive\\Desktop\\venv\\media\\Screenshot 2025-03-07 203437.jpg")  # Para imágenes
clases_detectadas = set()


model = YOLO(r"C:\\Users\\marth\\OneDrive\\Desktop\\venv\\modelo2\\best.pt")

nombre = ["Blasting Agents", "Corrosive", "Oxidicer", "Oxigen","Poison", "Radioactive", "Spontaneously Combustible",
           "Dangerous when wet", "Explosives","Flammables gas", "Flammable solid","Fuel Oil", "Inhalation hazard",
           "Non flammable Gas","Organic Peoxide"]

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error al capturar la cámara")
        break

    #flipped = cv2.flip(frame,0)

    results = model(frame) 

    for info in results:
        for box in info.boxes:
            x1, y1, x2, y2 = box.xyxy[0].numpy().astype(int)
            confidence = round(box.conf[0].item() * 100, 2)
            classd = int(box.cls[0].item())

            
            name = nombre[classd] if classd < len(nombre) else f"Clase {classd}"

            if confidence > 70:
                cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 255), 3)
                cvzone.putTextRect(
                    frame, f"{name} ({classd}): {confidence}%", 
                    (x1 + 8, y1 - 12), 
                    thickness=2, scale=1.2, 
                    colorR=(255, 0, 255), colorT=(255, 255, 255)
                )




                if name not in clases_detectadas:
                    ruta_guardado = f"detecciones/{name}.jpg"
                    cv2.imwrite(ruta_guardado, frame)
                    print(f"Captura guardada: {ruta_guardado}")
                    clases_detectadas.add(name)

    #final =  cv2.flip(flipped, 0)
    cv2.imshow("Detección", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
