#include <cstdlib>
#include <math.h> 

class Functions
{
	public:
		float distance2d(float pos1x, float pos1y, float pos2x, float pos2y)
		{
			float a = 0;
			float b = 0;
			float c = 0;
			
			if (pos1x > pos2x) { a = pos1x - pos2x; }
			if (pos1x < pos2x) { a = pos2x - pos1x; }

			if (pos1y > pos2y) { b = pos1y - pos2y; }
			if (pos1y < pos2y) { b = pos2y - pos1y; }
			
			c = sqrt(pow(a, 2.0) + pow(b, 2.0));

			return c;
		}
		/*float getdir2d(float pos1x, float pos1y, float pos2x, float pos2y)
		{
			float dir = 0;
			...

			return dir;
		}*/
};