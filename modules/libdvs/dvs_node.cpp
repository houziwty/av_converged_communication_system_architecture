#include "dvs_node.h"
using namespace module::device::dvs;

DVSNode::DVSNode(
    PolledDataCallback data, 
    PolledExceptionCallback exception) 
    : polledDataCallback{data}, polledExceptionCallback{exception}, 
    did{0}, uid{-1}, module{DVSModuleType::DVS_MODULE_TYPE_NONE}
{
    memset(cid, -1, 64 * sizeof(int64_t));
}

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
            //Try to open all live stream of channels at once.
            for (int i = 0; i != 64; ++i)
            {
                cid[i] = openRealplayStream(uid, i);
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
        //Try to close all live stream of channels at once.
        for (int i = 0; i != 64; ++i)
        {
            if (-1 < cid[i])
            {
                closeRealplayStream(cid[i]);
                cid[i] = -1;
            }
        }

        ret = logout(uid);
        if (Error_Code_Success == ret)
        {
            uid = -1;
        }
    }
    
    return ret;
}
