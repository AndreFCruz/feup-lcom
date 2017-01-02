#include "Serial.h"
#include "Communication.h"

// Current state of communication
static serial_state_t comState = NONE;

void serial_handler() {
	unsigned char received = serial_read();

	switch (comState) {
	case WAITING_START:
		if ( MP_WAITING == received ) {
			serial_write(MP_STARTED);
			comState = ONGOING;
		} else if ( MP_STARTED == received ) {
			serial_write(ACK);
			comState = ONGOING;
		}
		break;
	case ONGOING:
		if ( MP_ENDED == received ) {
			//serial_write(ACK);
			comState = ENDED;
		}
		break;
	case ENDED:
		break;
	case NONE:
		break;
	}

	printf("State: %x. Received: %x.\n", (int) comState, received);
}

void setComState(serial_state_t state) {
	printf("SetComState called. State: %x.\n", (int) state);

	switch (state) {
	case WAITING_START:
		comState = WAITING_START;
		break;
	case ENDED:
		comState = NONE;
		break;
	}

}

serial_state_t getComState() {
	return comState;
}
