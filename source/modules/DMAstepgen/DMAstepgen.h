#ifndef DMASTEPGEN_H
#define DMASTEPGEN_H

#include <cstdint>
#include <string>
#include <sys/errno.h>

#include "../module.h"
#include "../../drivers/pin/pin.h"

#include "extern.h"

void createDMAstepgen(void);

class DMAstepgen : public Module
{
  private:

    int mask;

    int jointNumber;              			// LinuxCNC joint number
    std::string step, direction;	 		// physical pins connections
    uint16_t DMAbufferSize;					// DMA buffer size, number of stepgen iterations be call
    int32_t stepBit;                		// position in the DDS accumulator that triggers a step pulse
    volatile int32_t *ptrFrequencyCommand; 	// pointer to the data source where to get the frequency command
    volatile int32_t *ptrFeedback;       	// pointer where to put the feedback
    volatile uint8_t *ptrJointEnable;

    uint8_t timer1, timer2, timer3;			// step pattern timers
    uint8_t stepTime;						// step time in number of DMA periods
    uint8_t dirHoldTime, dirSetupTime;		// dir times in number of DMA periods

    int32_t *stepDMAbuffer;
    int32_t *stepDMAbuffer_0;
    int32_t *stepDMAbuffer_1;
    int32_t *dirDMAbuffer_0;
    int32_t *dirDMAbuffer_1;

    bool *stepDMAactiveBuffer;
    bool *dirDMAactiveBuffer;

    uint32_t stepMask;						// step and direction pin locations in DR_TOGGLE registers
    uint32_t dirMask;

    bool isEnabled;        					// flag to enable the step generator
    //bool isForward;        					// current direction
    bool dir;								// direction of this servo period
    bool oldDir;							// direction of previous servo period
    bool holdDDS;							// used to hold DDS calculations during direction setup time
    //bool isStepping;

    int32_t frequencyCommand;     			// the joint frequency command generated by LinuxCNC
    int32_t rawCount;             			// current position raw count
    int32_t DDSaccumulator;       			// Direct Digital Synthesis (DDS) accumulator
    int32_t accumulator;       				// accumulator
    //float   frequencyScale;		  			// frequency scale
  	int32_t	addValue;		  				//accumulator add add value
  	int32_t oldaddValue;					// previous accumulator add value
  	int32_t remainder;
  	int32_t prevRemainder;
  	int16_t stepPos;
  	int16_t stepHigh;
  	int16_t stepLow;

  	void makeStep();

  public:

    DMAstepgen(int32_t, int, std::string, std::string, int, int, volatile int32_t&, volatile int32_t&, volatile uint8_t&);  // constructor

    Pin *stepPin, *directionPin, *debug;		// class object members - Pin objects

    virtual void update(void);           // Module default interface
    virtual void updatePost(void);
    virtual void slowUpdate(void);
    void makePulses();
    void stopPulses();
    void setEnabled(bool);
};


#endif