/*
	Ynvisible_ECD-1.0.0.h - Library for driving Ynvisibles electrochromic display
	Created by Philip Holgersson - Ynvisible, April 12, 2021
*/

#ifndef _YNV_ECD
#define _YNV_ECD

#define MAX_NUMBER_OF_SEGMENTS 100

#include "Arduino.h"

class YNV_ECD
{
	public:
		//FUNCTIONS
		YNV_ECD(char counter_electrode, char counter_enable, char segments[], char number_of_segments, char driving_method = 'P', float supply_voltage = 5.0);
		void init();
		void set(bool next_state[]);
		void refresh();
		void reset();

		//PAR DRIVE VARIABLES
		float _par_reduction_voltage = 2.6;
		int _par_switching_time = 2200;
		
		//SEQ DRIVE VARIABLES
		int _seq_reduction_time = 2200;
		int _seq_oxidation_time = 2200;
		float _seq_reduction_voltage = 3.0;
		float _seq_oxidation_voltage = -3.0;

		//GENERAL VARIABLES
		char _number_of_segments;
		char _driving_method;
		bool _current_state[MAX_NUMBER_OF_SEGMENTS];
		float _supply_voltage;

		//REFRESH VARIABLES
		int _ref_reduction_voltage = 2.6;
		int _ref_reduction_time = 500;

		//PORTS 
		char _counter_electrode; 
		char _counter_enable;
		char _segments[MAX_NUMBER_OF_SEGMENTS];

	private:
		//FUNCTIONS
		void setPar(bool next_state[]); 
		void setSeq(bool next_state[]); 
		void highImpMode();
		void setRegisters();
		void enableCounter(float voltage);
		void disableCounter();
};

#endif