# -*- coding: utf-8 -*-

from scipy import signal
import math
import matplotlib.pyplot as plt
import numpy as np

version = 1
subversion = 0


def butterDesign(fo, fPass, fStop, gPass, gStop, fs):
    wp = [2*f/fs for f in fPass]
    ws = [2*f/fs for f in fStop]
    N, Wn = signal.buttord(wp, ws, gPass, gStop)
    b, a = signal.butter(N, Wn, 'band')
    sos = signal.butter(N, Wn, 'band', False, 'sos')
    return(b, a, sos)


def filterPlot(g, sos, fPass, fStop, gPass, gStop, fs, fo, H_max):
    fig, (ax1,ax2) = plt.subplots(2,1)
    ax2.set_title('Biquad stages transfer functions')
    ax2.set_xlabel('Frequency [Hz]')
    ax2.set_ylabel('Amplitude [dB]')
    ax2.grid(which='both', axis='both')
    ax2.axis([fo-100, fo+100, -30, 10])
    ax2.plot([fo-100, fo+100], np.ones(2)*20*np.log10(H_max), '--', label="Hmax")
    h_tot = np.ones(4096)
    for i in range(len(sos)):
        w, h = signal.freqz(g[i]*sos[i][0:3], sos[i][3:6], 4096)
        f = fs*w/(2*math.pi)
        ax2.plot(f[1:], 20*np.log10(abs(h[1:])), label="Stage %i" % (i+1))
        h_tot = h_tot*abs(h)
    ax2.legend()
    
    ax1.plot(f[1:], 20*np.log10(h_tot[1:]))
    ax1.set_title('Full bandpass filter transfer function and design constraints')
    ax1.set_xlabel('Frequency [Hz]')
    ax1.set_ylabel('Amplitude [dB]')
    ax1.grid(which='both', axis='both')
    ax1.fill([500, 500, fStop[0], fStop[0]], [-gStop, 20, 20, -gStop], '0.9', lw=0) # stop
    ax1.fill([fPass[0], fPass[0], fPass[1], fPass[1]], [-60, -gPass, -gPass, -60], '0.9', lw=0) # pass
    ax1.fill([fStop[1], fStop[1], 1500, 1500], [-gStop, 20, 20, -gStop], '0.9', lw=0) # stop
    ax1.axis([fo-100, fo+100, -30, 10])

    plt.show()



def params():
    success = False
    while not(success):
        try:
            fs = float(input("  1. Fréquence d'échantillonnage (Hz) : "))
            success = True
        except ValueError:
            print('    Veuillez entrer un nombre')

    success = False
    while not(success):
        try:
            fo = float(input("  2. Fréquence centrale du filtre (Hz) : "))
            if fo < fs/2:
                success = True
            else:
                print("    Vous ne respectez pas la fréquence de Nyquist !")
        except ValueError:
            print('    Veuillez entrer un nombre')

    success = False
    while not(success):
        try:
            dfPass = float(input("  3. Largeur de la bande passante (Hz) : "))
            success = True
        except ValueError:
            print('    Veuillez entrer un nombre')

    success = False
    while not(success):
        try:
            gPass = float(input("  4. Gain minimum dans la bande passante ]0,1[ : "))
            if 0 < gPass < 1:
                success = True
            else:
                print("    Le gain doit être dans l'intervalle ]0,1[ !")
        except ValueError:
            print('    Veuillez entrer un nombre')

    success = False
    while not(success):
        try:
            dfStop = float(input("  3. Largeur de la bande bloquante (Hz) : "))
            if dfStop > dfPass:
                success = True
            else:
                print("    La bande bloquante doit être plus large que la bande passante !")
        except ValueError:
            print('    Veuillez entrer un nombre')

    success = False
    while not(success):
        try:
            gStop = float(input("  4. Gain maximum dans la bande bloquante ]0,1[ : "))
            if 0 < gStop < 1:
                if gStop < gPass:
                    success = True
                else:
                    print('    Le gain dans la bande bloquante doit être inférieur à celui de la bande passante !')
            else:
                print("    Le gain doit être dans l'intervalle ]0,1[ !")
        except ValueError:
            print('    Veuillez entrer un nombre')

    return fs,fo,dfPass,dfStop,gPass,gStop


print('Design de filtre passe-bande numérique')
print('')
print('   Version %i.%i' % (version, subversion) )
print('')
print('--------------------------------------')
print('')
print('Entrez les paramètres du filtre :')

fs = 16000
fo = 1100
dfPass = 30
dfStop = 70
gPass = 0.9
gStop = 0.1
fs,fo,dfPass,dfStop,gPass,gStop = params()

fPass = [fo-dfPass/2, fo+dfPass/2]
fStop = [fo-dfStop/2, fo+dfStop/2]
gPass = -20*math.log10(gPass)
gStop = -20*math.log10(gStop)

b, a, sos = butterDesign(fo, fPass, fStop, gPass, gStop, fs)


n = len(sos)
g_tot = sos[0][0]
sos[0][0] = sos[0][0]/g_tot
sos[0][1] = sos[0][1]/g_tot
sos[0][2] = sos[0][2]/g_tot


print('')
print("Le filtre est découpé {} en étages d'ordre 2".format(n))
print("La fonction de transfert d'un étage est :")
print("                     -1       -2")
print("             1 + b1.z   + b2.z  ")
print("  H(z) = g . -------------------")
print("                     -1       -2")
print("             1 + a1.z   + a2.z  ")
print('')
print('Les coefficents des étages sont :')
g = np.ones(n)*np.power(g_tot, 1/n)
H_i_max = np.ones(n)
for i in range(n):
    w, h = signal.freqz(g[i]*sos[i][0:3], sos[i][3:6], 4096)
    H_i_max[i] = max(abs(h))
H_max = np.power(H_i_max.prod(), 1/n)
for i in range(n):
    g[i] = g[i]*H_max/H_i_max[i]
    print('  Etage {} :'.format(i+1))
    print('    g = {:.4}'.format(g[i]))
    print('    b = {}'.format(sos[i][0:3]))
    print('    a = {}'.format(sos[i][3:6]))
print('  Gain max des étages = {:.4f}'.format(H_max))
filterPlot(g, sos, fPass, fStop, gPass, gStop, fs, fo, H_max)
plt.show()

