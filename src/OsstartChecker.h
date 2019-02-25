#ifndef     OSSTART_CHKER_H
#define     OSSTART_CHKER_H

#include <pthread.h>
#include <termios.h>
#include "ds_DBLList.h"
#include "HWCheck.h"
#include "Animation.h"
#include "ConsoleColor.h"

#define OSSTART_CHK_RETRY_TIMES		3

typedef int OSSTART_CHKER_STATUS;
#define OSSTART_CHKER_STATUS_SUCCESS    0
#define OSSTART_CHKER_STATUS_FAILED     99

typedef struct _OSSTART_CHECKER
{
    char m_policy_dir_root[1024];
    int m_check_retry_times;
    int m_check_passed;	//0:not passed, 1:passed
    int m_display_sys_detail;   //0:not display, 1:display

    //---------Store error messages--------------
    PDBLLIST_NODETYPE m_err_msg_header;
    PDBLLIST_NODETYPE m_err_msg_tail;
    //-------------------------------------------

    //---------Store policy dir path-------------
    PDBLLIST_NODETYPE m_policy_dir_list_header;
    PDBLLIST_NODETYPE m_policy_dir_list_tail;
    //-------------------------------------------
}OSSTART_CHECKER, *POSSTART_CHECKER;

//public functions
OSSTART_CHKER_STATUS InitOsstartChecker(POSSTART_CHECKER poc, char* policy_dir_root);

HWCHK_STATUS ExecOsstartCheck(POSSTART_CHECKER poc);

OSSTART_CHKER_STATUS DestroyOsstartChecker(POSSTART_CHECKER poc);

//private functions
OSSTART_CHKER_STATUS read_policy_dirs(POSSTART_CHECKER poc);

HWCHK_STATUS check_executor(POSSTART_CHECKER poc);

HWCHK_STATUS check_once(POSSTART_CHECKER poc, char* policy_dir_path);

void system_reboot();

int is_f6_pressed(void* param);

void print_err_msg_list(PDBLLIST_NODETYPE header);

static char* get_tty_name();

#endif
