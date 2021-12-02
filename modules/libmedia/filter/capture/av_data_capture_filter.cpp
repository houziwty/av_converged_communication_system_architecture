#include "libavp/hiksdk/hk_sdk_av_capture.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "error_code.h"
#include "av_data_capture_filter.h"
using namespace module::media::av;

AVDataCaptureFilter::AVDataCaptureFilter() : MediaSourceFilter()
{}

AVDataCaptureFilter::~AVDataCaptureFilter()
{}

int AVDataCaptureFilter::createNew(void* param/* = nullptr*/)
{
	int ret{param ? MediaSourceFilter::createNew(param) : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		char* url{reinterpret_cast<char*>(param)};
		Url parser;
		ret = parser.parse(url);

		if(Error_Code_Success == ret)
		{
			if(0 == parser.getProtocol().compare("hiksdk"))
			{
				const std::string host{parser.getHost()};
				const std::vector<ParamItem> params{parser.getParameters()};
				const std::size_t pos{host.find_first_of(':')};

				LoginInfo info;
				info.ip = host.substr(0, pos);
				info.port = static_cast<unsigned short>(atoi(host.substr(pos + 1, host.length()).c_str()));
				for(int i = 0; i != params.size(); ++i)
				{
					if(0 == params[i].key.compare("username"))
					{
						info.username = params[i].value;
					}
					else if(0 == params[i].key.compare("password"))
					{
						info.passwd = params[i].value;
					}
				}

				if(!info.ip.empty() && !info.username.empty() && !info.passwd.empty() && 0 < info.port)
				{
					AVProcessorPtr ptr{boost::make_shared<HKSdkAVCapture>()};
					if(ptr && Error_Code_Success == ptr->createNew(&info))
					{

					}
				}
				else
				{
					ret = Error_Code_Operate_Failure;
				}
			}
			else
			{
				ret = Error_Code_Not_Support;
			}
		}
	}
	
	return ret;
}
