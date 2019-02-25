#include "OsstartChecker.h"

void print_policy_dir_list(PDBLLIST_NODETYPE header)
{
    PDBLLIST_NODETYPE node = header->next;

    while(node->nodetype != NTYPE_TAIL)
    {
        printf("%s\n", (char*)node->DATA_AREA.data);
        node = node->next;
    }
}

int main()
{
    OSSTART_CHECKER oc;
        
    InitOsstartChecker(&oc, "/root/hw");

    if(ExecOsstartCheck(&oc) == HWCHK_STATUS_SUCCESS)
        printf("Check successfully.\n");
    else
        printf("Check failed.\n");
    DestroyOsstartChecker(&oc);

    return 0;
}
