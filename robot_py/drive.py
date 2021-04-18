# Drive the robot by talking to Arduino via USB
import curses, serial

def menu(w):
    # loop around, handling input
    quit = 0
    w.addstr("Drive Instructions\n")
    w.addstr("F=8, B=5, L=4, R=6 [use number pad]\n")
    w.addstr("Accelerate=3, Brake=1\n")
    w.addstr("0=Stop\n")
    w.addstr("Any other key to stop and quit\n")

    while True:
        # set direction of robot
        c = w.getkey()
        
        if c == '8':
            direction = b'\x08'
        elif c == '5':
            direction = b'\x05'
        elif c == '4':
            direction = b'\x04'
        elif c == '6':
            direction = b'\x06'
        elif c == '0':
            direction = b'\x00'
        elif c == '1':
            direction = b'\x01'
        elif c == '3':
            direction = b'\x03'
        else:
            direction = 99

        ser.write(direction)
        ser.flush()
        
        if (direction == 99):
            break

# Serial port
ser_port = '/dev/ttyACM0'

ser = serial.Serial(ser_port, 9600, timeout=1)
ser.flush()

curses.wrapper(menu)

# tidy up
ser.write(0) # stop
ser.flush()
ser.close()
