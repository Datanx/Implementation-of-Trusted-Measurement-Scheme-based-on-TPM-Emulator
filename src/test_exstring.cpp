#include "ExString.h"

int main()
{
    std::string s1("haha|hoho|hehe$what|when|where$that|this");
    std::vector<std::string> vct_tmp;
    std::vector<std::string> vct_p1;
    std::vector<std::string> vct_p2;
    std::vector<std::string> vct_p3;

    vct_tmp = ExString::esTokenize(s1, "$");

    for(int i=0; i<vct_tmp.size(); i++)
        std::cout<<vct_tmp[i]<<std::endl;

    return 0;
}
