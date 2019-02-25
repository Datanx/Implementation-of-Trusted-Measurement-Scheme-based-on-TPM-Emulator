#include <stdio.h>
#include "OsstartPolicyOperator.h"

int main()
{
    OsstartPolicyOperator opo(
        "010100000000000|2000000000|0000000000|10000000000000000000$0000000000",
        "c05583fe4a58a2eaa15a258e3c322bbb$0$c05583fe4a58a2eaa15a258e3c322b21"
    );
    opo.ApplyPolicy();
    
    return 0;
}

