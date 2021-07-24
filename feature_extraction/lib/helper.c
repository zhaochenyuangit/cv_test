#include "helper.h"

void print_array_sh(short *array, int width, int height)
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int index = r * width + c;
            if (array[index])
            {
                printf("%d ", array[index]);
            }
            else
            {
                printf("- ");
            }
            /*
            if (array[index])
            {
                printf("* ");
            }
            else
            {
                printf("- ");
            }
            //*/
        }
        printf("\n");
    }
}
void print_array_c(uint8_t *array, int width, int height)
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int index = r * width + c;
            if (array[index])
            {
                printf("%d ", array[index]);
            }
            else
            {
                printf("- ");
            }
        }
        printf("\n");
    }
}
/*0 is start, 1 is end, return eclipsed time in ms*/
double performance_evaluation(int start_or_end)
{
    static struct timeval tik, tok;
    if (start_or_end)
    {
        gettimeofday(&tok, NULL);
        double eclipsed_time_ms = (tok.tv_sec - tik.tv_sec) * 1000 + (tok.tv_usec - tik.tv_usec) / 1000.0;
        return eclipsed_time_ms;
    }
    else
    {
        gettimeofday(&tik, NULL);
        return -1;
    }
}
