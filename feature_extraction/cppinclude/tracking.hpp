#include "human_object.hpp"
extern "C"
{
#include "feature_extraction.h"
}

struct ObjectNode
{
    HumanObject *ob;
    ObjectNode *next;
};

class ObjectList
{
private:
    ObjectNode *head;
    ObjectNode *tail;
    ObjectNode *p;

public:
    unsigned int n_ob;
    ObjectList();
    ~ObjectList();
    bool append_object(HumanObject *ob);
    bool delete_object(int i);
    int get_n_objects();
};
