#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "solar.h"

#define TRANSITION_LOW     SOLAR_CIVIL_TWILIGHT_ELEV
#define TRANSITION_HIGH    3.0
#define DEG(x)  ((x)*(180/M_PI))

//also from redshift
float calculate_interpolated_value(double elevation, float day, float night)
{
	float result;
	if (elevation < TRANSITION_LOW) {
		result = night;
	} else if (elevation < TRANSITION_HIGH) {
		/* Transition period: interpolate */
		float a = (TRANSITION_LOW - elevation) /
			(TRANSITION_LOW - TRANSITION_HIGH);
		result = (1.0-a)*night + a*day;
	} else {
		result = day;
	}

	return result;
}

int main(){

int brightness_day=20;
int brightness_night=5;

double date=0;
double lat=51.0;
double lon=17.0;

int brightness=20;
int elevation=0;

int i=0;
int r=0;

FILE *file;

r=systemtime_get_time(&date);
elevation=solar_elevation(date, lat, lon);
brightness=(int)calculate_interpolated_value(elevation, brightness_day, brightness_night);

while(1){
	file=fopen("/sys/class/backlight/acpi_video0/brightness", "w");
	if (file == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	fprintf(file, "%d", brightness);
	fclose(file);
	sleep(60);
}
return 0;
}
