#include <wiringPi.h>
#include <signal.h>
#include <pthread.h>

#define WAVE_PERIOD_MS 10

#define STATE_FORWARD 	1
#define STATE_BACK 		-1
#define STATE_STOP		0

static int left_wheel_state = 0;
static int right_wheel_state = 0;
static int left_wheel_speed = 0;
static int right_wheel_speed = 0;
static int speed_left_fix = -20;
static int speed_right_fix = 0;


static int left_wheel_up_delayms = 0;
static int left_wheel_down_delayms = 0;
static int right_wheel_up_delayms = 0;
static int right_wheel_down_delayms = 0;

/*
speed: 0-100, 100 full speed. Suppose to be 0, 10, 20, 30, 40, 50, ..., 90, 100
*/
static void set_wheel_speed(is_left, speed)
{
	if(is_left) {
		if(left_wheel_speed == speed) {
			return;
		}
		left_wheel_speed = speed;
		left_wheel_up_delayms = speed / WAVE_PERIOD_MS;
		left_wheel_down_delayms = (100 - speed) / WAVE_PERIOD_MS;
	}
	else {
		if(right_wheel_speed == speed) {
			return;
		}
		right_wheel_speed = speed;
		right_wheel_up_delayms = speed / WAVE_PERIOD_MS;
		right_wheel_down_delayms = (100 - speed) / WAVE_PERIOD_MS;
	}
}

/*
LEFT BACK=====>
0 -> HIGH
1 -> LOW

LEFT FORWARD====>
0 -> LOW
1 -> HIGH

speed: 0-100, 100 full speed. Suppose to be 0, 10, 20, 30, 40, 50, ..., 90, 100
*/
static void __left_wheel_internal(int state)
{
	switch(state)
	{
	case STATE_FORWARD:
		digitalWrite(0, LOW);
    	digitalWrite(1, HIGH);
	break;
	case STATE_BACK:
		digitalWrite(0, HIGH);
    	digitalWrite(1, LOW);
	break;
	case STATE_STOP:
		digitalWrite(0, LOW);
    	digitalWrite(1, LOW);
	break;
	}
}

/*
RIGHT BACK=====>
0 -> HIGH
1 -> LOW

RIGHT FORWARD====>
0 -> LOW
1 -> HIGH
*/
static void __right_wheel_internal(int state)
{
	switch(state)
	{
	case STATE_FORWARD:
		digitalWrite(2, LOW);
    	digitalWrite(3, HIGH);
	break;
	case STATE_BACK:
		digitalWrite(2, HIGH);
    	digitalWrite(3, LOW);
	break;
	case STATE_STOP:
		digitalWrite(2, LOW);
    	digitalWrite(3, LOW);
	break;
	}
}

static void pause_wheels()
{
	if(left_wheel_state != STATE_STOP || 
		right_wheel_state != STATE_STOP)
	{
		left_wheel_state = STATE_STOP;
		right_wheel_state = STATE_STOP;
		delay(200);
	}
}

static void left_wheel_control(int state, int speed)
{
	__left_wheel_internal(state);

	delay(left_wheel_up_delayms);

	__left_wheel_internal(STATE_STOP);

	delay(left_wheel_down_delayms);
}

static void right_wheel_control(int state, int speed)
{
	__right_wheel_internal(state);

	delay(right_wheel_up_delayms);

	__right_wheel_internal(STATE_STOP);

	delay(right_wheel_down_delayms);
}

static void trapsigterm(int signum)
{
	printf("trapsigterm\n");
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);

    exit(0);
}

static void trapsigquit(int signum)
{
    printf("trapsigquit\n");
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);

    exit(0);
}

static void on_stop(int signum)
{
	printf("on_stop\n");
	digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);

    exit(0);
}

void *pthread_func_left_wheel(void *no_use)
{
	while(1)
	{
		left_wheel_control(left_wheel_state, left_wheel_speed);
	}
	return NULL;
}

void *pthread_func_right_wheel(void *no_use)
{
	while(1)
	{
		right_wheel_control(right_wheel_state, right_wheel_speed);
	}
	return NULL;
}

void *pthread_func_console(void *no_use)
{
	char command[64] = {0};
	int speed = 0;
	while(1)
	{
		memset(command, 0, 64);
		printf("cmd:");
		scanf("%s %d", command, &speed);

		printf("Received command: %s %d\n", command, speed);
		if(speed < 0 || speed > 100)
		{
			printf("[ERROR] speed must be from 0 to 100\n");
			continue;
		}
		if(strcmp("fd", command) == 0)
		{
			pause_wheels();
			set_wheel_speed(1, speed + speed_left_fix);
			set_wheel_speed(0, speed + speed_right_fix);
			left_wheel_state = STATE_FORWARD;
			left_wheel_speed = speed;
			right_wheel_state = STATE_FORWARD;
			right_wheel_speed = speed;
		}
		else if(strcmp("bk", command) == 0)
		{
			pause_wheels();
			set_wheel_speed(1, speed + speed_left_fix);
			set_wheel_speed(0, speed + speed_right_fix);
			left_wheel_state = STATE_BACK;
			left_wheel_speed = speed;
			right_wheel_state = STATE_BACK;
			right_wheel_speed = speed;
		}
		else if(strcmp("rt", command) == 0)
		{
			pause_wheels();
			set_wheel_speed(1, speed);
			set_wheel_speed(0, speed);
			left_wheel_state = STATE_BACK;
			left_wheel_speed = speed;
			right_wheel_state = STATE_FORWARD;
			right_wheel_speed = speed;
		}
		else if(strcmp("lt", command) == 0)
		{
			pause_wheels();
			set_wheel_speed(1, speed);
			set_wheel_speed(0, speed);
			left_wheel_state = STATE_FORWARD;
			left_wheel_speed = speed;
			right_wheel_state = STATE_BACK;
			right_wheel_speed = speed;
		}
	}
	return NULL;
}

static void __test()
{
	while(1)
	{
		digitalWrite(1, LOW);
		digitalWrite(0, HIGH);
	}

}

int main(void)
{
	pthread_t pthread_leftwheel_tid;
	pthread_t pthread_rightwheel_tid;
	pthread_t pthread_console_tid;

	signal(SIGTERM, trapsigterm);
    signal(SIGQUIT, trapsigquit);
    signal(SIGINT, on_stop); 

	wiringPiSetup();
	pinMode (0, OUTPUT);
	pinMode (1, OUTPUT); 
	pinMode (2, OUTPUT); 
	pinMode (3, OUTPUT); 
	

	//__test();


	pthread_create(&pthread_leftwheel_tid, NULL, pthread_func_left_wheel, NULL);
	pthread_create(&pthread_rightwheel_tid, NULL, pthread_func_right_wheel, NULL);
	pthread_create(&pthread_console_tid, NULL, pthread_func_console, NULL);

	
	while(1)
	{
		delay(500);
	}
	return 0;
}
