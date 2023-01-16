//DEFINES
#define SECONDS *1000
#define MINUTES SECONDS*60
#define HOURS MINUTES*60
#define DAYS HOURS*24

//VARIABLES
//Integer used to produce a readable output of what level of moisture the soil is currently at.
int moist = 0;
//Boolean used to determine if the plant has been watered or not according to the code.
bool watered = false;
//Boolean used to determine if the plant could potentially have been watered according to the code however the sensors are still detecting it as unwatered.
bool uncertWater = false;
//Change this to true if you're currently debugging the code and don't want to wait an entire day for the next watering.
bool debug = true;

//MAIN CODE
void setup() {
  // put your setup code here, to run once:
  pinMode(A5, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);

  moist = analogRead(A5);
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(moist);
  if (analogRead(A5) != moist) {
    moist = analogRead(A5);
  }

  if (!watered & !uncertWater) {
    MoistChecking(moist);
  } else if (watered | uncertWater) {
    if (debug) {
      //Debug version of this to both showcase it works and to test stuff, will instead wait 3 seconds before repeating the loop
      delay(3 SECONDS);

      //Reset the variables as we are running in debug mode and there's a chance we might miss reseting them otherwise
      uncertWater = false;
      watered = false;
    } else if (uncertWater) {
      //Delay by 12 Hours before repeating the loop as we are uncertain if the plant has actually been watered
      delay(12 HOURS);
      uncertWater = false;
    } else {
      //If we have watered today then wait exactly 24 hours until we repeat this cycle.
      delay(1 DAYS);
    }
    //It has been a day, the plant is checked if it needs watering yet again
    watered = false;
  }
}


void MoistChecking(int input) {
  if (input <= 50) {
    //Allow circuit complete connection to power source
    digitalWrite(2, HIGH);

    //Wait for five seconds of continous watering before checking for errors
    delay(5 SECONDS);
    PowerControl(input);
  }
}


void PowerControl(int check) {
  //Check to see if the system is telling us that there still hasn't been any watering done, in which case we allow for an error of margin.
  if (check <= 50) {
    //Terminates Ground connection to power source
    digitalWrite(2, LOW);

    //We have watered the plant, however due to the resulting conflicting inputs, we are instead waiting for half the usual time.
    uncertWater = true;
  }
  //If no error has been detected then terminate the connection
  if (check >= 100) {
    //Terminates Ground connection to power source
    digitalWrite(2, LOW);

    //We have watered the plant
    watered = true;
  }
}
