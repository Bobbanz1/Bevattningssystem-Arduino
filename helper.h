/**
 * Seconds
 * 
 * @param Number
 * @return Input converted to Seconds
 */
#define SECONDS(i) (i*1000)

/**
 * Minutes
 * 
 * @param Number
 * @return Input converted to Minutes
 */
#define MINUTES(i) (SECONDS(i)*60)

/**
 * Hours
 * 
 * @param Number
 * @return Input converted to Hours
 */
#define HOURS(i) (MINUTES(i)*60)

/**
 * Days
 * 
 * @param Number
 * @return Input converted to Days
 */
#define DAYS(i) (HOURS(i)*24)
