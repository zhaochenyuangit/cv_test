#pragma once
extern "C"
{
#include "stdbool.h"
#include "stdio.h"
#include <string.h>
}

class HumanObject
{
private:
    static int serial_num_;
    int label_;
    int size_;
    int last_size_;
    int pos_x_;
    int pos_y_;
    int vel_x_;
    int vel_y_;

public:
    HumanObject(int label, int pos_x, int pos_y, int size);
    ~HumanObject();
    void update(int pos_x, int pos_y, int size);
    void ab_filter(int pos_x, int pos_y);
    void predict(int *ppos_x, int *ppos_y);
};
