import cv2
from datetime import datetime
import FaceRecognitionModule as frm 
import FirebaseModule as fbm 

from time import sleep

cap = cv2.VideoCapture(0)
encodeList, classNames = frm.findEncodings("ImageAttendance")

def markAttendance(name):
    #myLed.color('green')

    with open('Attendance.csv','r+') as f:
        myDataList = f.readlines()
        nameList = []
        for line in myDataList:
            entry = line.split(',')
            nameList.append(entry[0])
        if name not in nameList:
            now = datetime.now()
            dtString = now.strftime('%H:%M:%S')
            f.writelines(f'{name},{dtString}\n')
            fbm.postData(name,dtString)

while True:
    success, img = cap.read()
    imgFaces, names = frm.recognizeFaces(img, encodeList, classNames,0.2)
    for name in names:
        if name == "unknown":
            sleep(0.2)
        else:
            markAttendance(name)

    cv2.imshow("Image",imgFaces)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break