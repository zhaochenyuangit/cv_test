#pragma once
#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>

/* use linear index or subscription index? this is a question*/
struct Blob
{
    uint32_t size;
    uint32_t centroid_index;
    uint32_t n_central_points;
    uint32_t *central_index_list;
    uint8_t *central_distance_list;
};
typedef struct Blob Blob;

void chamfer_dt_city(uint8_t *mask, int width, int height);
Blob *extract_feature(uint8_t *labeled, int n_blobs, int img_w, int img_h);
int central_detector(uint8_t *input, uint8_t *output, int const img_w, int const img_h);
void chamfer_distance_transform(uint8_t *mask, int width, int height, int a, int b);

void print_blob_info(Blob *blob_list, int n_blobs);
void print_blob_mask(Blob *blob_list, int n_blobs, int w, int h);
void delete_blob_list(Blob *blob_list, int n_blobs);