import serial

def initConnection(portNo, baudRate):

    try:
        ser = serial.Serial(portNo, baudRate)
        print("Device Connected ")
        return ser
    except:
        print("Not Connected ")
        pass

def sendData(se, data, digits):

    myString = "$"
    for d in data:
        myString += str(int(d)).zfill(digits)
    try:
        se.write(myString.encode())
        print(myString)
    except:
        print("Data Transmission Failed ")


#:param portNo: COM port No e.g.
#       Win: 'COM3'
#       Linux: '/dev/ttyACM0'
#       mac: '/dev/cu.usbmodem14201'
#:param baudRate: BaudRate set in the Serial Device
#:return: Initialzed Serial Object
