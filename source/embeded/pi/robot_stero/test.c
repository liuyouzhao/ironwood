#include <wiringPi.h>
#include <wiringSerial.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>

static int fd;
unsigned char DM0_Speed20_Position_90[15]   =    { 0xff, 0x01, 0x00, 0x14, 0x00, 0xff, 0x02, 0x00, 0xdc, 0x05 };
unsigned char DM0_Speed1_Position_90[15]    =    { 0xff, 0x01, 0x00, 0x01, 0x00, 0xff, 0x02, 0x00, 0xdc, 0x05 };  //通道0，速度=1，位置90度
unsigned char DM2_Speed1_Position_0[15]     =    { 0xff, 0x01, 0x02, 0x01, 0x00, 0xff, 0x02, 0x02, 0xf4, 0x01 };
unsigned char reset_action_group[5]   = { 0xff, 0x09, 0x00, 0x00, 0x00 };
static int param_angle_from_freq = 500;
static int param_angle_to_freq = 2500;
static int angle_range = 180;
static const int channel_num = 16;


static void stero_action_group(int channel)
{
    reset_action_group[3] = channel;
    write(fd, reset_action_group, 5);
    usleep(1000 * 100);
}

/*
| Mark | Command   | Channel 0-15 | speed 1-20  | End  | Mark | Command | Channel 0-15 | Angle 500-2500   |
| 0xff | 0x01      | [0x00-0x0f]  | [0x01-0x14] | 0x00 | 0xff | 0x02    | [0x00-0x0f]  | 0xdc, 0x05 |

Arguments:
channel: 1-20
degree: 0-180
speed: 1-20
*/
static void stero_control(int channel, int degree, int speed)
{
    unsigned char cn = channel;
    unsigned int angle = (degree * (param_angle_to_freq - param_angle_from_freq) / angle_range + param_angle_from_freq);
    unsigned char alow = angle & 0xff;
    unsigned char ahigh = (angle & 0xff00) >> 8;
    unsigned char spd = speed;

    printf("angle: %x %x\n", alow, ahigh);
    
    unsigned char cmd[15] = {0xff, 0x01, cn, speed, 0x00, 0xff, 0x02, cn, alow, ahigh};
//    stero_action_group(channel);
    write(fd, cmd, 10);
//    usleep(1000 * 1000);
}

static void trapsigterm(int signum)
{
    printf("trapsigterm\n");
    serialClose(fd);
    exit(0);
}

static void trapsigquit(int signum)
{
    printf("trapsigquit\n");
    serialClose(fd);

    exit(0);
}

static void on_stop(int signum)
{
    printf("on_stop\n");
    serialClose(fd);

    exit(0);
}

int main(int argc, char **argv)
{

    char command[64] = {0};
    int value = 0;
    int speed = 10;
    int channel = 0;
    int i = 0;

    pid_t pid = getpid();
    struct sched_param sparam;
    sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(pid, SCHED_RR, &sparam);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam);    

    signal(SIGTERM, trapsigterm);
    signal(SIGQUIT, trapsigquit);
    signal(SIGINT, on_stop); 

    wiringPiSetup();
    fd = serialOpen("/dev/ttyAMA0", 9600);
    usleep(1000 * 500);
    if(fd < 0) return -1;
    printf("**************************************\n"); 
    printf("*** Welcome to control the stero ***\n");
    printf("Default speed is 10\n");
    printf("Commands:\n");
    printf("cn 2 \"select channel 2\"\n");
    printf("ss 20 \"set speed to 20\"\n");
    printf("r 180 \"roll stero to 180 degree\"\n");
    
    while(1)
    {
        memset(command, 0, sizeof(command));
        printf("cmd:");
        scanf("%s %d", command, &value);
        if(strcmp("ss", command) == 0)
            speed = value;
        else if(strcmp("cn", command) == 0)
            channel = value;            
        else if(strcmp("af", command) == 0)
            param_angle_from_freq = value;
        else if(strcmp("at", command) == 0)
            param_angle_to_freq = value;
        else if(strcmp("r", command) == 0)
            stero_control(channel, value, speed);
        else if(strcmp("ar", command) == 0)
        {
    	    for(i = 0; i < channel_num; i ++)
            {
                stero_control(i, value, speed);
            }  
        }
            
    }
    return 0;
}
