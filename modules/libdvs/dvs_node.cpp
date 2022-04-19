#include "dvs_node.h"
using namespace module::device::dvs;

DVSNode::DVSNode(
    PolledDataCallback data, 
    PolledExceptionCallback exception) 
    : polledDataCallback{data}, polledExceptionCallback{exception}, 
    did{0}, uid{-1}, module{DVSModuleType::DVS_MODULE_TYPE_NONE}
{}

DVSNode::~DVSNode()
{
    stop();
}

int DVSNode::run(const DVSModeConf& conf)
{
    int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        catchException();
        uid = login(conf.ip, conf.port, conf.user, conf.passwd);

        if (-1 < uid)
        {
            if (Error_Code_Success == getChanNum(uid, chanNums))
            {
                for (int i = 0; i != chanNums.size(); ++i)
                {
                    int64_t sid{openRealplayStream(uid, chanNums[i])};
                    if (-1 < sid)
                    {
                        sids.push_back(sid);
                    }
                }
            }
            
            did = conf.id;
        }
        else
        {
            ret = Error_Code_Device_Login_Failure;
        }
    }
    
    return ret;
}

int DVSNode::stop()
{
    int ret{0 < did && -1 < uid ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        for (int i = 0; i != sids.size(); ++i)
        {
            if (-1 < sids[i])
            {
                closeRealplayStream(sids[i]);
            }
        }
        sids.clear();

        ret = logout(uid);
        if (Error_Code_Success == ret)
        {
            uid = -1;
        }
    }
    
    return ret;
}
