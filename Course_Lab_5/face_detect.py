import cv2
import numpy as np


face_cascade = cv2.CascadeClassifier('/home/heze/下载/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('/home/heze/下载/opencv-master/data/haarcascades/haarcascade_eye.xml')
smile_cascade = cv2.CascadeClassifier('haarcascade_smile.xml')

def detect_video():
    cap = cv2.VideoCapture(0)
    while(1):
        ret, frame = cap.read()
        #按q退出
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break;

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)
        for (x,y,w,h) in faces:
            cv2.rectangle(frame ,(x,y), (x+w,y+h), (0, 255, 0), 2)
            roi_gray = gray[y:y+h, x:x+w]
            roi_color = frame[y:y+h, x:x+w]
            eyes = eye_cascade.detectMultiScale(roi_gray)
            for (ex,ey,ew,eh) in eyes:
                cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(255, 0,0),2)
        cv2.imshow('detect_video', frame)

    cap.release()
    cv2.destroyAllWindows()

def detect_picture(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    for (x, y, w, h) in faces:
        cv2.rectangle(image, (x, y), (x+w, y+h), (0, 255, 0), 2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = image[y:y+h, x:x+h]
        eyes = eye_cascade.detectMultiScale(roi_gray)
        for (ex, ey, ew, eh) in eyes:
            cv2.rectangle(roi_color, (ex, ey), (ex+ew, ey+eh), (255 ,0, 0), 2)
    cv2.imshow('image', image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
        
if __name__ == "__main__":
    detect_video()
    #detect_picture(cv2.imread('people.jpg'))
    #detect_picture(cv2.imread('linus.jpg'))
