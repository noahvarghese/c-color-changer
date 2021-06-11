#include "color_node.h"

bool rgba_is_equal(int *origin, int *compare)
{

    if (
        origin[0] == compare[0] &&
        origin[1] == compare[1] &&
        origin[2] == compare[2] &&
        origin[3] == compare[3])
    {
        return true;
    }

    return false;
}