class FPS
{
	public:
		int fps = 0;
		int get_fps(int kezdet, int vege)
		{
			if ((vege - kezdet) > 0)
			{
				fps = 1000 / (vege - kezdet);
			} else {
				fps = 1000 / (vege + (1000 - kezdet));
			}
			return fps;
		}
};