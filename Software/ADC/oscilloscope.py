# -*- coding: cp1252 -*-
import sys                      # pour sys.exit
import serial                   # package de gestion du port serie
from matplotlib import pyplot   # pour l'affichage


# Configuration du port serie
serialPort = serial.Serial()    # creation de l'objet associe au port serie
serialPort.baudrate = 57600     # definition du baudrate
serialPort.port = 'COM3'        # selection du port serie
serialPort.timeout = 1        # temps d'attente avant d'abandonner une lecture

# Connexion au dsPIC
try:
    serialPort.open()               # on tente d'etablir la connexion
except:
    print('Echec de la connexion')  # En cas de probleme,
    sys.exit()                      # on ferme l'application

# Parametres de l'acquisition
sampleNb = 1000                     # Nombre d'echantillons a lire
samplePer = 200E-6                  # Periode d'echantillonnage

# Creation d'un vecteur pour stocker les donnees
data = []
time = []

# Recuperation des donnees
serialPort.write(b's')              # Envoi du caractere de synchro
for a in range(sampleNb):
    tmp1 = serialPort.read(1)       # on lit 1 octet sur le port serie
    tmp2 = ord(tmp1)                # on le transforme en entier
    data.append(3.3*tmp2/255)       # on calcule la tension correspondante, en V
    time.append(a*samplePer)

# Deconnexion du dsPIC
serialPort.close()

# Affichage des donnees
pyplot.plot(time, data)
pyplot.grid()
pyplot.xlabel('t (s)')
pyplot.ylabel('CH1 (V)')
pyplot.show()
