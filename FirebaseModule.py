from firebase import firebase
import datetime

fb = firebase.FirebaseApplication('https://face-rec-dd032-default-rtdb.firebaseio.com',None)

def postData(name, time):
    data = {
        'Name': name,
        'Time': time
    }

    dateToday = datetime.date.today().strftime('%Y-%m-%d')
    fb.post(f'/{dateToday}',data)
