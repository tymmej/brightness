/* brightness.c -- Set brightness based on hour
This file is part of brightness.

Brightness is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Brightness is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Brightness. If not, see <http://www.gnu.org/licenses/>.

Copyright (c) 2013 Jakub Tymejczyk <tymmej@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "solar.h"

#define TRANSITION_LOW     SOLAR_CIVIL_TWILIGHT_ELEV
#define TRANSITION_HIGH    3.0
#define DEG(x)  ((x)*(180/M_PI))

/*also from Redshift*/
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
int prev_brightness=0;
int elevation=0;

int r=0;

FILE *file;

while(1){
	r=systemtime_get_time(&date);
	elevation=solar_elevation(date, lat, lon);
	brightness=(int)calculate_interpolated_value(elevation, brightness_day, brightness_night);
	if(brightness != prev_brightness){
		file=fopen("/sys/class/backlight/acpi_video0/brightness", "w");
		if (file == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fprintf(file, "%d", brightness);
		fclose(file);
	}
	prev_brightness=brightness;
	sleep(60);
}
return 0;
}
