// jjy simulator for pigpio (RPi)
// James Lee 
// https://github.com/nsicko42
//
// make : gcc -Wall -o jjy jjy.c -lpigpio -lrt
//
// run : ./jjy &
//
// jjy signal is transmitted with 40kHz carrier frequency via 23 pin if it run successully.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pigpio.h>

#define PWM_OUT     18

#define PWM_BASE_FREQ   192000.0F

#define PWM_CLOCK       ((PWM_BASE_FREQ/PWM_FREQ)/PWM_RANGE)
#define PWM_ON          500000
#define PWM_OFF         0

static int carrier_freq = 40000;
static int quiet = 0;

static int jjy_tick_ms(char type)
{
    return 500 + (type * 300);
}

static void ASK_Off(void)
{
    gpioHardwarePWM(PWM_OUT, carrier_freq, PWM_OFF);
}

static void ASK_On(unsigned char tm_ms)
{
    gpioHardwarePWM(PWM_OUT, carrier_freq, PWM_ON);
    gpioSleep(PI_TIME_RELATIVE, tm_ms / 1000 , (tm_ms % 1000) * 1000);
    ASK_Off();
}

char get_current_jjy_tick(time_t time)
{
    static const unsigned char parity[60] =
    {
        0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0,
        1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1
    };
    struct tm *tm;
    unsigned char leaf = 0;

    if ((tm = localtime (&time)) == NULL)
    {
        return -1;
    }

    tm->tm_year += 1900;
    tm->tm_mon += 1;
    tm->tm_yday += 1;

    switch(tm->tm_sec)
    {
        case 0:  return jjy_tick_ms(-1);
        case 1:  return jjy_tick_ms((tm->tm_min / 10) & 4);
        case 2:  return jjy_tick_ms((tm->tm_min / 10) & 2);
        case 3:  return jjy_tick_ms((tm->tm_min / 10) & 1);
        case 4:  return jjy_tick_ms(0);
        case 5:  return jjy_tick_ms((tm->tm_min % 10) & 8);
        case 6:  return jjy_tick_ms((tm->tm_min % 10) & 4);
        case 7:  return jjy_tick_ms((tm->tm_min % 10) & 2);
        case 8:  return jjy_tick_ms((tm->tm_min % 10) & 1);
        case 9:  return jjy_tick_ms(-1);
        case 10: return jjy_tick_ms(0);
        case 11: return jjy_tick_ms(0);
        case 12: return jjy_tick_ms((tm->tm_hour / 10) & 2);
        case 13: return jjy_tick_ms((tm->tm_hour / 10) & 1);
        case 14: return jjy_tick_ms(0);
        case 15: return jjy_tick_ms((tm->tm_hour % 10) & 8);
        case 16: return jjy_tick_ms((tm->tm_hour % 10) & 4);
        case 17: return jjy_tick_ms((tm->tm_hour % 10) & 2);
        case 18: return jjy_tick_ms((tm->tm_hour % 10) & 1);
        case 19: return jjy_tick_ms(-1);
        case 20: return jjy_tick_ms(0);
        case 21: return jjy_tick_ms(0);
        case 22: return jjy_tick_ms((tm->tm_yday / 100) & 2);
        case 23: return jjy_tick_ms((tm->tm_yday / 100) & 1);
        case 24: return jjy_tick_ms(0);
        case 25: return jjy_tick_ms(((tm->tm_yday / 10) % 10) & 8);
        case 26: return jjy_tick_ms(((tm->tm_yday / 10) % 10) & 4);
        case 27: return jjy_tick_ms(((tm->tm_yday / 10) % 10) & 2);
        case 28: return jjy_tick_ms(((tm->tm_yday / 10) % 10) & 1);
        case 29: return jjy_tick_ms(-1);
        case 30: return jjy_tick_ms((tm->tm_yday  % 10) & 8);
        case 31: return jjy_tick_ms((tm->tm_yday  % 10) & 4);
        case 32: return jjy_tick_ms((tm->tm_yday  % 10) & 2);
        case 33: return jjy_tick_ms((tm->tm_yday  % 10) & 1);
        case 34: return jjy_tick_ms(0);
        case 35: return jjy_tick_ms(0);
        case 36: return jjy_tick_ms(parity[tm->tm_hour]);
        case 37: return jjy_tick_ms(parity[tm->tm_min]);
        case 38: return jjy_tick_ms(0);
        case 39: return jjy_tick_ms(-1);
        case 40: return jjy_tick_ms(0);
        case 41: return jjy_tick_ms((tm->tm_year / 10) & 8);
        case 42: return jjy_tick_ms((tm->tm_year / 10) & 4);
        case 43: return jjy_tick_ms((tm->tm_year / 10) & 2);
        case 44: return jjy_tick_ms((tm->tm_year / 10) & 1);
        case 45: return jjy_tick_ms((tm->tm_year % 10) & 8);
        case 46: return jjy_tick_ms((tm->tm_year % 10) & 4);
        case 47: return jjy_tick_ms((tm->tm_year % 10) & 2);
        case 48: return jjy_tick_ms((tm->tm_year % 10) & 1);
        case 49: return jjy_tick_ms(-1);
        case 50: return jjy_tick_ms(tm->tm_wday & 4);
        case 51: return jjy_tick_ms(tm->tm_wday & 2);
        case 52: return jjy_tick_ms(tm->tm_wday & 1);
        case 53: return jjy_tick_ms(leaf & 2);
        case 54: return jjy_tick_ms(leaf & 1);
        case 55: return jjy_tick_ms(0);
        case 56: return jjy_tick_ms(0);
        case 57: return jjy_tick_ms(0);
        case 58: return jjy_tick_ms(0);
        case 59: return jjy_tick_ms(-1);
    }
    return 0;
}

static void help_print(void)
{
    printf("Usage: jjy [-h46q]\r\n\r\n"
           "Option:\r\n"
           "  -h         Help\r\n"
           "  -4         Carrier Frequency: 40kHz (default)\r\n"
           "  -6         Carrier Frequency: 60kHz\r\n"
           "  -q         Quiet mode\r\n");
}

void exit_handler(void)
{
}

int main( int argc, char **argv )
{
    time_t now, old;

    // pwm init
    if(gpioInitialise() == PI_INIT_FAILED)
    {
        perror(argv[0]);
        exit(-1);
    }

    while ((opt = getopt(argc, argv, "h46q")) != -1)
    {
        switch (opt)
        {
            case '4':
            case '6':
                if (carrier_freq == 0)
                {
                    carrier_freq = (opt-'0')*10000;
                }
                else
                {
                    fprintf(stderr, "Option -4 and -6 are mutually exclusive options.\r\n\r\n");
                    help_print();
                    exit(1);
                }
                break;
            case 'q': quiet = 1; break;
            default:
            case '?': fprintf(stderr, "Unkwon parameter\r\n\r\n");
            case 'h': help_print(); exit(1); break;
        }
    }

    if (carrier_freq == 0)
    {
        carrier_freq = 40000;
        printf("Default carrier frequency: 40kHz\r\n");
    }
    else
    {
        printf("Carrier frequency: %dkHz\r\n", carrier_freq / 1000);
    }

    ASK_Off();
    atexit(exit_handler);

    printf("Press [ENTER] to stop the transmission.\r\n");

    // main loop
    while(1)
    {
        now = time(0);

        if(old != now)
        {
            ASK_On(get_current_jjy_tick(now));
            old = now;
            if (!quiet)
            {
                struct tm * tm = localtime(&now);
                printf("%02d-%02d-%02d %02d:%02d:%02d %dkHz\r\n",
                        tm->tm_year-100, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, carrier_freq/1000);
            }
        }
        else
        {
            fd_set s;
            FD_ZERO(&s);

            fflush(stdin);

            struct timeval  timeout;

            FD_SET(STDIN_FILENO, &s);
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            if (select(STDIN_FILENO+1, &s, NULL, NULL, &timeout) > 0)
            {
                if (FD_ISSET(STDIN_FILENO, &s)) return 0;
            }
        }
    }

    return 0;
}
