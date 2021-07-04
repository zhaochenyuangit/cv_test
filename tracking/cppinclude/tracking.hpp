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
    unsigned int n_ob;

public:
    ObjectList();
    ~ObjectList();
    bool append_object(HumanObject *ob);
    bool delete_object(int label);
    int get_n_objects();
    ObjectNode *get_head_node();
    int match_centroid(HumanObject *ob, Blob *blob_list, int n_blobs);
    int match_centrals(HumanObject *ob, Blob *blob_list, int n_blobs);
    void matching(Blob *blob_list, int n_blobs);
};
