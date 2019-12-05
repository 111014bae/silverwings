// project_sub에서 main.c에 사용된 함수들 작성
// project.h에 함수 이름 처넣기. 헤더파일 넣기



int main();
{
	int start = 0;
	int exit = 0;
	
	game_wait_start( ); // 게임시작 대기화면
	{
		//이미지 game start 표시
		// start 클릭시 >> start = 1, exit = 0 return OR vol.up버튼으로 대치
		
		//이미지 game exit 표시
		// exit 클릭시 >> kill project OR vol.dn버튼으로 대치
	}
	
	game_wait_end_fail(); // 실패 종료 대기화면. 라이브러리 추가 필요.
	{
		printf ("fail game\n");
		pirntf ("stage = %d\n", stage);
		printf ("END GAME << click this \n"); // 이미지로 대충 이렇게 표현. OR 버튼으로 처리
	}
	
	int stage = 0;//스테이지 값
	int random = 0; //난수발생기 값
	int time = N; // stage 당 주어지는 시간
	int delay = 0;// 1< stage <5 딜레이 3s , 6< stage <10 딜레이 2s , 11< stage <16 딜레이 1s 처럼 stage따라 딜레이 변화. 

	game_start(start=1); // 게임시작시 설정값 생성
	{
		//게임시작대기 이미지 출력
		random = rand()%16; //random, 난수발생기 난수 발생. 0~15값 생성 ,<stdlib.h>
		
	}

	while(1) // 게임루프 부분
	// for(stage=0;satge<=10;stage++)도 사용가능
	{

	if(0<=random && random <=4)  //touch game, success_flag 추가가능.
	{
		touch game();
		{
			success_flag = 0; // 성공 여부 체크
			// text lcd에 stage 표시, text lcd 함수 호출 첫번째줄에 표시
			//time= time - stage , 시간을 초당 1씩 줄이는 코드, text lcd 2번째줄에 표시
			// 게임 이미지 호출
			// 이미지 좌표값 호출
			// touchscreen click 좌표값 read
			// 이미지 좌표값 = 터치 좌표값 >> success, success_flag = 1; >break > delay 적용 후 다음 stage.
			// 이미지 좌표값 != 터치 좌표값 or time out>> fail >> go game_wait_end_fail
			
			//success시 stage +1, 난수발생기 재실행, time 값 return, success_flag = 1;
		}
			delay= 1s; // 1< stage <5 딜레이 3s , 6< stage <10 딜레이 2s , 11< stage <16 딜레이 1s 처럼 stage따라 딜레이 변화.
	}

	if(5<=random && random <=10) //button game
	{
		button game();
		{
			// text lcd에 stage 표시, text lcd 함수 호출 첫번째줄에 표시
			//time= time - stage , 시간을 초당 1씩 줄이는 코드, text lcd 2번째줄에 표시
			// 이미지 호출
			// 이미지 방향키값 호출
			// 입력 button 값 read
			// 이미지 값 = 버튼 값 >> success
			// 이미지 값 != 버튼 값 or time out >> fail >> go game_wait_end_fail
			
			//success시 stage +1, 난수발생기 재실행, time 값 return		
		}
	}
	
	if(11<=random && random <=15) //gyro game
	{
		gyro game();
		{
			// text lcd에 stage 표시, text lcd 함수 호출 첫번째줄에 표시
			//time= time - stage , 시간을 초당 1씩 줄이는 코드, text lcd 2번째줄에 표시
			// 이미지 호출 *상하좌우
			// 초기 gyro 값 호출 
			// 입력 gyro 값 read
			// if (상) x>0,y>0,z>0 >> success, else fail > go game_wait_end_fail
			// if (하) x>0,y>0,z>0 >> success, else fail > go game_wait_end_fail
			// if (좌) x>0,y>0,z>0 >> success, else fail > go game_wait_end_fail
			// if (우) x>0,y>0,z>0 >> success, else fail > go game_wait_end_fail
			
			//success시 stage +1, 난수발생기 재실행, time 값 return			
			
		}
	}
	
	if(stage = 10 && time <5 && ) //게임 성공으로 종료하는 부분, 조건 변경 가능
		{
			sucess_flag = 2;
			break;
			
		}
	}
		game_wait_end_success(); //success_flag = 2 일때 실행.(게임종료,정산 화면)
		{
			/*game_wait_end_success();
			{
				printf("success game\n");
				printf("stage = %d\n", stage);
				printf("game end << press home button\n"); //home 버튼 누르면 game_wait_start 로, 이미지로 대충 이렇게 표현.
			}*/			
			
		}
}
