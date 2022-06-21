/* timer.h
 * Include file defining the timer routines.
 * Include this file in any program using them.
 */

/*Calculate time elapsed (in milliseconds) between Start and Stop*/
unsigned long	elapsedtime(long start, long stop);

/*Reprogram the timer chip to allow 1 millisecond resolution*/
extern void	initializetimer(void);

/*Restore the timer chip to its normal state*/
extern void	restoretimer(void);

/*Read the timer with 1 millisecond resolution*/
extern long	readtimer(void);
