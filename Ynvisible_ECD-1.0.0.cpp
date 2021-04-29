#include "Arduino.h"
#include "Ynvisible_ECD-1.0.0.h"

/*********************** PUBLIC FUNCTIONS ************************/
YNV_ECD::YNV_ECD(char counter_electrode, char counter_enable, char segments[], char number_of_segments, char driving_method, float supply_voltage) //Constructor
{
  setRegisters();
  _counter_electrode = counter_electrode;
  _counter_enable = counter_enable;
  _number_of_segments = number_of_segments;
  _driving_method = driving_method;
  _supply_voltage = supply_voltage;

  for (int i = 0; i < _number_of_segments; i++)
  {
    _segments[i] = segments[i];
    pinMode(_segments[i], INPUT);
  }
  highImpMode();
}

void YNV_ECD::init() //Removes all content.
{
  bool reset_1[_number_of_segments];
  bool reset_2[_number_of_segments];
  for (int i = 0; i < _number_of_segments; i++)
  {
    _current_state[i] =  0;
    reset_1[i] = 1;
    reset_2[i] = 0;
  }    
  set(reset_1);
  set(reset_2);
}

void YNV_ECD::set(bool next_state[]){

  if(_driving_method == 'P'){
    setPar(next_state);
  }

  if(_driving_method == 'S'){
    setSeq(next_state);
  }

}

void YNV_ECD::refresh() //Refreshes the dipslay to maintain the current state.
{
  enableCounter(_supply_voltage - _ref_reduction_voltage);
  for (int i = 0; i < _number_of_segments; i++)
  {

    if(_current_state[i] == 1){
      pinMode(_segments[i], OUTPUT);
      digitalWrite(_segments[i], _current_state[i]);
    }
  }
  delay(_ref_reduction_time); 
  highImpMode(); 
  disableCounter();
}


void YNV_ECD::reset() //Removes all content.
{
  init();
}
/********************* END PUBLIC FUNCTIONS **********************/


/*********************** PRIVATE FUNCTIONS ***********************/
void YNV_ECD::setPar(bool next_state[]) //Function for updating the display in parallel mode.
{
  enableCounter(_supply_voltage-_par_reduction_voltage);
  for (int i = 0; i < _number_of_segments; i++)
  {
    if(next_state[i] != _current_state[i])
    {
      pinMode(_segments[i], OUTPUT);
      digitalWrite(_segments[i], next_state[i]);
    }
    _current_state[i] = next_state[i];
  }
  delay(_par_switching_time);
  highImpMode();
  disableCounter();
}


void YNV_ECD::setSeq(bool next_state[]) //Function for updating the display in sequential mode.
{
  enableCounter(-_seq_oxidation_voltage);
  bool delay_required = false;
  for (int i = 0; i < _number_of_segments; i++)
  {
    if(next_state[i] != _current_state[i])
    {
      if(next_state[i] == 0){
        pinMode(_segments[i], OUTPUT);
        digitalWrite(_segments[i], next_state[i]);
        _current_state[i] = next_state[i];
        delay_required = true;
      }
    }
  }
  if(delay_required == true){
    delay(_seq_oxidation_time);
  }
  highImpMode();

  enableCounter(_supply_voltage - _seq_reduction_voltage);
  delay_required = false;
  for (int i = 0; i < _number_of_segments; i++)
  {
    if(next_state[i] != _current_state[i])
    {
      if(next_state[i] == 1){
        pinMode(_segments[i], OUTPUT);
        digitalWrite(_segments[i], next_state[i]);
        _current_state[i] = next_state[i];
        delay_required = true;
      }
    }
  }
  if(delay_required == true){
    delay(_seq_reduction_time);
  }
  highImpMode();
  
  disableCounter();
}

void YNV_ECD::highImpMode(){ //Put all work electrodes in High-Z mode.
  for (int i = 0; i < _number_of_segments; i++)
  {
    pinMode(_segments[i], INPUT);
  }
}

void YNV_ECD::setRegisters(){ //Set counter pins as outputs.
  pinMode(_counter_electrode, OUTPUT);
  pinMode(_counter_enable, OUTPUT);
}

void YNV_ECD::enableCounter(float voltage) //Enable counter electrode
{
  setRegisters();
  analogWrite(_counter_electrode, int(255*(voltage/_supply_voltage)));   
  delay(50);
  digitalWrite(_counter_enable, HIGH);
  delay(50);
}

void YNV_ECD::disableCounter() //Set counter electrode in High-Z.
{
  digitalWrite(_counter_enable, LOW);
  analogWrite(_counter_electrode, 0); 
}
/********************* END PRIVATE FUNCTIONS **********************/