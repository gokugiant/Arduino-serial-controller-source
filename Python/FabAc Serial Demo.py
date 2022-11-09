import PySimpleGUI as sg
import serial


def sendMessageSerialInit(msg,serialCom):
    serialCom.baudrate = 9600
    serialCom.write(msg.encode())
    answer = serialCom.readline()
    print(answer)

layout = [
    [sg.Text("Ampel-Lichter")],
    [sg.Button("RED LED"),sg.Button("YELLOW LED"),sg.Button("GREEN LED")],

    [sg.Text("Change the set up parameters")],
    [sg.Button("Change parameters")],
    [sg.Button("Close")],
]
window = sg.Window("Welcome to the photogrammetry scanner interface",layout)
ser = serial.Serial("/dev/tty.usbmodem142301",9600,timeout=5)
ser.setDTR(1)

while True:
    event, values = window.read()
    if event == "RED LED":
        sendMessageSerialInit("2",ser)
        print("Red light on changed")
    if event == "YELLOW LED":
        sendMessageSerialInit("1",ser)
        print("Red light on changed")
    if event == "GREEN LED":
        sendMessageSerialInit("0",ser)
        print("Red light on changed")
        
    if event == "Close" or event == sg.WIN_CLOSED:
        break
window.close()
