#include "helper.h"

//DEFINES

//Moisture Sensor Pin
#define MOIST_SENSOR_PIN A5
//LED and Transistor controller Pin - Used to showcase both status of the System and to control the circuit current for the Water Valve.
#define CONTROL_PIN 2

//Integer used to produce a readable output of what level of moisture the soil is currently at.
int moist = 0;
//Boolean used to determine if the plant has been watered or not according to the code.
bool watered = false;
//Boolean used to determine if the plant could potentially have been watered according to the code however the sensors are still detecting it as unwatered.
bool uncertWater = false;
//Change this to 'true' if you're currently debugging the code and don't want to wait an entire day for the next watering.
bool debug = false;

//MAIN CODE
void setup() {
  pinMode(MOIST_SENSOR_PIN, INPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  Serial.begin(9600);

  moist = analogRead(A5);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(moist);
  if (analogRead(MOIST_SENSOR_PIN) != moist) {
    moist = analogRead(MOIST_SENSOR_PIN);
  }

  if (!watered && !uncertWater) {
    MoistChecking(moist);
  } else if (watered || uncertWater) {
    if (debug) {
      //Debug version of this to both showcase it works and to test stuff, will instead wait 10 seconds before repeating the loop
      delay(SECONDS(5));

      //Reset the variables as we are running in debug mode and there's a chance we might miss reseting them otherwise
      uncertWater = false;
      watered = false;
    } else if (uncertWater) {
      //Delay by 12 Hours before repeating the loop as we are uncertain if the plant has actually been watered
      delay(HOURS(12));
      uncertWater = false;
    } else {
      //If we have watered today then wait exactly 24 hours until we repeat this cycle.
      delay(DAYS(1));
    }
    //It has been an unknown specified amount of time (Probably 24 hours), the plant is checked if it needs watering yet again
    watered = false;
  }
}

/**
   MoistChecking

   @brief Checks the moisture level of the soil and if it is below a certain threshold, it will allow the circuit to complete and water the plant.
   @param input The current moisture level of the soil
*/
void MoistChecking(int input) {
  if (input <= 500) {
    //Allow circuit complete connection to power source
    digitalWrite(CONTROL_PIN, HIGH);

    //Waits for twenty seconds of continous watering before checking for errors and power control
    delay(SECONDS(20));
    PowerControl(input);
  }
}

/**
   PowerControl

   @brief Checks to see if the system is telling us that there still hasn't been any watering done, in which case we allow for an error of margin.
   @param check The current moisture level of the soil
*/
void PowerControl(int check) {
  //Check to see if the system is telling us that there still hasn't been any watering done, in which case we allow for an error of margin.
  if (check <= 100) {
    //Terminates Ground connection to power source
    digitalWrite(CONTROL_PIN, LOW);

    //We have watered the plant, however due to the resulting conflicting inputs, we are instead waiting for half the usual time.
    uncertWater = true;
  }

  //If no error has been detected then terminate the connection
  if (check >= 500) {
    //Terminates Ground connection to power source
    digitalWrite(CONTROL_PIN, LOW);

    //We have watered the plant
    watered = true;
  }
}
