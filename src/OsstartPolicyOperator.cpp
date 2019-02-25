#include <iostream>

#include "OsstartPolicyOperator.h"

int OsstartPolicyOperator::m_policy_pos_array[] = {
	PLCY_POS_PC,
    PLCY_POS_MB,
    PLCY_POS_BIOS,
    PLCY_POS_CPU,
    PLCY_POS_HDD,
    PLCY_POS_MEM,
    PLCY_POS_NIC,
    PLCY_POS_USB,
    PLCY_POS_KERNEL,
    PLCY_POS_GCC,
    PLCY_POS_BASH,
    PLCY_POS_OPENOFFICE,
    PLCY_POS_FIREFOX
};

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1: Policy string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
OsstartPolicyOperator::OsstartPolicyOperator(std::string policy_str, std::string hash_string)
{
    //Do some initializations
    this->m_policy_str = policy_str;
    this->m_hash_string = hash_string;
    //std::cout<<this->m_policy_str<<std::endl;
    m_policy_usb_ptr = 0; m_policy_usb_current_ptr = 0;
    m_policy_mem_ptr = 0; m_policy_mem_current_ptr = 0;
    m_policy_nic_ptr = 0; m_policy_nic_current_ptr = 0;
    m_policy_undef_ptr = 1;

    m_policy_pc = 0;
    m_policy_mb = 0;
    m_policy_bios = 0;
    m_policy_cpu = 0;
    m_policy_hdd = 0;
    
    for(int i=0; i<10; i++)
    {
        m_policy_mem[i]=0;
        m_policy_nic[i]=0;
    }
    for(int i=0; i<20; i++)
        m_policy_usb[i]=0;

    m_policy_kernel = 0;
    m_policy_gcc = 0;
    m_policy_bash = 0;
    m_policy_openoffice = 0;
    m_policy_firefox = 0;

    //Extract the policy value of each item
    for(int i=0; i<CHECK_ITEM_NUM; i++)
    {
        extract_hwsw_policy(m_policy_pos_array[i]);
    }

    this->hw_info_collector();

    /*
    printf("pc: %s\n", this->m_pc_hash_value.c_str());
    printf("mb: %s\n", this->m_mb_hash_value.c_str());
    printf("bios: %s\n", this->m_bios_hash_value.c_str());
    printf("cpu: %s\n", this->m_cpu_hash_value.c_str());
    printf("hdd: %s\n", this->m_hdd_hash_value.c_str());
    for(int i=0; i < this->m_policy_mem_ptr; i++)
        printf("mem%d: %s\n", i+1, this->m_mem_hash_list[i].c_str());
        //std::cout<<"mem"<<i+1<<": "<<this->m_mem_hash_list[i]<<std::endl;
    for(int i=0; i < this->m_policy_nic_ptr; i++)
        printf("nic%d: %s\n", i+1, this->m_nic_hash_list[i].c_str());
    for(int i=0; i < this->m_policy_usb_ptr; i++)
        printf("usb%d: %s\n", i+1, this->m_usb_hash_list[i].c_str());
     */
}

/*****************************************************
*Function: Apply the osstart policy set by server.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
int OsstartPolicyOperator::ApplyPolicy()
{
    char* policy_dir_name = NULL;
    std::string policy_dir_full_path(POLICY_DIR_ROOT);
    int policy_val = PLCY_CHKTYPE_IGNORE;
    std::string eff_policy_str(m_policy_str.substr(0,58));
    char buffer[1024]={0};
    int ret = APP_PLCY_STATUS_SUCCESS;

    if(policy_dir_full_path[policy_dir_full_path.length() - 1] != '/')
        policy_dir_full_path.append("/");
    
    strcpy(buffer, eff_policy_str.c_str());
    policy_dir_name = (char*)MDString(buffer);

    policy_dir_full_path.append(policy_dir_name);
    
    memset(buffer, 0, 1024);
    sprintf(buffer, "%s%s", "mkdir ", policy_dir_full_path.c_str());
    system(buffer);

    //apply pc policy
    if((ret = this->apply_policy_by_check_value(PLCY_TYPE_PC, policy_dir_full_path, this->m_policy_pc)) != APP_PLCY_STATUS_SUCCESS)
        return ret;
    //apply mb policy
    if((ret = this->apply_policy_by_check_value(PLCY_TYPE_MB, policy_dir_full_path, this->m_policy_mb)) != APP_PLCY_STATUS_SUCCESS)
        return ret;
    //apply bios policy
    if((ret = this->apply_policy_by_check_value(PLCY_TYPE_BIOS, policy_dir_full_path, this->m_policy_bios)) != APP_PLCY_STATUS_SUCCESS)
        return ret;
    //apply cpu policy
    if((ret = this->apply_policy_by_check_value(PLCY_TYPE_CPU, policy_dir_full_path, this->m_policy_cpu)) != APP_PLCY_STATUS_SUCCESS)
        return ret;
    //apply hdd policy
    if((ret = this->apply_policy_by_check_value(PLCY_TYPE_HDD, policy_dir_full_path, this->m_policy_hdd)) != APP_PLCY_STATUS_SUCCESS)
        return ret;
    //apply mem policy
    for(int i=0; i<this->m_policy_mem_ptr; i++)
        if((ret = this->apply_policy_by_check_value(PLCY_TYPE_MEM, policy_dir_full_path, this->m_policy_mem[i])) != APP_PLCY_STATUS_SUCCESS)
            return ret;
    //apply nic policy
    for(int i=0; i<this->m_policy_nic_ptr; i++)
        if((ret = this->apply_policy_by_check_value(PLCY_TYPE_NIC, policy_dir_full_path, this->m_policy_nic[i])) != APP_PLCY_STATUS_SUCCESS)
            return ret;
    //apply usb policy
    for(int i=0; i<this->m_policy_usb_ptr; i++)
        if((ret = this->apply_policy_by_check_value(PLCY_TYPE_USB, policy_dir_full_path, this->m_policy_usb[i])) != APP_PLCY_STATUS_SUCCESS)
            return ret;

    return ret;
}

int OsstartPolicyOperator::apply_policy_by_check_value(int item_type, std::string dir_path, int check_value)
{
    std::string file_name(dir_path);
    char buf[32]={0};
    FILE* fp = NULL;

    if(check_value == PLCY_CHKTYPE_IGNORE)
        return APP_PLCY_STATUS_SUCCESS;

    file_name.append("/");

    //composite file name
    switch(item_type)
    {
        case PLCY_TYPE_PC:
            file_name.append("pcid");
            break;
        case PLCY_TYPE_MB:
            file_name.append("mbid");
            break;
        case PLCY_TYPE_BIOS:
            file_name.append("biosid");
            break;
        case PLCY_TYPE_CPU:
            file_name.append("cpuid");
            break;
        case PLCY_TYPE_HDD:
            file_name.append("hddid");
            break;
        case PLCY_TYPE_MEM:
            file_name.append("memid");
            sprintf(buf, "%d", m_policy_mem_current_ptr + 1);
            file_name.append(buf);
            m_policy_mem_current_ptr++;
            break;
        case PLCY_TYPE_NIC:
            file_name.append("nicid");
            sprintf(buf, "%d", m_policy_nic_current_ptr + 1);
            file_name.append(buf);
            m_policy_nic_current_ptr++;
            break;
        case PLCY_TYPE_USB:
            file_name.append("usbid");
            sprintf(buf, "%d", m_policy_usb_current_ptr + 1);
            file_name.append(buf);
            m_policy_usb_current_ptr++;
            break;
        case PLCY_TYPE_KERNEL:
            break;
        case PLCY_TYPE_GCC:
            break;
        case PLCY_TYPE_BASH:
            break;
        case PLCY_TYPE_OPENOFFICE:
            break;
        case PLCY_TYPE_FIREFOX:
            break;
        default:
            file_name.append("otherid");
            sprintf(buf, "%d", m_policy_undef_ptr);
            file_name.append(buf);
            m_policy_undef_ptr++;
            break;
    }
    
    //decide whether NOT is to be enabled based on check_value
    if(check_value == PLCY_CHKTYPE_CHECKNOT)
    {
        file_name.append("_NOT");
    }
    //write file
    fp = fopen(file_name.c_str(), "w");
    if(fp == NULL)
        return APP_PLCY_STATUS_FILEOPENFAILED;

    fprintf(fp, "%s\r\n", this->get_hash_value_by_item_type(item_type).c_str());
    fclose(fp);

    return APP_PLCY_STATUS_SUCCESS;
}

std::string OsstartPolicyOperator::get_hash_value_by_item_type(int item_type)
{
    switch(item_type)
    {
        case PLCY_TYPE_PC:
            return this->m_pc_hash_value;
        case PLCY_TYPE_MB:
            return this->m_mb_hash_value;
        case PLCY_TYPE_BIOS:
            return this->m_bios_hash_value;
        case PLCY_TYPE_CPU:
            return this->m_cpu_hash_value;
        case PLCY_TYPE_HDD:
            return this->m_hdd_hash_value;
        case PLCY_TYPE_MEM:
            return this->m_mem_hash_list[this->m_policy_mem_current_ptr - 1];
        case PLCY_TYPE_NIC:
            return this->m_nic_hash_list[this->m_policy_nic_current_ptr - 1];
        case PLCY_TYPE_USB:
            return this->m_usb_hash_list[this->m_policy_usb_current_ptr - 1];
        case PLCY_TYPE_KERNEL:
        case PLCY_TYPE_GCC:
        case PLCY_TYPE_BASH:
        case PLCY_TYPE_OPENOFFICE:
        case PLCY_TYPE_FIREFOX:
        default:
            return "undefined";
    }
}

void OsstartPolicyOperator::hw_info_collector()
{
    InitHwInfo();
    GetHwInfo();

    //CPU info
    this->m_cpu_hash_value.assign(((PCPU_INFO)(__HWLIST__[HWPRT_CPU]))->md5_hash);

    //Memory, NIC & USB info
    std::vector<std::string> vct_tmp = ExString::esTokenize(this->m_hash_string, "$");
    this->m_mem_hash_list = vct_tmp[0]!="0" ? ExString::esTokenize(vct_tmp[0], "|") : this->m_mem_hash_list;
    this->m_nic_hash_list = vct_tmp[1]!="0" ? ExString::esTokenize(vct_tmp[1], "|") : this->m_nic_hash_list;
    this->m_usb_hash_list = vct_tmp[2]!="0" ? ExString::esTokenize(vct_tmp[2], "|") : this->m_usb_hash_list;

    //Harddisk info
    this->m_hdd_hash_value.assign(((PHDD_INFO)(__HWLIST__[HWPRT_HDD]))->md5_hash);

    //Motherboard info
    this->m_mb_hash_value.assign(((PMOTHER_BOARD_INFO)(__HWLIST__[HWPRT_MTHRBRD]))->md5_hash);
    //BIOS info
    this->m_bios_hash_value.assign(((PBIOS_INFO)(__HWLIST__[HWPRT_BIOS]))->md5_hash);
    //PC info
    this->m_pc_hash_value.assign(((PPC_INFO)(__HWLIST__[HWPRT_PC]))->md5_hash);

    FreeHwInfo();
}

/*****************************************************
*Function: Delete files in specific directory.
*Parameters:
****Param 1: Directory path.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void OsstartPolicyOperator::delete_files_in_specific_dir(std::string path)
{
    /*
    //Make sure the path exists
    if(access(path.c_str(), F_OK) < 0)
        return;

    //Delete all files in path
    char cmd[256]={0};
    sprintf(cmd, "rm -rf %s/*", path.c_str());
    system(cmd);
     * */
    char cmd[256]={0};
    sprintf(cmd, "rm -rf %s", path.c_str());
    system(cmd);
}

bool OsstartPolicyOperator::is_dir_exists(std::string dir_path)
{
    DIR *pDir = NULL;

    pDir = opendir(dir_path.c_str());
    if(pDir != NULL)
        return true;

    return false;
}

/*****************************************************
*Function: Get hw/sw policy value at specific position in policy string.
*Parameters:
****Param 1: Position in policy string.
*Return: Policy value.
*Remark: Nothing.
*****************************************************/
void OsstartPolicyOperator::extract_hwsw_policy(int policy_pos)
{
    int policy_val = 0;

    switch(policy_pos)
    {
        case PLCY_POS_PC:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_pc = policy_val;
            break;
        case PLCY_POS_BIOS:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_bios = policy_val;
            break;
        case PLCY_POS_MB:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_mb = policy_val;
            break;
        case PLCY_POS_CPU:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_cpu = policy_val;
            break;
        case PLCY_POS_HDD:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_hdd = policy_val;
            break;
        case PLCY_POS_KERNEL:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_kernel = policy_val;
            break;
        case PLCY_POS_GCC:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_gcc = policy_val;
            break;
        case PLCY_POS_BASH:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_bash = policy_val;
            break;
        case PLCY_POS_OPENOFFICE:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_openoffice = policy_val;
            break;
        case PLCY_POS_FIREFOX:
            policy_val = atoi(m_policy_str.substr(policy_pos, 1).c_str());
            m_policy_firefox = policy_val;
            break;
        case PLCY_POS_MEM:
            for(int i=0; i<PLCY_POS_MELLEN; i++)
            {
                if(m_policy_str.substr(PLCY_POS_MEM + i, 1) == "0")
                    continue;
                this->m_policy_mem[i] = atoi(m_policy_str.substr(PLCY_POS_MEM + i, 1).c_str());
                this->m_policy_mem_ptr++;
            }
            break;
        case PLCY_POS_USB:
            for(int i=0; i<PLCY_POS_USBLEN; i++)
            {
                if(m_policy_str.substr(PLCY_POS_USB + i, 1) == "0")
                    continue;
                this->m_policy_usb[i] = atoi(m_policy_str.substr(PLCY_POS_USB + i, 1).c_str());
                this->m_policy_usb_ptr++;
            }
            break;
        case PLCY_POS_NIC:
            for(int i=0; i<PLCY_POS_NICLEN; i++)
            {
                if(m_policy_str.substr(PLCY_POS_NIC + i, 1) == "0")
                    continue;
                this->m_policy_nic[i] = atoi(m_policy_str.substr(PLCY_POS_NIC + i, 1).c_str());
                this->m_policy_nic_ptr++;
            }
            break;
            break;
        default:
            break;
    }

    return;
}
