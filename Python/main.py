import serial
import PySimpleGUI as sg

log_entries=""

sg.theme('BluePurple')

layout = [[sg.Text('LOG:')],
          [sg.Text(log_entries, key='-logtext-')],
          [sg.Button('Start'), sg.Button('Exit')]]

log_window = sg.Window('Pattern 2B', layout, finalize=True)
       
ser = serial.Serial(
    port='/dev/tty.usbmodem142301',\
    baudrate=9600)
ser.flushInput()

print("connected to: " + ser.portstr)

def add_to_log(window, text, log_entries, text_colour='black'):
        log_entries = log_entries + "\n" + text
        window['-logtext-'].update(log_entries, text_color=text_colour)
        window.refresh()
        return log_entries

while True:
    try:
       ser_bytes = ser.readline()
       decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
       print(decoded_bytes)
    except:
        print("Keyboard Interrupt")
        ser.close()
        break


