import pygame
from serial import Serial
import sys

class Controle():

    def __init__(self, port_name = 'COM10', baudrate = 115200):
        
        self.__valid_keys = ['a' , 's', 'd', 'w', 'q', 'e', ' ']

        self.porta = Serial()
        self.porta.port     = port_name
        self.porta.baudrate = baudrate
        self.porta.timeout  = 0.01

        connect()

    def connect(self):

        print("Abrindo porta " + self.porta.port)

        self.porta.open()

        print("Porta aberta com sucesso!")

    def send(self, pack):

        for i in range(5):
            self.porta.write(pack)
        

    def listener(self):
        
        alive = True

        pygame.init()

        print("Init")

        clock = pygame.time.Clock()
        win = pygame.display.set_mode((640, 480), pygame.RESIZABLE)

        while alive:
            
            # print("HUe")
            for e in pygame.event.get():
                
                if e.type == pygame.QUIT:
                    alive = False

                if e.type == pygame.KEYDOWN:
                    # self.send(e.key)
                    print("{0} up!".format( bytes(e.key) ))

                if e.type == pygame.KEYUP:
                    self.send(e.key)
                    print("{0} down!".format( bytes(e.key) ))

            clock.tick(20)

        pygame.quit()   

