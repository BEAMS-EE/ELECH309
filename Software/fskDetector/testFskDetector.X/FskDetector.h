
#define DEBUG_FSK_DETECTOR

#define MESSAGE_LENGTH		10
#define SAMPLING_FREQ		200
#define BIT_FREQ			10
#define OSR					(SAMPLING_FREQ/BIT_FREQ)
#define FSK_MIN_SAMPLES_NB	OSR	//(3*OSF/4)

int FskDetector(int detLow, int detHigh);
