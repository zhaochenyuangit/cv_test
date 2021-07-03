#include "tracking.hpp"

ObjectList::ObjectList()
{
    n_ob = 0;
    HumanObject *head_ob = new HumanObject(0, 0, 0, 0);
    head->ob = head_ob;
    head->next = NULL;
    p = head;
    tail = head;
}

ObjectList::~ObjectList()
{
    while (head)
    {
        p = head->next;
        head->ob->~HumanObject();
        delete head;
        head = p;
    }
}

bool ObjectList::append_object(HumanObject *ob)
{
    ObjectNode *node = new ObjectNode;
    if (node == NULL)
    {
        return 1;
    }
    node->ob = ob;
    node->next = NULL;
    tail->next = node;
    n_ob += 1;
    return 0;
}

bool ObjectList::delete_object(int i)
{
    if (i > n_ob)
    {
        return 1;
    }
    p = head;
    for (int index = 0; index < i - 1; index++)
    {
        p = p->next;
    }
    ObjectNode *to_delete = p->next;
    to_delete->ob->~HumanObject();
    p->next = to_delete->next;
    delete to_delete;
    n_ob -= 1;
    return 0;
}

int ObjectList::get_n_objects(){
    return n_ob;
}