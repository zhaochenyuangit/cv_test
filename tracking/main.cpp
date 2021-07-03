extern "C"
{
#include "stdio.h"
#include <stdbool.h>
#include <string.h>

#include "helper.h"
#include "detect.h"
#include "feature_extraction.h"
}
#include "human_object.hpp"
#include "tracking.hpp"

enum
{
    IM_W = 71
};
enum
{
    IM_H = 71
};
enum
{
    IM_LEN = IM_W * IM_H
};

short im[IM_LEN];
uint8_t mask[IM_LEN];
static short holder1[IM_LEN];
static short holder2[IM_LEN];

extern short dummy[];
extern short two[];

int blob_detection(short *raw, uint8_t *result)
{
    interpolation71x71(raw, im);
    image_copy(im, holder1, IM_LEN);
    average_filter(holder1, IM_W, IM_H, 35);
    grayscale_thresholding(im, holder2, IM_LEN, holder1, 0);
    int max = max_of_array(holder2, IM_LEN);
    int std = std_of_array(holder2, IM_LEN);
    short th = max - 2 * std;
    binary_thresholding(holder2, result, IM_LEN, &th, 1);
    binary_fill_holes(result, IM_W, IM_H);
    /*for (int i = 0; i < IM_LEN; i++)
    {
        im[i] = im[i] * result[i];
    }
    average_filter(im, IM_W, IM_H, 7);
    binary_thresholding(im, result, IM_LEN, &th, 1);*/
    int num = labeling8(result, IM_W, IM_H);
    return num;
}

int main(void)
{
    performance_evaluation(0);
    int n_blobs = blob_detection(two, mask);
    printf("detect: %.2f ms\n", performance_evaluation(1));
    printf("detected %d blobs\n", n_blobs);
    performance_evaluation(0);
    Blob *blobs = extract_feature(mask, n_blobs, IM_W, IM_H);
    printf("feature extract: %.2f ms\n", performance_evaluation(1));

    //print_blob_mask(blobs,n_blobs,holder1,IM_W,IM_H);
    print_blob_info(blobs, n_blobs);

    HumanObject obj1(1,30,30,200);
    obj1.update(50,20,300);
    int x,y;
    obj1.predict(&x,&y);
    printf("new pos: %d,%d\n",x,y);

    ObjectList tracking;
    tracking.append_object(&obj1);
    printf("n objects: %d\n",tracking.get_n_objects());

    //print_array_c(mask, IM_W, IM_H);
    return 0;
}