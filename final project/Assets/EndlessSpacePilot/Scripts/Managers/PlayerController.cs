using System;
using UnityEngine;
using System.Collections;

namespace EndlessSpacePilot
{
	public class PlayerController : MonoBehaviour
	{
		///***********************************************************************
		/// PlayerController class
		/// This class is responsible to move the player by touch or by device acceleraion sensors
		///***********************************************************************

		//Player Control Type (tilt or touch)
		public static int controlType; //0=tilt , 1=touch

		//Distance between player and user's finger
		private int fingerOffset = 100;

		//Private internal variables
		private int key =1;
		private float xVelocity = 0.0f;
		private float zVelocity = 0.0f;
		private float speed = 23.0f;
		private Vector3 dir = Vector3.zero;
		private Vector3 screenToWorldVector;

		private float position_x = 583;
		private float position_y = 225;

		void Awake()
		{
			//fetch user defined controlType
			controlType = PlayerPrefs.GetInt("controlType");
		}

		void Start()
		{
			// Y offset for player
			transform.position = new Vector3(transform.position.x,
											 0.5f,
											 transform.position.z);
		}
		void Update()
		{
			if (!GameController.gameOver)
			{

				if (controlType == 0)
					tiltControl();
				else
					touchControl();

				//this is just for debug and play in PC and SHOULD be commented at final build
				//this can also be used to override control type for WebPlayer and Standalone...
				if (Application.isEditor)
				{

					
					int offset_x = 0;
					int offset_y = 0;
					// Debug.Log("Received: " + ServerScript.stm_x + "Received y: "+ ServerScript.stm_y);

					if(ServerScript.stm_x > 4738647){
						offset_x = 1;
					}
					else if(ServerScript.stm_x < -5738647){
						offset_x = -1;
					}
					else if(ServerScript.stm_y > 8105634){
						offset_y = -1;
					}
					else if(ServerScript.stm_y < -9105634){
						offset_y = +1;
					}
					if(offset_x != 0){
						if(key == 1){
							key = 0;
						}
						else{
							offset_x = Convert.ToInt32(offset_x * 0.1);
							offset_y = Convert.ToInt32(offset_y * 0.1);
							key = 1;
						}
					}
					Debug.Log("x: "+ position_x + " y: " + position_y );
					if(position_x < 650 && position_x >360){
						position_x += offset_y;
					}
					else if(position_x >= 650){
						position_x = 640;
					}
					else if(position_x <= 360){
						position_x = 370;
					}

					if(position_y < 410 && position_y >0){
						position_y += offset_x;
					}
					else if(position_y >= 410){
						position_y = 400;
					}
					else if(position_y <= 0){
						position_y = 10;
					}
					// x: 360 - 650
					// y: 0 - 410
					screenToWorldVector = Camera.main.ScreenToWorldPoint(new Vector3(position_x, position_y, 10));
					// screenToWorldVector = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x + offset_y, Input.mousePosition.y + offset_x + fingerOffset, 10));
					// Debug.Log("x: "+Input.mousePosition.x + " y: " + Input.mousePosition.y );
					float editorX = Mathf.SmoothDamp(transform.position.x, screenToWorldVector.x, ref xVelocity, 0.1f);
					float editorZ = Mathf.SmoothDamp(transform.position.z, screenToWorldVector.z, ref zVelocity, 0.1f);
					transform.position = new Vector3(editorX, transform.position.y, editorZ);

				}

				//offset for player
				transform.position = new Vector3(transform.position.x,
												 0.5f,
												 transform.position.z);

				//prevent player from exiting the view (downside)
				if (transform.position.z < -5)
					transform.position = new Vector3(transform.position.x,
													 transform.position.y,
													 -5);

				//prevent player from exiting the view (Upside)
				if (transform.position.z > 2.9f)
					transform.position = new Vector3(transform.position.x,
													 transform.position.y,
													 2.9f);

				//left/right movement limiter
				if (transform.position.x > 2.9f)
					transform.position = new Vector3(2.9f,
													 transform.position.y,
													 transform.position.z);

				if (transform.position.x < -2.9f)
					transform.position = new Vector3(-2.9f,
													 transform.position.y,
													 transform.position.z);
			}
		}

		///***********************************************************************
		/// Control playerShip's position by acceleration sensors
		///***********************************************************************
		void tiltControl()
		{
				// int offset_x = 0;
				// int offset_y = 0;
				// Debug.Log("Received: " + ServerScript.stm_x + "Received y: "+ ServerScript.stm_y);

				// if(ServerScript.stm_x > 4738647){
				// 	offset_x = -100;
				// 	Debug.Log("Move forward");
				// }
				// else if(ServerScript.stm_x < -5738647){
				// 	offset_x = 100;
				// 	Debug.Log("Move backward");
				// }
				// else if(ServerScript.stm_y > 8105634){
				// 	offset_y = -100;
				// 	Debug.Log("Move left");
				// }
				// else if(ServerScript.stm_y < -8105634){
				// 	offset_y = 100;
				// 	Debug.Log("Move right");
				// }
			// if (Input.touchCount > 0 || Input.GetMouseButton(0))
			// {

			// 	screenToWorldVector = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x + offset_x, Input.mousePosition.y + offset_y + fingerOffset, 10));
			// 	float touchX = Mathf.SmoothDamp(transform.position.x, screenToWorldVector.x*100000, ref xVelocity, 0.1f);
			// 	float touchZ = Mathf.SmoothDamp(transform.position.z, screenToWorldVector.z, ref zVelocity, 0.1f);
			// 	transform.position = new Vector3(touchX, transform.position.y, touchZ);
			// }
		}


		///***********************************************************************
		/// Control playerShip's position with touch position parameters
		///***********************************************************************
		void touchControl()
		{
				// int offset_x = 0;
				// int offset_y = 0;
				// Debug.Log("Received: " + ServerScript.stm_x + "Received y: "+ ServerScript.stm_y);

				// if(ServerScript.stm_x > 4738647){
				// 	offset_x = -100;
				// 	Debug.Log("Move forward");
				// }
				// else if(ServerScript.stm_x < -5738647){
				// 	offset_x = 100;
				// 	Debug.Log("Move backward");
				// }
				// else if(ServerScript.stm_y > 8105634){
				// 	offset_y = -100;
				// 	Debug.Log("Move left");
				// }
				// else if(ServerScript.stm_y < -8105634){
				// 	offset_y = 100;
				// 	Debug.Log("Move right");
				// }
			// if (Input.touchCount > 0 || Input.GetMouseButton(0))
			// {
			// 	screenToWorldVector = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y + fingerOffset, 10));
			// 	float touchX = Mathf.SmoothDamp(transform.position.x, screenToWorldVector.x, ref xVelocity, 0.1f);
			// 	float touchZ = Mathf.SmoothDamp(transform.position.z, screenToWorldVector.z, ref zVelocity, 0.1f);
			// 	transform.position = new Vector3(touchX, transform.position.y, touchZ);
			// }
		}


		void playSfx(AudioClip _sfx)
		{
			GetComponent<AudioSource>().clip = _sfx;
			if (!GetComponent<AudioSource>().isPlaying)
				GetComponent<AudioSource>().Play();
		}

	}
}