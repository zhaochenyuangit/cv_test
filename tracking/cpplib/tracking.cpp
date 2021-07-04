#include "tracking.hpp"

ObjectList::ObjectList()
{
    printf("init tracking...\n");
    n_ob = 0;
    ObjectNode *node = new ObjectNode;
    head = node;
    head->ob = NULL;
    head->next = NULL;
    p = head;
    tail = head;
}

ObjectList::~ObjectList()
{
    printf("destroy tracking...");
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
    printf("insert Object\n");
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

bool ObjectList::delete_object(int label)
{
    p = head;
    while (p->next)
    {
        if (p->next->ob->get_index() == label)
        {
            break;
        }
        p = p->next;
    }
    ObjectNode *to_delete = p->next;
    if (to_delete)
    {
        to_delete->ob->~HumanObject();
        p->next = to_delete->next;
        delete to_delete;
        n_ob -= 1;
        return 0;
    }
    printf("Object %d not found\n", label);
    return 1;
}

int ObjectList::get_n_objects()
{
    return n_ob;
}

ObjectNode *ObjectList::get_head_node()
{
    return head;
}

int ObjectList::match_centroid(HumanObject *ob, Blob *blob_list, int n_blobs)
{
    int ob_x, ob_y;
    ob->predict(&ob_x, &ob_y);
    for (int i = 1; i <= n_blobs; i++)
    {
        Blob blob = blob_list[i - 1];
        int blob_x = blob.centroid_index % 71;
        int blob_y = blob.centroid_index / 71;
        int distance = abs(ob_x - blob_x) + abs(ob_y - blob_y);
        if (distance <= 25)
        {
            printf("match object %d with blob %d centroid\n", ob->get_index(), i);
            ob->update(blob_x, blob_y, blob.size);
            return i;
        }
    }
    return 0;
}

int ObjectList::match_centrals(HumanObject *ob, Blob *blob_list, int n_blobs)
{
    printf("try match with centrals\n");
    int ob_x, ob_y;
    ob->predict(&ob_x, &ob_y);
    for (int n = 1; n <= n_blobs; n++)
    {
        printf("central of blob %d\n", n);
        Blob blob = blob_list[n - 1];
        int16_t *central_x = (int16_t *)malloc(sizeof(int16_t) * blob.n_central_points);
        int16_t *central_y = (int16_t *)malloc(sizeof(int16_t) * blob.n_central_points);
        for (int i = 0; i < blob.n_central_points; i++)
        {
            central_x[i] = blob.central_index_list[i] % 71;
            central_y[i] = blob.central_index_list[i] / 71;
        }
        int min_loc = 0;
        for (int i = 1; i < blob.n_central_points; i++)
        {
            int dist_i = abs(central_x[i] - ob_x) + abs(central_y[i] - ob_y);
            int dist_min = abs(central_x[min_loc] - ob_x) + abs(central_y[min_loc] - ob_y);
            if (dist_i < dist_min)
            {
                min_loc = i;
            }
        }
        int dist_min = abs(central_x[min_loc] - ob_x) + abs(central_y[min_loc] - ob_y);
        printf("dist min %d, %d, loc %d %d\n", dist_min, min_loc, central_x[min_loc], central_y[min_loc]);
        if (dist_min < 15)
        {
            printf("match ob %d with blob %d at central %d %d\n", ob->get_index(), n, central_x[min_loc], central_y[min_loc]);
            ob->update(central_x[min_loc], central_y[min_loc], 8 * blob.central_distance_list[min_loc]);
            free(central_x);
            free(central_y);
            return n;
        }
        else
        {
            free(central_x);
            free(central_y);
        }
    }
    return 0;
}

void ObjectList::matching(Blob *blob_list, int n_blobs)
{
    bool *matched_flag = (bool *)malloc(sizeof(bool) * n_blobs);
    for (int i = 0; i < n_blobs; i++)
    {
        matched_flag[i] = false;
    }
    p = head->next;
    while (p)
    {
        int matched_blob_num = 0;
        matched_blob_num = match_centroid(p->ob, blob_list, n_blobs);
        if (matched_blob_num)
        {
            matched_flag[matched_blob_num - 1] = true;
            p = p->next;
            continue;
        }
        matched_blob_num = match_centrals(p->ob, blob_list, n_blobs);
        if (matched_blob_num)
        {
            matched_flag[matched_blob_num - 1] = true;
            p = p->next;
            continue;
        }
        /* if an object is not matched with any blob then delete it*/
        int first_x, now_x, first_y, now_y;
        p->ob->get_shift(&first_x, &first_y, &now_x, &now_y);
        if ((first_y < 35) && (now_y > 35))
        {
            printf("count +1\n");
        }
        else if ((first_y > 35) && (now_y < 35))
        {
            printf("count -1\n");
        }
        int label = p->ob->get_index();
        p = p->next;
        delete_object(label);
    }
    /* spawn new object from unmatched blobs*/
    for (int i = 0; i < n_blobs; i++)
    {
        if (matched_flag[i])
        {
            continue;
        }
        Blob blob = blob_list[i];
        int pos_x, pos_y;
        pos_x = blob.centroid_index % 71;
        pos_y = blob.centroid_index / 71;
        HumanObject *ob = new HumanObject(i + 1, pos_x, pos_y, blob.size);
        append_object(ob);
    }
    free(matched_flag);
}