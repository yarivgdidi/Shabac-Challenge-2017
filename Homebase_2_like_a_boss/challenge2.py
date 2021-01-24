import ctypes
def apply_operator(target, operation, parameter ):
    if operation == 0: #Xor
        result = target ^ parameter
    elif operation == 1: #ADD
        result = (target + parameter) & 255
    elif operation == 2: #Subtract
        result= (target - parameter ) & 255
    else:
        result = False
    return result

def print_message(message):
    for c in message:
       if c == 10:
           print(' ', end='')
       else:
           print(chr(c), end='')
    print();

def read_message():
    message =  open("./EncryptedMessage.bin", "rb")
    encripted=[]
    while (True):
        byte = message.read(1)
        if not byte:
            break
        encripted.append( int.from_bytes(byte, byteorder='little'))

    return encripted



def decript(message):
    key = open("./Key.bin", "rb")
    try:
        i=0
        direction = 1
        while (True):
           byte=key.read(1)
           if not byte:
               break
           operation =  int.from_bytes(byte, byteorder='little')
           operationParameter= int.from_bytes(key.read(1), byteorder='little')
           lengthToOperateOn= int.from_bytes(key.read(4),byteorder='little')
           print( operation,operationParameter, lengthToOperateOn)
           for n in range(lengthToOperateOn):
               message[i] = apply_operator(message[i],operation,operationParameter)
               if (direction == 1 and i==(len(message)-1) or direction == -1 and i == 0):
                   direction *= -1
               i += direction
    except Exception as e:
        print(repr(e))
    return message

message= read_message()
print(len(message))
print_message(message)
print_message(decript(message))