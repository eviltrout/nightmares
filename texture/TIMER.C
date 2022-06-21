#include	<dos.h>

#define TimerResolution    1193L
#define UL unsigned long

static UL cardinal(UL l)
{
	return( (signed long)l<0 ? 4294967296.0 + l : l );
}

UL
elapsedtime(UL start, UL stop)
{
	return( (cardinal(stop - start) / TimerResolution));
}
