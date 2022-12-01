import serial 
import sys
import os
import numpy
from midiutil import MIDIFile
mf = MIDIFile(1, eventtime_is_ticks=True)     # only 1 track
track = 0   # the only track
time = 0    # start at the beginning

os.system('ls /dev/ | grep "tty.usbserial"')
name = input('/dev/tty.usbserial-')
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
        while ser.in_waiting:
            print("connected: " + ser.portstr)
            data = ser.readline() 
            # print(data)        
            if data[0] == b'e'[0]:
                size = data[1]*256 + data[2]
                volume = data[3]
                tempo = data[4]
                print("size:",size)
                print("volume:",volume)
                print("tempo:",tempo)
                mf.addTrackName(track, 0, "Sample Track")
                mf.addTempo(track, 0, tempo)
                for i in range (size):
                    time = data[5+i*6]*256*256*256+data[i*6+6]*256*256+data[i*6+7]*256+data[i*6+8]
                    channel = data[i*6+9] & 15
                    pitch = data[i*6+10]
                    if(data[i*6+9] & 16):
                        note_on_time[pitch-21] = time
                    else:
                        print(track," ", channel," ", pitch," ", int(note_on_time[pitch-21])," ", int(time - note_on_time[pitch-21])," ", volume)
                        mf.addNote(track, channel, pitch, int(note_on_time[pitch-21]), int(time - note_on_time[pitch-21]), volume)

                           
except KeyboardInterrupt:
    ser.close()    # 清除序列通訊物件
    print('再見！')
with open("output.mid", 'wb') as outf:
    mf.writeFile(outf)