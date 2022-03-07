
#define DEBUG_FSK_DETECTOR

#define MESSAGE_LENGTH      10                          // nombre de bits de donnees de la trame
#define SAMPLING_FREQ       200                         // frequence d'échantillonnage du signal audio, en Hz
#define BIT_FREQ            10                          // frequence des bits de la trame, en Hz
#define OSR                 (SAMPLING_FREQ/BIT_FREQ)    // OverSampling Ratio
#define FSK_MIN_SAMPLES_NB  (3*OSR/4)                   // nombre d'echantillons minimum pour considerer un bit valide

int fskDetector(int detLow, int detHigh);
