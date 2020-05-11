#pragma once

#include <d3dx9.h>

extern int moveXPos;
extern int moveYPos;
extern int last_Xpos;
extern int last_Ypos;
extern int delta;
extern int scroll;
extern int screen_width;
extern int screen_height;

class Camera
{
	public:
		enum CameraType { LANDOBJECT, AIRCRAFT };

		Camera();
		Camera(CameraType cameraType);
		~Camera();

		bool elore_gomb;
		bool hatra_gomb;
		bool jobbra_gomb;
		bool balra_gomb;
		bool lenez_gomb;
		bool felnez_gomb;
		bool jobbrafordul_gomb;
		bool balrafordul_gomb;
		bool emelkedik_gomb;
		bool sullyed_gomb;
		bool shift_gomb;
		bool q_gomb;
		bool y_gomb;

		void strafe(float units); // left/right
		void fly(float units);    // up/down
		void walk(float units);   // forward/backward

		void pitch(float angle); // rotate on right vector
		void yaw(float angle);   // rotate on up vector
		void roll(float angle);  // rotate on look vector

		void getViewMatrix(D3DXMATRIX* V);
		void setCameraType(CameraType cameraType);
		void getPosition(D3DXVECTOR3* pos);
		void setPosition(D3DXVECTOR3* pos);

		void getRight(D3DXVECTOR3* right);
		void getUp(D3DXVECTOR3* up);
		void getLook(D3DXVECTOR3* look);
		
		void camera_move(D3DXVECTOR3 &);
		void camera_turn(void);

	private:
		CameraType  _cameraType;
		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;
		D3DXVECTOR3 _look;
		D3DXVECTOR3 _pos;
};