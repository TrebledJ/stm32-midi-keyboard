import serial 
import sys
import os
import numpy
from midiutil import MIDIFile


mf = MIDIFile(1, eventtime_is_ticks=True)     # only 1 track
track = 0   # the only track
time = 0    # start at the beginning

os.system('ls /dev/ | grep "tty.usbserial"')
print("Welcome to midi file generator, press ctrl+c to save the midi file!")
# name = input('/dev/tty.usbserial-')
name = '1410'
file_name = input('Midi output name:')
if not file_name:
    file_name = 'sample'
# COM_PORT = '/dev/tty.usbserial-14230'   
COM_PORT = '/dev/tty.usbserial-'+name  
BAUD_RATES = 115200    
note_on_time = numpy.zeros(88)
ser = serial.Serial(
    port=COM_PORT,
    baudrate=BAUD_RATES,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS

)  
    # struct message {
    #     uint32_t time_stamp;
    #     uint8_t status_byte;
    #     uint8_t data_byte;
    # };
try:
    while True:
        data = b''
        while ser.in_waiting:
            # print("connected: " + ser.portstr)
            data += ser.readline()

        if not data:
            continue

        # print("data length:",len(data)) 
        print(data)
        
        # if(len(data) >= 0):       
        if data[0] == b'e'[0]:
        # if(len(data) < 6):
            print(data)
            size = data[1]*256 + data[2]
            volume = data[4]
            tempo = data[5]
            print("size:",size)
            print("volume:",volume)
            print("tempo:",tempo)
            mf.addTrackName(track, 0, "Sample Track")
            mf.addTempo(track, 0, tempo)

            for i in range(size):
                # time = int.from_bytes(data[i*8+6 : i*8+10], 'big')
                if(data[i*8+6] == 0 and data[i*8+7] == 0 and data[i*8+8] == 0):
                    time = data[i*8+9]
                elif(data[i*8+6] == 0 and data[i*8+7] == 0):
                    time = data[i*8+8] + data[i*8+9]*256
                elif(data[i*8+6] == 0):
                    time = data[i*8+7] + data[i*8+8]*256 + data[i*8+9]*256*256
                else:
                    time = data[i*8+6] + data[i*8+7]*256 + data[i*8+8]*256*256+ data[i*8+9]*256*256*256

                status_byte = data[i*8+12]
                data_byte = data[i*8+13]

                channel = status_byte & 15
                pitch = data_byte
                if(status_byte & 16):
                    note_on_time[pitch-21] = time
                    print("on:",time)
                else:
                    print("off:",time)
                    print(track," ", channel," ", pitch," ", int(note_on_time[pitch-21])," ", int(time - note_on_time[pitch-21])," ", volume)
                    mf.addNote(track, channel, pitch, int(note_on_time[pitch-21]), int(time - note_on_time[pitch-21]), volume)
            
        if data.endswith(b'\n'):
            raise KeyboardInterrupt
            # else:
            #     print("Error: array out of bound, please try send again")

            

                           
except KeyboardInterrupt:
    ser.close()    # 清除序列通訊物件
with open(file_name+".mid", 'wb') as outf:
    print('Finished!')
    mf.writeFile(outf)