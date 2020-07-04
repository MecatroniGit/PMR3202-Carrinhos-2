from serial import Serial
from pynput import keyboard
from time import sleep
import sys

class Controle(keyboard.Listener):

    def __init__(self, port_name = 'COM11', baudrate = 11520):

        self.__valid_keys = [keyboard.Key.backspace, 'a' , 's', 'd', 'w', 'q', 'e', '0', ' ', 'l', 'z', 'r', 'n', 'm', 'h', '0', 'p']

        self.porta = Serial()
        self.porta.port     = port_name
        self.porta.baudrate = baudrate
        self.porta.timeout  = 0.01
    
        print("Abrindo porta {0} com baudrate {1}".format(port_name, baudrate))

        self.porta.open()

        print("Porta aberta com sucesso!")
        
        keyboard.Listener.__init__(self, on_press=self.on_press, on_release=self.on_release)

    def on_press(self, key):

        comm = None

        try:
            comm = key.char.casefold()
        except AttributeError:
            comm = " "
        
        if(comm in self.__valid_keys):
            for i in range(5):
                self.porta.write( str.encode(comm) )
          
    def on_release(self, key):
        
        if key == keyboard.Key.esc:
            # Stop listener
            return False 

        try:
            if(key.char in self.__valid_keys):
                for i in range(5):
                    self.porta.write( str.encode(" ") )
        except:
            pass

           

def main():

    # if len(sys.argv) > 2 or len(sys.argv) < 3:
    #     print("Parâmetros inválidos. Devem ser fornecidas a porta serial e a baudrate")wwdadasdwadsawdsaadaweqqeqedaswadswadadaqeqesdaswads
    #     return -1

    controle = Controle(*sys.argv[1:])

    controle.start()
    controle.join()

main()
