//This class is used to implement "the reverse policy"
//Generating the files containing dev hash value in /root/hw
#ifndef OSSTARTPOLICYOP_H
#define OSSTARTPOLICYOP_H

#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "ExString.h"
#include "PolicyOperator.h"
#include "OSSTART_CONFIG.h"

extern "C"
{
    #include "md5lib.h"
    #include "hdinfo.h"
    #include "cpuinfo.h"
    #include "usbinfo.h"
    #include "nicinfo.h"
    #include "dmi_info.h"
    #include "HWInfo.h"
}

#define APP_PLCY_STATUS_SUCCESS             0
#define APP_PLCY_STATUS_FILEOPENFAILED    1
#define APP_PLCY_STATUS_UNDEFERR            99

class OsstartPolicyOperator
{
public:
    OsstartPolicyOperator(std::string policy_str, std::string hash_string);
    int ApplyPolicy();
private:
    int apply_policy_by_check_value(int item_type, std::string dir_path, int check_value);
    std::string get_hash_value_by_item_type(int item_type);
    void hw_info_collector();
    void delete_files_in_specific_dir(std::string path);
    bool is_dir_exists(std::string dir_path);
    void extract_hwsw_policy(int policy_pos);
private:
    std::string m_policy_str;
    std::string m_hash_string;
    int m_policy_pc;
    int m_policy_mb;
    int m_policy_bios;
    int m_policy_cpu;
    int m_policy_hdd;
    int m_policy_usb[20]; int m_policy_usb_ptr; int m_policy_usb_current_ptr;
    int m_policy_mem[10]; int m_policy_mem_ptr; int m_policy_mem_current_ptr;
    int m_policy_nic[10]; int m_policy_nic_ptr; int m_policy_nic_current_ptr;
    int m_policy_undef_ptr;

    int m_policy_kernel;
    int m_policy_bash;
    int m_policy_gcc;
    int m_policy_openoffice;
    int m_policy_firefox;

    std::string m_pc_hash_value;
    std::string m_mb_hash_value;
    std::string m_bios_hash_value;
    std::string m_cpu_hash_value;
    std::string m_hdd_hash_value;
    std::vector<std::string> m_mem_hash_list;
    std::vector<std::string> m_nic_hash_list;
    std::vector<std::string> m_usb_hash_list;
public:
	static int m_policy_pos_array[CHECK_ITEM_NUM];
};

#endif
