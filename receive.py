import serial
import sys
import os
import numpy
from midiutil import MIDIFile
import glob


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


def select_port():
    def is_usb_serial(s):
        return 'tty.usbserial' in s

    try:
        with open('ports-cache.txt', 'r') as cache:
            print('Loading ports from cache file...')
            ports = cache.read().strip().split('\n')
    except:
        print('Finding ports...')
        ports = [*filter(is_usb_serial, serial_ports())]
        with open('ports-cache.txt', 'w') as cache:
            print('Saving ports to cache file...')
            print(*ports, sep='\n', file=cache)

    if len(ports) == 0:
        print('No serial ports found... Make sure your connections aren\'t loose.')
        return ''
    elif len(ports) == 1:
        print('Found a single usb serial port:', ports[0])
        print('Automatically connecting to it...')
        return ports[0]
    else:
        port = ''
        while True:
            print('Found multiple usb serial port connections.')
            for i, p in enumerate(ports):
                print(f'{i+1}: {p}')
            ind = input(f'Please select a port (1-{len(ports)}): ')
            try:
                port = ports[int(ind)]
                return port
            except:
                print(f'\'{ind}\' is not one of the options.')


print("Welcome to midi file generator!")

default_output_file = 'sample'
BAUD_RATE = 115200
COM_PORT = select_port()

file_name = input(f'.mid output name (default: {default_output_file}): ')
if not file_name:
    file_name = default_output_file

# struct message {
#     uint32_t time_stamp;
#     uint8_t status_byte;
#     uint8_t data_byte;
# };


def main(ser):
    midi = MIDIFile(1, eventtime_is_ticks=True)     # only 1 track
    track = 0   # the only track
    time = 0    # start at the beginning

    note_on_time = numpy.zeros(88)

    print('Waiting for data...')

    while True:
        data = b''
        while ser.in_waiting:
            data += ser.readline()

        if not data:
            continue

        print(data)

        if data[0] == b'e'[0]:
            print(data)
            size = data[1]*256 + data[2]
            volume = data[4]
            tempo = data[5]
            print("size:", size)
            print("volume:", volume)
            print("tempo:", tempo)
            midi.addTrackName(track, 0, "Sample Track")
            midi.addTempo(track, 0, tempo)

            for i in range(size):
                # time = int.from_bytes(data[i*8+6 : i*8+10], 'big')
                if(data[i*8+6] == 0 and data[i*8+7] == 0 and data[i*8+8] == 0):
                    time = data[i*8+9]
                elif(data[i*8+6] == 0 and data[i*8+7] == 0):
                    time = data[i*8+8] + data[i*8+9]*256
                elif(data[i*8+6] == 0):
                    time = data[i*8+7] + data[i*8+8]*256 + data[i*8+9]*256*256
                else:
                    time = data[i*8+6] + data[i*8+7]*256 + \
                        data[i*8+8]*256*256 + data[i*8+9]*256*256*256

                status_byte = data[i*8+12]
                data_byte = data[i*8+13]

                channel = status_byte & 15
                pitch = data_byte
                if(status_byte & 16):
                    note_on_time[pitch-21] = time
                    print("on:", time)
                else:
                    print("off:", time)
                    print(track, " ", channel, " ", pitch, " ", int(
                        note_on_time[pitch-21]), " ", int(time - note_on_time[pitch-21]), " ", volume)
                    midi.addNote(track, channel, pitch, int(
                        note_on_time[pitch-21]), int(time - note_on_time[pitch-21]), volume)

        if data.endswith(b'\n'):
            return


print('port:', COM_PORT)
if __name__ == '__main__':
    ser = serial.Serial(
        port=COM_PORT,
        baudrate=BAUD_RATE,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS
    )

    with ser as s:
        midi = main(s)
        with open(file_name + ".mid", 'wb') as outf:
            print('Finished!')
            midi.writeFile(outf)
