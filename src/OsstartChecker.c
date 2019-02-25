#include "OsstartChecker.h"

OSSTART_CHKER_STATUS InitOsstartChecker(POSSTART_CHECKER poc, char* policy_dir_root)
{
    poc->m_policy_dir_list_header = poc->m_policy_dir_list_tail = NULL;
    
    //initialize OSSTART_CHECKER members
    memset(poc->m_policy_dir_root, 0, 1024);
    dbll_InitList(&poc->m_policy_dir_list_header, &poc->m_policy_dir_list_tail);
    dbll_InitList(&poc->m_err_msg_header, &poc->m_err_msg_tail);
    poc->m_check_retry_times = 0;
    poc->m_check_passed = 0;
    poc->m_display_sys_detail = 0;

    if(strlen(policy_dir_root) >= 512)
    {
        printf("Too long policy directory root path. Expected less than 512 bytes\n");
        return OSSTART_CHKER_STATUS_FAILED;
    }
    strcpy(poc->m_policy_dir_root, policy_dir_root);
    //if the last character of m_policy_dir_root is lack of "/", add it
    if(poc->m_policy_dir_root[strlen(poc->m_policy_dir_root) - 1] != '/')
    {
        poc->m_policy_dir_root[strlen(poc->m_policy_dir_root)] = '/';
    }

    //read policy directories to policy dir list
    read_policy_dirs(poc);

    return OSSTART_CHKER_STATUS_SUCCESS;
}

HWCHK_STATUS ExecOsstartCheck(POSSTART_CHECKER poc)
{
    int i=0;
    HWCHK_STATUS checkRes = HWCHK_STATUS_SUCCESS;

    if(poc == NULL)
        return HWCHK_STATUS_FAILED;

    checkRes = check_executor(poc);

    if(checkRes != HWCHK_STATUS_FAILED)
        return checkRes;

    //If check failed, retry 3 times
    for(i=0; i<OSSTART_CHK_RETRY_TIMES; i++)
    {
        Console_SetTextColor(CONSOLECOLOR_WHITE, CONSOLECOLOR_BG_BLACK);
        printf("All policy configurations checked failed.\nRetry %d time...\n", i+1);
        printf("One of the following reasons may cause check failure.\n");
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        dbll_PrintList(poc->m_err_msg_header, print_err_msg_list);
        Console_SetTextColor(CONSOLECOLOR_WHITE, CONSOLECOLOR_BG_BLACK);
        printf("Please remove invalid devices or insert proper ones, and wait for next examination.\n");
        RotatePrint(15, ".", SOW_OFF);

		poc->m_display_sys_detail = 0;
        checkRes = check_executor(poc);
        poc->m_check_retry_times++;
        if(checkRes == HWCHK_STATUS_SUCCESS)
            return HWCHK_STATUS_SUCCESS;
    }

    return HWCHK_STATUS_FAILED;
}

OSSTART_CHKER_STATUS DestroyOsstartChecker(POSSTART_CHECKER poc)
{
    if(poc == NULL)
        return OSSTART_CHKER_STATUS_FAILED;

    memset(poc->m_policy_dir_root, 0, 1024);
    poc->m_check_retry_times = 0;
    poc->m_check_passed = 0;
    poc->m_display_sys_detail = 0;

    if(poc->m_policy_dir_list_header != NULL)
        dbll_DestoryList(&(poc->m_policy_dir_list_header));

    if(poc->m_err_msg_header != NULL)
        dbll_DestoryList(&(poc->m_err_msg_header));

    return OSSTART_CHKER_STATUS_SUCCESS;
}

OSSTART_CHKER_STATUS read_policy_dirs(POSSTART_CHECKER poc)
{
    struct dirent* ent = NULL;
    DIR *pDir = NULL;
    PDBLLIST_NODETYPE node = NULL;
    char tmp_path[1024]={0};

    pDir = opendir(poc->m_policy_dir_root);
    while(NULL != (ent=readdir(pDir)))
    {
        if((int)ent->d_type == 4 && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)   //directory
        {
            if(strlen(ent->d_name) > 200)
            {
                printf("Too long policy dir name. Expected not larger than 200 bytes.\n");
                return OSSTART_CHKER_STATUS_FAILED;
            }
            //make a policy dir
            sprintf(tmp_path, "%s%s%s", poc->m_policy_dir_root, ent->d_name, "/");
            //make a list node
            node = NULL;
            if(dbll_CreateNode(&node, tmp_path, strlen(tmp_path) * sizeof(char)) != DBLL_STATUS_SUCCESS)
            {
                printf("Create a node containing policy dir failed.\n");
                return OSSTART_CHKER_STATUS_FAILED;
            }
            //append node to policy dir list
            if(dbll_AppendNode(poc->m_policy_dir_list_header, node) != DBLL_STATUS_SUCCESS)
            {
                printf("Append node to policy dir list failed.\n");
                return OSSTART_CHKER_STATUS_FAILED;
            }
        }
    }//while

    return OSSTART_CHKER_STATUS_SUCCESS;
}

HWCHK_STATUS check_executor(POSSTART_CHECKER poc)
{
    PDBLLIST_NODETYPE node = NULL;
    HWCHK_STATUS checkRes = HWCHK_STATUS_SUCCESS;

    //No policy set
    if(poc->m_policy_dir_list_header->DATA_AREA.length == 0)
        return HWCHK_STATUS_NO_POLICY_SET;

    //Truncate error message list
    if(poc->m_err_msg_header != NULL)
        dbll_DestoryList(&(poc->m_err_msg_header));
    dbll_InitList(&poc->m_err_msg_header, &poc->m_err_msg_tail);
    
    node = poc->m_policy_dir_list_header->next;
    while(node->nodetype != NTYPE_TAIL)
    {
        //Get Hardware Information
        GetHwInfo();

        checkRes = check_once(poc, (char*)node->DATA_AREA.data);
        if(checkRes == HWCHK_STATUS_SUCCESS)
        {
            poc->m_check_passed = 1;
            return HWCHK_STATUS_SUCCESS;
        }
        
        if(node->next->nodetype != NTYPE_TAIL)
        {
            Console_SetTextColor(CONSOLECOLOR_CYAN, CONSOLECOLOR_BG_BLACK);
            printf("Check Failed, Now Trying Next Policy Configuration...\n");
            RotatePrint(3, "", SOW_ON);
        }

        //Destroy hardware info
        FreeHwInfo();
        node = node->next;
    }

    poc->m_check_passed = 0;
    return checkRes;
}

HWCHK_STATUS check_once(POSSTART_CHECKER poc, char* policy_dir_path)
{
    int CheckVal = HWCHK_STATUS_SUCCESS;
    PDBLLIST_NODETYPE node = NULL;
    char buf[1024] = {0};

    //Waiting for pressing F6
    //backup terminal configuration
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);

    pthread_t waitf6;
    pthread_create(&waitf6, NULL, (void*)&(is_f6_pressed), (void*)poc);
    Console_SetTextColor(CONSOLECOLOR_WHITE, CONSOLECOLOR_BG_BLACK);
    printf("\nInitalizing");
    RotatePrint(3, ".", SOW_OFF);
    pthread_cancel(waitf6);
    pthread_join(waitf6, NULL);
    //restore terminal configuration
    tcsetattr(0,TCSANOW,&stored_settings);

    //////////////Check PC Info/////////////////
    Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
    printf("\nCheck PC Information: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        print_pc_info((PPC_INFO)(__HWLIST__[HWPRT_PC]));
    //Check PC
    if((CheckVal = CheckPC(policy_dir_path)) == HWCHK_STATUS_IGNORE_PC)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[PC Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[PC Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_PC)
            strcpy(buf, "Invalid PC information. Consider change a mother board.");
        else
            strcpy(buf, "Lack of proper PC information. Consider change a correct mother board.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);
        
        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[PC Check OK]\n");
    }

    //////////////Check BIOS Info/////////////////
    Console_SetTextColor(CONSOLECOLOR_WHITE, CONSOLECOLOR_BG_BLACK);
    printf("Check BIOS: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        print_bios_info((PBIOS_INFO)(__HWLIST__[HWPRT_BIOS]));
    //Check BIOS
    if((CheckVal = CheckBIOS(policy_dir_path)) == HWCHK_STATUS_IGNORE_BIOS)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[BIOS Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[BIOS Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_BIOS)
            strcpy(buf, "Invalid BIOS information. Consider change a mother board.");
        else
            strcpy(buf, "Lack of proper BIOS information. Consider change a correct mother board.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[BIOS Check OK]\n");
    }

    //////////////Check Mother board Info/////////////////
    Console_SetTextColor(CONSOLECOLOR_CYAN, CONSOLECOLOR_BG_BLACK);
    printf("Check Mother Board Information: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        print_mb_info((PMOTHER_BOARD_INFO)(__HWLIST__[HWPRT_MTHRBRD]));
    //Check MB
    if((CheckVal = CheckMB(policy_dir_path)) == HWCHK_STATUS_IGNORE_MB)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[Mother Board Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[Mother Board Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_MB)
            strcpy(buf, "Invalid mother board information. Consider change a mother board.");
        else
            strcpy(buf, "Lack of proper mother board information. Consider change a correct mother board.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);
        
        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[Mother Board Check OK]\n");
    }

    //////////////Check CPU Info/////////////////
    Console_SetTextColor(CONSOLECOLOR_BULE, CONSOLECOLOR_BG_BLACK);
    printf("Check CPU: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        print_cpu_info((PCPU_INFO)(__HWLIST__[HWPRT_CPU]));
    //Check CPU
    if((CheckVal = CheckCPU(policy_dir_path)) == HWCHK_STATUS_IGNORE_CPU)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[CPU Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[CPU Check Failed]\n");
        
        if(CheckVal == HWCHK_STATUS_WRONG_CPU)
            strcpy(buf, "Invalid CPU. Consider change a CPU.");
        else
            strcpy(buf, "Lack of proper CPU. Consider change a correct one.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[CPU Check OK]\n");
    }

    //////////////Check Memory Info///////////////
    Console_SetTextColor(CONSOLECOLOR_PURPLE, CONSOLECOLOR_BG_BLACK);
    printf("Check Memory: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        mem_info_list_walk();
    //Check Memory
    if((CheckVal = CheckMem(policy_dir_path)) == HWCHK_STATUS_IGNORE_MEM)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[Memory Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[Memory Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_MEM)
            strcpy(buf, "Invalid memory. Consider remove the incorrect one.");
        else
            strcpy(buf, "Lack of proper memory. Consider plugin a correct one.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
        printf("[Memory Check OK]\n");
    }

    //////////////Check HDD Info///////////////
    Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
    printf("Check HDD: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        print_hdd_info((PHDD_INFO)(__HWLIST__[HWPRT_HDD]));
    //Check HDD
    if((CheckVal = CheckHDD(policy_dir_path)) == HWCHK_STATUS_IGNORE_HDD)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[Harddisk Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[HDD Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_HDD)
            strcpy(buf, "Invalid harddisk. Consider remove the incorrect one.");
        else
            strcpy(buf, "Lack of proper harddisk. Consider plugin a correct one.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[HDD Check OK]\n");
    }

    ///////////////Check NIC Info///////////////////
    Console_SetTextColor(CONSOLECOLOR_PURPLE, CONSOLECOLOR_BG_BLACK);
    printf("Check NIC Device: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        nic_list_walk();
    //Check NIC
    if((CheckVal = CheckNIC(policy_dir_path)) == HWCHK_STATUS_IGNORE_NIC)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[NIC Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[HDD Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_NIC)
            strcpy(buf, "Invalid NIC. Consider remove the incorrect one.");
        else
            strcpy(buf, "Lack of proper NIC. Consider plugin a correct one.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[NIC Check OK]\n");
    }

    ///////////////Check USB Info///////////////////
    ////Renew USB device info list////
    if(get_udi_list_header()->DATA_AREA.length > 0)
        DestroyUDIList();

    ___UDI_LIST_HEADER___ = NULL;
    ___UDI_LIST_TAIL___ = NULL;

    InitUDIList();
    CreateUDIList();
    __HWLIST__[HWPRT_USB] = get_udi_list_header();
    //////////////////////////////////
    Console_SetTextColor(CONSOLECOLOR_CYAN, CONSOLECOLOR_BG_BLACK);
    printf("Check USB Device: ");
    RotatePrint(1, ".", SOW_OFF);
    if(poc->m_display_sys_detail)
        udi_list_walk(0);
    if((CheckVal = CheckUSB(policy_dir_path)) == HWCHK_STATUS_IGNORE_USB)
    {
        Console_SetTextColor(CONSOLECOLOR_YELLOW, CONSOLECOLOR_BG_BLACK);
	printf("[USB Check Not Set]\n");
    }
    else if(CheckVal != HWCHK_STATUS_SUCCESS)
    {
        Console_SetTextColor(CONSOLECOLOR_RED, CONSOLECOLOR_BG_BLACK);
        printf("[USB Check Failed]\n");

        if(CheckVal == HWCHK_STATUS_WRONG_USB)
            strcpy(buf, "Invalid USB device. Consider remove the incorrect one.");
        else
            strcpy(buf, "Lack of proper USB device. Consider plugin a correct one.");
        dbll_CreateNode(&node, buf, strlen(buf));
        dbll_AppendNode(poc->m_err_msg_header, node);

        return HWCHK_STATUS_FAILED;
    }
    else
    {
        Console_SetTextColor(CONSOLECOLOR_GREEN, CONSOLECOLOR_BG_BLACK);
	printf("[USB Check OK]\n");
    }
    
    return HWCHK_STATUS_SUCCESS;
}

void print_err_msg_list(PDBLLIST_NODETYPE header)
{
    PDBLLIST_NODETYPE node = NULL;

    if(header == NULL)
        return;

    node = header->next;
    while(node->nodetype != NTYPE_TAIL)
    {
        printf("ERROR MSG: %s\n", (char*)node->DATA_AREA.data);
        node = node->next;
    }
}

void system_reboot()
{
	printf("System Rebooting...");
	RotatePrint(5, "", SOW_ON);
	printf("\nReboot!\n");
	//system("reboot");
}

int is_f6_pressed(void* param)
{
	POSSTART_CHECKER poc = (POSSTART_CHECKER)param;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	FILE *fp_tty;
	int c = 0;
	int i = 0;
	char keybuf[10] = {0};

	fp_tty = fopen(get_tty_name(), "r");
	if(fp_tty == NULL)
		return 0;	//Error occured

	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_lflag &=~ECHO;
	tcgetattr(0,&stored_settings);	//backup raw settings
	tcsetattr(fileno(stdin),TCSAFLUSH,&new_settings);

	while((c=getc(fp_tty)))
	{
		if(c == 27)		//Function Key
		{
			//look for F6(27 91 49 53 126)
			c = getc(fp_tty);
			keybuf[i++]=c;
			c = getc(fp_tty);
			keybuf[i++]=c;
			if(keybuf[0] != 91 || keybuf[1] != 49)
			{
				i=0;
				memset(keybuf, 0, 10);
				continue;
			}
			c = getc(fp_tty);
			if(c==55)	//F6 key
			{
				i=0;
				memset(keybuf, 0, 10);
				tcsetattr(0,TCSANOW,&stored_settings);
				poc->m_display_sys_detail = 1;
				return 1;	//F6 Pressed
			}
		}

		i=0;
		memset(keybuf, 0, 10);
		pthread_testcancel();
	}

	tcsetattr(0,TCSANOW,&stored_settings);
	return 0;
}

static char* get_tty_name()
{
	FILE* fp = NULL;
	char buffer[512] = {0};
	char* ptr = NULL;
	static char retBuf[512] = {0};
	
	fp = popen("tty", "r");
	fread(buffer, sizeof(char), sizeof(buffer), fp);
	pclose(fp);
	
	if((ptr = strstr(buffer, "tty")) == NULL)
	{
		strcpy(retBuf, "/dev/tty");
	}
	else
	{
		RTrim(buffer, strlen(buffer), retBuf);
	}
	
	return retBuf;
}
