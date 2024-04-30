import cv2
import numpy as np
import face_recognition
import os
import SerialModule as sm
import FirebaseModule as fbm
from datetime import datetime
from time import sleep
from scipy.spatial import distance

ser = sm.initConnection("COM5", 9600)

def findEncodings(path):

    classNames = []
    encodingList = []

    myList = os.listdir(path)
    for cl in myList:
        currentImg = cv2.imread(f'{path}/{cl}')

        classNames.append(os.path.splitext(cl)[0])
        currentImg = cv2.cvtColor(currentImg,cv2.COLOR_BGR2RGB)
        encode = face_recognition.face_encodings(currentImg)[0]
        encodingList.append(encode)
    print("Encoding Completed")

    return encodingList, classNames

def recognizeFaces(img, encodingList, classNames, scaleFactor=0.25):

    imgFaces = img.copy()
    imgS = cv2.resize(img,(0,0), None, scaleFactor, scaleFactor)
    imgS = cv2.cvtColor(imgS,cv2.COLOR_BGR2RGB)
    faceLocationCurrentImage = face_recognition.face_locations(imgS)
    encodeCurrentImage = face_recognition.face_encodings(imgS,faceLocationCurrentImage)

    names = []
    if not faceLocationCurrentImage:
        sm.sendData(ser,[1,1,0,0], 1)

    for encodingFace, faceLocation in zip(encodeCurrentImage, faceLocationCurrentImage):
        result = face_recognition.compare_faces(encodingList,encodingFace)
        faceDistance = face_recognition.face_distance(encodingList,encodingFace)
        #faceDistance = distance.euclidean(encodingList,encodingFace)
        matchIndex = np.argmin(faceDistance)

        if result[matchIndex]:
            name = classNames[matchIndex]
            color = (0,255,0)
            sm.sendData(ser,[0,0,1,0], 1)

        else:
            color = (0, 0, 255)
            name = 'unknown'
            sm.sendData(ser,[1,1,0,1], 1)

        names.append(name)
        y1,x2,y2,x1 = faceLocation
        y1,x2,y2,x1 = int(y1/scaleFactor), int(x2/scaleFactor), int(y2/scaleFactor), int(x1/scaleFactor)
        cv2.rectangle(imgFaces,(x1,y1),(x2,y2),color,2)
        cv2.putText(imgFaces, name, (x1+6, y1-6), cv2.FONT_HERSHEY_COMPLEX,1,color,2)

    return imgFaces, names

def markAttendance(name):

    with open('Attendance.csv','r+') as f:
        myDataList = f.readlines()
        nameList = []
        for line in myDataList:
            entry = line.split(',')
            nameList.append(entry[0])
        if name not in nameList:
            now = datetime.now()
            dateString = now.strftime('%H:%M:%S')
            f.writelines(f'{name},{dateString}\n')
            fbm.postData(name,dateString)

def main():

    encodingList, classNames = findEncodings("ImageAttendance")

    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    sm.sendData(ser,[1,1,0,0],1)

    while True:
        success, img = cap.read()
        imgFaces, names = recognizeFaces(img, encodingList, classNames)

        for name in names:
            if name == "unknown":
                sleep(0.2)
            else:
                markAttendance(name)

        cv2.imshow("Image", imgFaces)
        if cv2.waitKey(1) & 0xFF == ord("q"): break


main()