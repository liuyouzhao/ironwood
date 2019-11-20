#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#define M1_A 21
#define M1_B 22
#define M2_A 23
#define M2_B 24

static void trapsigterm(int signum)
{
    printf("trapsigterm\n");
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);

    exit(0);
}

static void trapsigquit(int signum)
{
    printf("trapsigquit\n");
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);

    exit(0);
}

static void on_stop(int signum)
{
    printf("on_stop\n");
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);

    exit(0);
}

static void __test()
{
    while(1)
    {
        digitalWrite(M1_A, HIGH);
        digitalWrite(M1_B, LOW);
        digitalWrite(M2_A, HIGH);
        digitalWrite(M2_B, LOW);
//        usleep(1000);
    }
}

int main(void)
{

    pid_t pid = getpid();
    struct sched_param sparam;
    sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(pid, SCHED_RR, &sparam);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam);    

    signal(SIGTERM, trapsigterm);
    signal(SIGQUIT, trapsigquit);
    signal(SIGINT, on_stop); 

    wiringPiSetup();
    pinMode (M1_A, OUTPUT);
    pinMode (M1_B, OUTPUT); 
    pinMode (M2_A, OUTPUT); 
    pinMode (M2_B, OUTPUT); 
	
    __test();

    return 0;
}
