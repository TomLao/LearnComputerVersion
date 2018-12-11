'''
使用截图处理好的人脸进行，脸部识别，并在摄像头显示识别结果
算法原理参考：https://docs.opencv.org/3.4.3/da/d60/tutorial_face_main.html
'''

import numpy as np
import cv2

class MyFaceRecognize(object):

    def __init__(self):
        self.face_cascade = cv2.CascadeClassifier(
            '/home/heze/下载/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml')
        self.eye_cascade = cv2.CascadeClassifier('/home/heze/下载/opencv-master/data/haarcascades/haarcascade_eye.xml')
        self.recognizer = cv2.face.LBPHFaceRecognizer_create()

    def read(self, base_path, names):
        faces = []
        IDs = []
        for idx, name in enumerate(names):
            for j in range(50):
                path = base_path + name + '/' + str(j) + '.jpg'
                src = cv2.cvtColor(cv2.imread(path), cv2.COLOR_BGR2GRAY)
                faces.append(np.array(src, 'uint8'))
                IDs.append(idx)
        return faces, IDs

    def train(self, base_path, names):
        faces, IDs = self.read(base_path, names)
        self.recognizer.train(faces, np.array(IDs))

    def predict(self, names):
        capture = cv2.VideoCapture(0)

        while(1):
            ret, frame = capture.read()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = self.face_cascade.detectMultiScale(gray, 1.3, 5)
            for (x, y, w, h) in faces:
                cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)
                roi_gray = gray[y:y+h, x:x+w]
                cv2.resize(roi_gray, (450, 450))

                face_id = self.recognizer.predict(np.array(roi_gray, 'uint8'))
                # print(names[face_id[0]])

                cv2.putText(frame, names[face_id[0]], (x+10, y-10), cv2.FONT_HERSHEY_SIMPLEX, 1,(0,255,0),2,cv2.LINE_AA)

            cv2.imshow('FaceRecognize', frame)
            #按q退出
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break


if __name__ == '__main__':

    names = ['myface', 'zzh', 'xrw']
    base_path = 'images/'

    myFaceRecognize = MyFaceRecognize()
    myFaceRecognize.train(base_path, names)
    myFaceRecognize.predict(names)
