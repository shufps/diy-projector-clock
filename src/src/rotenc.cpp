/* 
 * This file is part of DIY-Projector-Clock
 * www.microengineer.eu
 * 
 * If you like my work, please consider to donate IOTAs to:
 * LLEYMHRKXWSPMGCMZFPKKTHSEMYJTNAZXSAYZGQUEXLXEEWPXUNWBFDWESOJVLHQHXOPQEYXGIRBYTLRWHMJAOSHUY
 *
 *
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// rotation-encoder is based on:
// https://www.mikrocontroller.net/articles/Drehgeber

#include "main.h"

static volatile int8_t enc_delta;          // -128 ... 127
static int8_t last;

#define PHASE_A	(GPIO_ReadInputDataBit(GPIO_PHASEA) == Bit_SET)
#define PHASE_B	(GPIO_ReadInputDataBit(GPIO_PHASEB) == Bit_SET)

void rotenc_irq_process() {
	int8_t _new, diff;

	_new = 0;
	if (PHASE_A)
		_new = 3;
	if (PHASE_B)
		_new ^= 1;          // convert gray to binary
	diff = last - _new;               // difference last - new
	if (diff & 1) {                 // bit 0 = value (1)
		last = _new;                    // store new as next last
		enc_delta += (diff & 2) - 1;   // bit 1 = direction (+/-)
	}

}


void rotenc_init() {
	int8_t _new;

	_new = 0;
	if ( PHASE_A)
		_new = 3;
	if ( PHASE_B)
		_new ^= 1;       // convert gray to binary
	last = _new;                   // power on state
	enc_delta = 0;
}

int8_t rotenc_read1(void)         // read single step encoders
{
	int8_t val;

	__disable_irq();
	val = enc_delta;
	enc_delta = 0;
	__enable_irq();
	return val;                   // counts since last call
}
