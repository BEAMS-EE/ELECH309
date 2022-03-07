
#include "FskDetector.h"
#ifdef DEBUG_FSK_DETECTOR
#include <stdio.h>
#endif

typedef enum {
	IDLE,
	START_BIT,
	DATA,
	PARITY,
	STOP_BIT,
	SENDING
} stateType;

typedef enum {
	SILENCE,
	BIT0,
	BIT1,
	BRUIT
} SignalStateType;

int fskDetector(int detLow, int detHigh) {
	static stateType state = IDLE;
	int messageComplete = 0;
	static int message=0;
	static int timer=0, countL=0, countH=0, bitNb=0, parity=0;
	SignalStateType signalState;

	// Détermination de l'état du signal
	if (!detHigh) {
		if (!detLow) {signalState = SILENCE;}
		else {signalState = BIT0;}
	} else {
		if (!detLow) {signalState = BIT1;}
		else {signalState = BRUIT;}
	}
	// Machine d'état
	switch(state) {
		case IDLE:
			// Actions : none
			// Transitions
			if (signalState == BIT0) {
				state = START_BIT;
				timer = 1;   countL = 1;   countH = 0;						// Initial actions of START_BIT
				bitNb = MESSAGE_LENGTH-1;   parity = 0;   message = 0;
			}
			break;
		case START_BIT:
			// Actions
			timer++;
			if (signalState == BIT0) {countL++;}
			if (signalState == BIT1) {countH++;}
			// Transitions
			if (timer >= OSR) {
				if (countL >= FSK_MIN_SAMPLES_NB) {
					state = DATA;
					timer = 0;   countL = 0;   countH = 0;					// Initial actions of DATA
				} else {
					state = IDLE;
					#ifdef DEBUG_FSK_DETECTOR
					printf("Start Bit error\n");
					#endif
				}
			}
			break;
		case DATA:
			// Actions
			timer++;
			if (signalState == BIT0) {countL++;}
			if (signalState == BIT1) {countH++;}
			// Transitions
			if (timer >= OSR) {
				if (countL >= FSK_MIN_SAMPLES_NB) {
					bitNb--;
					timer = 0;   countL = 0;   countH = 0;					// Initial actions of DATA or PARITY
				} else if (countH >= FSK_MIN_SAMPLES_NB) {
					message |= (1 << bitNb);
					bitNb--;
					parity++;
					timer = 0;   countL = 0;   countH = 0;					// Initial actions of DATA or PARITY
				} else {
					state = IDLE;
					#ifdef DEBUG_FSK_DETECTOR
					printf("Data Bit error\n");
					#endif
				}
				if (bitNb < 0) {
					state = PARITY;
				}
			}
			break;
		case PARITY:
			// Actions
			timer++;
			if (signalState == BIT0) {countL++;}
			if (signalState == BIT1) {countH++;}
			// Transitions
			if (timer >= OSR) {
				if (parity%2) {												// Message parity is odd
					if (countH >= FSK_MIN_SAMPLES_NB) {
						state = STOP_BIT;
						timer = 0;   countL = 0;   countH = 0;				// Initial actions of STOP_BIT
					} else {
						state = IDLE;
					#ifdef DEBUG_FSK_DETECTOR
					printf("Parity error\n");
					#endif
					}
				} else {
					if (countL >= FSK_MIN_SAMPLES_NB) {
						state = STOP_BIT;
						timer = 0;   countL = 0;   countH = 0;				// Initial actions of STOP_BIT
					} else {
						state = IDLE;
					#ifdef DEBUG_FSK_DETECTOR
					printf("Parity error\n");
					#endif
					}
				}
			}
			break;
		case STOP_BIT:
			// Actions
			timer++;
			if (signalState == BIT0) {countL++;}
			if (signalState == BIT1) {countH++;}
			// Transitions
			if (timer >= OSR) {
				if (countL >= FSK_MIN_SAMPLES_NB) {
					messageComplete = 1;
				}
				#ifdef DEBUG_FSK_DETECTOR
				else {
					printf("Stop Bit error\n");
				}
				#endif
				state = IDLE;
			}
			break;
		default:
			state = IDLE;
			break;
	}
	if (messageComplete) {
		messageComplete = 0;
		return(message);
	} else {
		return(0);
	}
}