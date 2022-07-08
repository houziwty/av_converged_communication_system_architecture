# <div align="center">流媒体服务平台</div>

>>> * 编制：王科威
>>> * 审核：
>>> * 版本：V0.0.1

# <u>_修改历史_</u>

|版本|说明|作者|审核|日期|
|-|-|-|-|-|
| V0.0.1| 初稿 | 王科威 | | 2021-11-20|

   * 1. [引言](#First)
   * 2. [基本约定](#Second)
   * 3. [系统设计](#SystemDesign)
   * 4. [边缘计算](#EdgeComputing)
   * 5. [GStreamer插件编写](#Gstreamerplugins)

   # 1. <a name='First'></a>引言
   
   ## 1.1 编写目的

   流媒体服务是以多平台音视频数据交互与多种通信方式相融合的系统设计和实现。

  >>> * 支持GB28181-2016的设备接入；
  >>> * 支持GB28181-2016平台的级联；
  >>> * 支持GB28181-2016设备云台控制、远程重启、录像控制、报警布/撤防、报警复位、焦距放大/缩小、预置位配置等；
  >>> * 支持SDK设备强制关键帧和参数配置；
  >>> * 提供音视频PS流转ES流服务；
  >>> * 支持TCP和UDP数据传输模式；
  >>> * 支持HTTP和HTTPS交互；
  >>> * 支持RTSP、RTMP、HTTP-FLV、WEBSOCKET-FLV、HLS和WebRTC等多种数据格式等音视频流拉取和推送；
  >>> * 支持B/S结构下的浏览器端提供无插件播放；
  >>> * 支持实时音视频播放；
  >>> * 支持音视频数据集中存储；
  >>> * 支持历史数据回放与控制；
  >>> * 支持语音对讲；
  >>> * 支持系统用户管理、终端设备配置、系统信息查询、黑名单和白名单等业务逻辑处理；
  >>> * 提供REST API接口以支持客户端二次开发。

  ## 1.2 参考资料


   # 2. 基本约定
   ## 2.1 命名规则
   所有命名采用小写字母，每个单词间使用一个下划线连接。
   ## 2.2 服务标识
|名称|标识|默认端口|说明|
|-|-|-|-|
|XMQ主机服务|xmq_host_service|50531 50927|消息主机服务|
|设备主机服务|dvs_host_service|50820|设备接入主机服务，服务第三方监控设备接入|
|流媒体转发主机服务|xms_host_service|50820|流媒体转发主机服务|
|算法主机服务|video_cv_host_service|-|算法运算主机服务|

   # 3. 系统设计

   ## 3.1系统架构图

   <div align="center">
      <img src=./doc/architecture.png />
   </div>

   ## 3.2 Rest API接口

   ### 3.2.1 系统接口相关
   #### 3.2.1.1 用户登录
   ```GET　/streamapi/v1/system/login```

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| username| 必选 | String | 用户名称|
| password| 必选 | String | 密码(经过md5加密,32位长度,不带中划线,不区分大小写)|

```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| token| 必选 | String | 客户端标识|
| timeout| 必选 | Number | token超时时间，单位：秒|

   #### 3.2.1.2 用户注销
```GET　/streamapi/v1/system/logout```

```HTTP/1.1 200 OK```

   #### 3.2.1.3 查询登录用户信息
   ```GET　/streamapi/v1/system/userinfo```

   ```HTTP/1.1 200 OK```

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| token| 必选 | String | 客户端标识|
| id| 必选 | String | 用户标识|
| name| 必选 | String | 用户名|
| roles| 可选 | Array | 角色组|
| hasallchannel| 可选 | Boolean | 是否关联所有通道|
| cas| 可选 | Boolean | 是否单点登录用户|
| ip| 可选 | String | 登录IP|
| time| 可选 | String | 登录时间|

   #### 3.2.1.4 查询平台运行信息
   ```GET　/streamapi/v1/system/getserviceinfo```
   
   ```HTTP/1.1 200 OK```

   #### 3.2.1.5 查询平台API接口
   ```GET　/api/v1/getapilist```
   
   ```HTTP/1.1 200 OK```

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| ApiList | 必选 | Array | API接口列表 |
| - Name | 必选 | String | 接口名称 |
| - Comment | 必选 | String | 接口描述 |

   #### 3.2.1.6 修改密码
   ```GET　/streamapi/v1/system/modifypassword```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| oldpassword| 必选 | String | 旧密码|
| newpassword| 必选 | String | 新密码|
   
   ```HTTP/1.1 200 OK```

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| timeout| 必选 | String | token超时，单位：秒|

   #### 3.2.1.7 重启服务
   ```GET　/streamapi/v1/system/restart```
   
   ```HTTP/1.1 200 OK```

   ### 3.2.2 用户管理相关
   #### 3.2.2.1 查询用户列表
   ```GET　/streamapi/v1/user/querylist```
   
   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| count| 必选 | Number | 用户总数|
| userlist| 必选 | Array | 用户列表|
| - id | 必选 | String | 用户标识|
| - username | 必选 | String | 用户名称|
| - role | 必选 | String | 用户角色|
| - enable | 必选 | Boolean | 是否启用|
| - lastlogintime | 必选 | Boolean | 最近登录时间|

   #### 3.2.2.2 查询用户信息
   ```GET　/streamapi/v1/user/queryinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
   
   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| username| 必选 | String | 用户名称|
| role| 必选 | String | 用户角色|
| enable| 必选 | Boolean | 是否启用|
| hasallchannel| 必选 | Boolean | 是否关联所有通道|
| lastlogintime| 必选 | String | 最近登录时间|

   #### 3.2.2.3 新增用户
   ```GET　/streamapi/v1/user/add```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| username| 必选 | String | 用户名称|
| role| 必选 | String | 用户角色|
| enable| 可选 | Boolean | 是否启用|
   
   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| defaultpassword| 必选 | String | 默认初始密码|

   #### 3.2.2.4 删除用户
   ```GET　/streamapi/v1/user/remove```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
   
   ```HTTP/1.1 200 OK```
   
   #### 3.2.2.5 修改密码
   ```GET　/streamapi/v1/user/modifypassword```

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| oldpassword| 必选 | String | 旧密码|
| newpassword| 必选 | String | 新密码|

   #### 3.2.2.6 设置用户启用状态
   ```GET　/streamapi/v1/user/setenable```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| enable| 必选 | Boolean | 是否启用|

   ```HTTP/1.1 200 OK```

   #### 3.2.2.7 查询用户关联通道
   ```GET　/streamapi/v1/user/querychannellist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| relatedcount| 必选 | Number | 已关联通道总数|
| hasallchannel| 必选 | Boolean | 是否关联所有通道|
| channellist| 必选 | Array | 已关联通道列表|
| - channelid| 必选 | String | 通道标识|
| - deviceid| 必选 | String | 设备标识|
| - channelindex| 必选 | String | 通道序号|
| - channelname| 必选 | String | 通道名称|
| - custom| 必选 | Boolean | 是否自定义通道|
| - customname| 必选 | String | 自定义通道名称|
| - manufacturer| 必选 | String | 通达设备厂商|
| - model| 必选 | String | 通道设备型号|
| - owner| 必选 | String | 通道设备归属|
| - civilcode| 必选 | String | 行政区域|
| - address| 必选 | String | 安装地址|
| - parental| 必选 | Boolean | 是否有通道子设备|
| - parentid| 必选 | Boolean | 直接上级标识|
| - secrecy| 必选 | Boolean | 是否保密|
| - registerway| 必选 | Number | 注册方式, 缺省为1, 1-IETF RFC3261, 2-基于口令的双向认证, 3-基于数字证书的双向认证|
| - status| 必选 | Boolean | 是否在线|
| - audioenable| 必选 | Boolean | 是否配置音频开关|

   #### 3.2.2.8 设置用户关联所有通道
   ```GET　/streamapi/v1/user/sethasallchannel```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| hasallchannel| 必选 | Boolean | 是否关联所有通道|

   ```HTTP/1.1 200 OK```

   #### 3.2.2.9 修改用户关联通道
   ```GET　/streamapi/v1/user/setrelatedchannels```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| channels| 必选 | Array | 通道列表，格式为[id1:true, id2:false]|

   ```HTTP/1.1 200 OK```

   ### 3.2.3 GB28181向上级联相关

   #### 3.2.3.1 查询上级平台信息
   ```GET　/streamapi/v1/cascade/querylist```

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| cascadecount| 必选 | String | 上级平台总数|
| cascadelist| 必选 | Array | 上级平台列表|
| - id| 必选 | String | 平台标识|
| - name| 必选 | String | 平台名称|
| - sipid| 必选 | String | SIP服务标识|
| - realm| 必选 | String | SIP服务域|
| - host| 必选 | String | SIP服务IP|
| - port| 必选 | Number | SIP服务端口号|
| - localid| 必选 | String | 设备编号|
| - localhost| 必选 | String | 本地IP|
| - localport| 必选 | Number | 本地SIP端口号|
| - username| 必选 | String | SIP认证用户名|
| - password| 必选 | String | SIP认证密码|
| - keepaliveinterval| 必选 | Number | 心跳周期， 单位：秒|
| - registerinterval| 必选 | Number | 注册周期， 单位：秒|
| - registertimeout| 必选 | Number | 注册有效期， 单位：秒|
| - online| 必选 | Boolean | 是否在线|
| - shareallchannel| 必选 | Boolean | 是否共享所有通道|
| - enable| 必选 | Boolean | 是否启用|

   #### 3.2.3.2 查询单个上级平台信息
   ```GET　/streamapi/v1/cascade/queryinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 上级平台标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| - name| 必选 | String | 平台名称|
| - sipid| 必选 | String | SIP服务标识|
| - realm| 必选 | String | SIP服务域|
| - host| 必选 | String | SIP服务IP|
| - port| 必选 | Number | SIP服务端口号|
| - localid| 必选 | String | 设备编号|
| - localhost| 必选 | String | 本地IP|
| - localport| 必选 | Number | 本地SIP端口号|
| - username| 必选 | String | SIP认证用户名|
| - password| 必选 | String | SIP认证密码|
| - keepaliveinterval| 必选 | Number | 心跳周期， 单位：秒|
| - registerinterval| 必选 | Number | 注册周期， 单位：秒|
| - registertimeout| 必选 | Number | 注册有效期， 单位：秒|
| - online| 必选 | Boolean | 是否在线|
| - shareallchannel| 必选 | Boolean | 是否共享所有通道|
| - enable| 必选 | Boolean | 是否启用|

   #### 3.2.3.3 查询单个上级平台通道列表信息
   ```GET　/streamapi/v1/cascade/querychannellist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 上级平台标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelcount| 必选 | Number | 通道总数|
| channelrelatecount| 必选 | Number | 已共享通道总数|
| shareallchannel| 必选 | Boolean | 是否共享所有通道|
| channellist| 必选 | Array | 已共享通道列表|
| - cascadeid| 必选 | String | 上级平台标识|
| - channelid| 必选 | String | 通道标识|
| - deviceid| 必选 | String | 设备标识|
| - channelindex| 必选 | String | 通道序号|
| - channelname| 必选 | String | 通道名称|
| - custom| 必选 | Boolean | 是否自定义通道|
| - customname| 必选 | String | 自定义通道名称|
| - manufacturer| 必选 | String | 通达设备厂商|
| - model| 必选 | String | 通道设备型号|
| - owner| 必选 | String | 通道设备归属|
| - civilcode| 必选 | String | 行政区域|
| - address| 必选 | String | 安装地址|
| - parental| 必选 | Boolean | 是否有通道子设备|
| - parentid| 必选 | Boolean | 直接上级标识|
| - secrecy| 必选 | Boolean | 是否保密|
| - registerway| 必选 | Number | 注册方式, 缺省为1, 1-IETF RFC3261, 2-基于口令的双向认证, 3-基于数字证书的双向认证|
| - status| 必选 | Boolean | 是否在线|
| - audioenable| 必选 | Boolean | 是否配置音频开关|

   #### 3.2.3.4 新增上级平台信息
   ```GET　/streamapi/v1/cascade/add```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 上级平台标识|
| enable| 必选 | Boolean | 是否启用|
| name| 必选 | String | 上级平台名称|
| sipid| 必选 | String | SIP服务标识|
| realm| 可选 | String | SIP服务域|
| host| 必选 | String | SIP服务IP|
| port| 必选 | Number | SIP服务端口号|

   ```HTTP/1.1 200 OK```

   #### 3.2.3.5 修改级联共享通道
   ```GET　/streamapi/v1/cascade/modifysharechannel```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 用户标识|
| channels| 必选 | Array | 通道列表，格式为[id1:true, id2:false]|

   ```HTTP/1.1 200 OK```

   #### 3.2.3.6 新增级联所有共享通道
   ```GET　/streamapi/v1/cascade/setshareallchannel```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| id| 必选 | String | 上级平台标识|

   ```HTTP/1.1 200 OK```

   ### 3.2.4 黑/白名单设备管理相关

   #### 3.2.4.1 查询黑/白名单设备列表
   ```GET　/streamapi/v1/block/querylist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| type| 必选 | Number | 黑/白名单类型， 0表示黑名单，1表示白名单|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| blockcount| 必选 | Number | 黑/白名单总数|
| blocklist| 必选 | Array | 黑/白名单列表|
| - sipid| 必选 | String | SIP标识|
| - password| 必选 | String | 接入密码|
| - description| 必选 | String | 描述信息|

   #### 3.2.4.2 新增黑/白名单设备
   ```GET　/streamapi/v1/block/add```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| type| 必选 | Number | 黑/白名单类型， 0表示黑名单，1表示白名单|
| sipid| 必选 | String | SIP标识|
| description| 可选 | String | 描述信息|

   ```HTTP/1.1 200 OK```

   #### 3.2.4.3 删除黑/白名单设备
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| type| 必选 | Number | 黑/白名单类型， 0表示黑名单，1表示白名单|
| sipid| 必选 | String | SIP标识|
| description| 可选 | String | 描述信息|

   ```HTTP/1.1 200 OK```

   #### 3.2.4.3 导入黑/白名单配置
   ```GET　/streamapi/v1/block/import```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| type| 必选 | Number | 黑/白名单类型， 0表示黑名单，1表示白名单|
| file| 必选 | String | 配置文件，类型为xml或json|

   ```HTTP/1.1 200 OK```

   #### 3.2.4.4 导出黑/白名单配置
   ```GET　/streamapi/v1/block/export```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| type| 必选 | Number | 黑/白名单类型， 0表示黑名单，1表示白名单|

   ```HTTP/1.1 200 OK```

   ### 3.2.5 GB28181设备管理相关

   #### 3.2.5.1 查询设备列表
   ```GET　/streamapi/v1/device/querylist```

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| devicecount| 必选 | Number | 设备总数|
| devicelist| 必选 | Array | 设备列表|
| - id| 必选 | String | 设备SIP标识|
| - name| 必选 | String | 设备名称|
| - channelcount| 必选 | Number | 设备通道总数|
| - enablesubcatalog| 必选 | Boolean | 是否开启目录订阅|
| - enablesubalarm| 必选 | Boolean | 是否开启报警订阅|
| - enablesubposition| 必选 | Boolean | 是否开启位置订阅|
| - status| 必选 | Boolean | 是否在线|
| - lastsynctime| 必选 | String | 最近同步时间|

   #### 3.2.5.2 查询单个设备信息
   ```GET　/streamapi/v1/device/querylist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| devicecount| 必选 | Number | 设备总数|
| devicelist| 必选 | Array | 设备列表|
| - id| 必选 | String | 设备SIP标识|
| - name| 必选 | String | 设备名称|
| - channelcount| 必选 | Number | 设备通道总数|
| - enablesubcatalog| 必选 | Boolean | 是否开启目录订阅|
| - enablesubalarm| 必选 | Boolean | 是否开启报警订阅|
| - enablesubposition| 必选 | Boolean | 是否开启位置订阅|
| - status| 必选 | Boolean | 是否在线|
| - lastsynctime| 必选 | String | 最近同步时间|

   #### 3.2.5.3 查询单个设备通道列表
   ```GET　/streamapi/v1/device/querychannellist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelount| 必选 | Number | 通道总数|
| channellist| 必选 | Array | 通道列表|
| - channelid| 必选 | String | 通道SIP标识|
| - deviceid| 必选 | String | 设备SIP标识|
| - devicename| 必选 | String | 设备名称|
| - devicestatus| 必选 | Boolean | 设备在线状态|
| - channelindex| 必选 | Number | 通道序号|
| - block| 必选 | String | 警区|
| - custom| 必选 | Boolean | 是否自定义通道|
| - manufacturer| 必选 | String | 通道设备厂商|
| - model| 必选 | String | 通道设备型号|
| - owner| 必选 | String | 通道设备归属|
| - civilcode| 必选 | String | 行政区域|
| - address| 必选 | String | 安装地址|
| - status| 必选 | Boolean | 在线状态|
| - enableaudio| 必选 | Boolean | 是否开启音频|
| - cloudrecord| 必选 | Boolean | 是否开启云存储|

   #### 3.2.5.4 查询单个通道信息
   ```GET　/streamapi/v1/device/querychannelinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelid| 必选 | String | 通道SIP标识|
| deviceid| 必选 | String | 设备SIP标识|
| devicename| 必选 | String | 设备名称|
| devicestatus| 必选 | Boolean | 设备在线状态|
| channelindex| 必选 | Number | 通道序号|
| block| 必选 | String | 警区|
| custom| 必选 | Boolean | 是否自定义通道|
| manufacturer| 必选 | String | 通道设备厂商|
| model| 必选 | String | 通道设备型号|
| owner| 必选 | String | 通道设备归属|
| civilcode| 必选 | String | 行政区域|
| address| 必选 | String | 安装地址|
| status| 必选 | Boolean | 在线状态|
| enableaudio| 必选 | Boolean | 是否开启音频|
| cloudrecord| 必选 | Boolean | 是否开启云存储|

   #### 3.2.5.5 查询设备状态记录
   ```GET　/streamapi/v1/device/querystatusinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| statuscount| 必选 | Number | 设备状态总数|
| statuslist| 必选 | Array | 设备状态列表|
| - sipid| 必选 | String | 设备SIP标识|
| - status| 必选 | String | 设备在线状态|
| - description| 可选 | String | 描述信息|

   #### 3.2.5.6 查询设备流量统计数据
   ```GET　/streamapi/v1/device/querystreaminfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|
| type| 可选 | Number | 统计方式：0表示当日, >0表示最近|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamlist| 必选 | Array | 设备统计数据列表|
| - time| 必选 | String | 时间段|
| - inbytes| 必选 | Number | 接收字节数|
| - outbytes| 必选 | Number | 发送字节数|

   #### 3.2.5.7 查询通道巡航列表
   ```GET　/streamapi/v1/device/getchannelnavigate```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| navigationcount| 必选 | Number | 巡航总数|
| navigationlist| 必选 | Array | 巡航列表|
| - groupid| 必选 | String | 巡航组标识|
| - preset| 必选 | String | 预置位列表，以逗号分隔|
| - speed| 必选 | Number | 巡航速度|
| - interval| 必选 | Number | 巡航停留间隔|

   #### 3.2.5.8 获取下级通道列表
   ```GET　/streamapi/v1/device/querysubcatalog```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelount| 必选 | Number | 通道总数|
| channellist| 必选 | Array | 通道列表|
| - channelid| 必选 | String | 通道SIP标识|
| - deviceid| 必选 | String | 设备SIP标识|
| - devicename| 必选 | String | 设备名称|
| - devicestatus| 必选 | Boolean | 设备在线状态|
| - channelindex| 必选 | Number | 通道序号|
| - block| 必选 | String | 警区|
| - custom| 必选 | Boolean | 是否自定义通道|
| - manufacturer| 必选 | String | 通道设备厂商|
| - model| 必选 | String | 通道设备型号|
| - owner| 必选 | String | 通道设备归属|
| - civilcode| 必选 | String | 行政区域|
| - address| 必选 | String | 安装地址|
| - status| 必选 | Boolean | 在线状态|
| - enableaudio| 必选 | Boolean | 是否开启音频|
| - cloudrecord| 必选 | Boolean | 是否开启云存储|

   #### 3.2.5.9 获取下级设备信息
   ```GET　/streamapi/v1/device/querysubdeviceinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | Number | 设备SIP标识|
| name| 必选 | String | 设备名称|
| result| 必选 | String | 查询结果， OK表示成功，ERROR表示失败|
| status| 可选 | Boolean | 在线状态|
| manufacturer| 可选 | String | 设备厂商|
| firmware| 可选 | String | 设备固件版本|

   #### 3.2.5.10 获取下级通道状态
   ```GET　/streamapi/v1/device/querysubchannelstatus```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | Number | 通道SIP标识|
| name| 必选 | String | 通道名称|
| result| 必选 | String | 查询结果， OK表示成功，ERROR表示失败|
| online| 必选 | Boolean | 在线状态|
| status| 必选 | Boolen | 是否正常工作|

   #### 3.2.5.11 获取下级通道信息
   ```GET　/streamapi/v1/device/querysubchannelinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | Number | 通道SIP标识|
| result| 必选 | String | 通道名称|
| result| 必选 | String | 查询结果， OK表示成功，ERROR表示失败|
| online| 必选 | Boolean | 在线状态|
| status| 必选 | Boolen | 是否正常工作|

   #### 3.2.5.12 获取下级通道预置位
   ```GET　/streamapi/v1/device/querysubchannelpreset```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | Number | 通道SIP标识|
| result| 必选 | String | 查询结果， OK表示成功，ERROR表示失败|
| presetcount| 必选 | Number | 预置位总数|
| presetlist| 必选 | Array | 预置位列表|
| - presetid| 必选 | String | 预置位SIP标识|
| - presetname| 必选 | String | 预置位名称|

   #### 3.2.5.13 设置通道流传输模式
   ```GET　/streamapi/v1/device/setstreamtransport```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| transport| 必选 | String | 传输模式，TCP/UDP|
| transport_mode| 可选 | String | 主被动模式，active/passive|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.14 设置通道音频开关
   ```GET　/streamapi/v1/device/setchannelaudio```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| enable| 必选 | Boolean | 是否使能|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.15 设置通道共享开关
   ```GET　/streamapi/v1/device/setchannelshare```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| enable| 必选 | Boolean | 是否使能|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.16 设置通道云端录像
   ```GET　/streamapi/v1/device/setchannelcloudrecord```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| enable| 必选 | Boolean | 是否使能|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.17 设置通道自定义名称
   ```GET　/streamapi/v1/device/setchanneluserdefinename```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| enable| 必选 | Boolean | 是否使能|
| name| 必选 | String | 通道名称|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.18 设置通道自定义国标编号
   ```GET　/streamapi/v1/device/setchanneluserdefinesipid```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| enable| 必选 | Boolean | 是否使能|
| userdefinesipid| 必选 | String | SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.19 设置设备名称
   ```GET　/streamapi/v1/device/setdevicename```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|
| name| 必选 | String | 设备名称|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.20 删除离线设备
   ```GET　/streamapi/v1/device/removeofflinedevice```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.5.21 获取设备通道快照
   ```GET　/streamapi/v1/device/setchannelsnap```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```

   ```Channel-Snap-Time:yyyy-MM-DD HH:mm:ss```

   ```image/jpeg 格式，http body为图片数据```

   ### 3.2.6 GB28181设备控制相关

   #### 3.2.6.1 设置通道云台控制
   ```GET　/streamapi/v1/control/ptzcontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: stop=0, left=1, right=2, up=3, down=4, upright=5, downleft=6, downright=7, zoomin=8, zoomout=9|
| speed| 可选 | Number | 速度，范围0～255|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.2 设置通道光圈
   ```GET　/streamapi/v1/control/focuscontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: stop=0, near=1, far=2, irisin=3, irisout=4|
| speed| 可选 | Number | 速度，范围0～255|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.3 设置通道预置位
   ```GET　/streamapi/v1/control/presetcontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: set=1, goto=2, remove=3|
| preset| 可选 | Number | 预置位编号，范围1～255|
| name| 可选 | String | 预置位名称|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.4 设置通道看守位
   ```GET　/streamapi/v1/control/homepositioncontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| resettime| 必选 | Number | 自动回位时间间隔，单位：秒|
| presetindex| 必选 | Number | 调用预置位编号|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.5 设置通道巡航控制
   ```GET　/streamapi/v1/control/navigatecontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: stop=0, start=1，query=2, add=3, remove=4, setspeed=5， setstay=6，setstart=7|
| groupid| 可选 | Number | 巡航组编号(1~255), 适用于 add, remove, setspeed, setstay, start|
| preset| 可选 | Number | 预置位编号(0~255), 适用于 add, remove, 当 command=remove 且 preset=0 时, 表示删除 groupid 对应的整条巡航|
| speed| 可选 | Number | 巡航速度(1~4095), 适用于 setspeed|
| stay| 可选 | Number | 巡航停留时间(秒)(1~4095), 适用于 setstay|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| navigatecount| 必选 | Number | 巡航列表总数|
| navigatelist| 必选 | Array | 巡航列表|
| - groupid| 必选 | Number | 巡航组编号|
| - presets| 必选 | String | 巡航组内预置位表列, 逗号间隔|
| - speed| 必选 | Number | 巡航速度|
| - stay| 必选 | Number | 巡航停留时间|

   #### 3.2.6.6 设置通道焦距拉近
   ```GET　/streamapi/v1/control/zoomin```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| length| 必选 | Number | 播放窗口长度像素值|
| width| 必选 | Number | 播放窗口宽度像素值|
| midpointx| 必选 | Number | 拉框中心的横坐标像素值|
| midpointy| 必选 | Number | 拉框中心的纵坐标像素值 |
| lengthx| 必选 | Number | 拉框长度像素值|
| lengthy| 必选 | Number | 拉框宽度像素值|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.7 设置通道焦距拉远
   ```GET　/streamapi/v1/control/zoomout```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| length| 必选 | Number | 播放窗口长度像素值|
| width| 必选 | Number | 播放窗口宽度像素值|
| midpointx| 必选 | Number | 拉框中心的横坐标像素值|
| midpointy| 必选 | Number | 拉框中心的纵坐标像素值 |
| lengthx| 必选 | Number | 拉框长度像素值|
| lengthy| 必选 | Number | 拉框宽度像素值|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.8 设置通道远程录像
   ```GET　/streamapi/v1/control/zoomout```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.9 设置通道报警布/撤防
   ```GET　/streamapi/v1/control/guard```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: setguard=1, resetguard=2|
| timeout| 必选 | Number | 超时时间，单位：秒|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.10 设置通道雨刷开关
   ```GET　/streamapi/v1/control/wiper```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: on=1, off=0|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.11 设置通道补光灯开关
   ```GET　/streamapi/v1/control/lighting```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| command| 必选 | Number | 控制指令: on=1, off=0|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.12 设置设备报警通知
   ```GET　/streamapi/v1/control/alarmnotify```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| priority| 必选 | Number | 报警级别: 一级=1, 二级=2，三级-3，四级=4|
| method| 必选 | Number | 报警方式: 电话=1, 设备=短信-3，GPS=4， 视频=5，设备故障=6，其他=7|
| type| 可选 | Number | 报警方式为2时,不携带 AlarmType 为默认的设备报警,携带 AlarmType 取值对应报警类型如下:1-视频丢失报警,2-设备防拆报警,3-存储设备磁盘满报警,4-设备高温报警,5-设备低温报警; 报警方式为5时,取值:1-人工视频报警,2-运动目标检测报警,3-遗留物检测报警,4-物体移除检测报警,5-绊线检测报警,6-入侵检测报警,7-逆行检测报警,8-徘徊检测报警,9-流量统计报警,10-密度检测报警,11-视频异常检测报警,12-快速移动报警
报警方式为6时,取值如下:1-存储设备磁盘故障报警,2-存储设备风扇故障报警|
| eventtype| 可选 | Number | 报警类型为入侵检测报警时,可携带事件类型,1-进入区域,2-离开区域|
| timeout| 可选 | Number | 超时时间，单位：秒|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.13 设置设备报警复位
   ```GET　/streamapi/v1/control/alarmset```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.14 设置通道强制关键帧
   ```GET　/streamapi/v1/control/forceiframe```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.15 设置设备配置
   ```GET　/streamapi/v1/control/deviceconfig```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| basic| 可选 | String | xml|
| encodecfg| 可选 | String | xml|
| decodecfg| 可选 | String | xml|

   ```HTTP/1.1 200 OK```

   #### 3.2.6.16 启动设备语音喊话
   ```GET　/streamapi/v1/control/audiotalk```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| audio| 必选 | String | 音频数据, Base64 编码, encodeURIComponent, 8000 采样率, 单通道|

   ```HTTP/1.1 200 OK```

   ### 3.2.7 实时播放

   #### 3.2.7.1 开始实时播放
   ```GET　/streamapi/v1/livestream/play```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| audio| 可选 | Boolean | 是否开启音频|
| transport| 可选 | String | 流传输模式，TCP/UDP|
| transport_port| 可选 | String | 当transport=TCP 时有效，active/passive|
| cdn| 可选 | String | 转推CDN地址|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|
| smsid| 必选 | String | 流媒体标识|
| deviceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| channelname| 必选 | String | 通道名称|
| webrtc| 必选 | String | WEBRTC地址|
| flv| 必选 | String | FLV地址|
| wsflv| 必选 | String | Websocket-FLV地址|
| rtmp| 必选 | String | RTMP地址|
| hls| 必选 | String | HLS地址|
| rtsp| 必选 | String | RTSP地址|
| cdn| 可选 | String | 转推CDN地址|
| transport| 必选 | String | 流传输模式，TCP/UDP|
| starttime| 必选 | String | 开始时间|
| enableaudio| 必选 | Boolean | 是否开启音频|

   #### 3.2.7.2 停止实时流播放
   ```GET　/streamapi/v1/livestream/stopplay```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.7.3 获取实时流播放列表
   ```GET　/streamapi/v1/livestream/streamlist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamcount| 必选 | String | 流播放总数|
| streamlist| 必选 | Array | 流播放列表|
| - streamid| 必选 | String | 流标识|
| - smsid| 必选 | String | 流媒体标识|
| - deviceid| 必选 | String | 设备标识|
| - channelid| 必选 | String | 通道标识|
| - channelname| 必选 | String | 通道名称|
| - webrtc| 必选 | String | WEBRTC地址|
| - flv| 必选 | String | FLV地址|
| - wsflv| 必选 | String | Websocket-FLV地址|
| - rtmp| 必选 | String | RTMP地址|
| - hls| 必选 | String | HLS地址|
| - rtsp| 必选 | String | RTSP地址|
| - cdn| 可选 | String | 转推CDN地址|
| - transport| 必选 | String | 流传输模式，TCP/UDP|
| - starttime| 必选 | String | 开始时间|
| - enableaudio| 必选 | Boolean | 是否开启音频|

   #### 3.2.7.4 获取单个实时流播放信息
   ```GET　/streamapi/v1/livestream/streaminfo```
 |参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|
| smsid| 必选 | String | 流媒体标识|
| deviceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| channelname| 必选 | String | 通道名称|
| webrtc| 必选 | String | WEBRTC地址|
| flv| 必选 | String | FLV地址|
| wsflv| 必选 | String | Websocket-FLV地址|
| rtmp| 必选 | String | RTMP地址|
| hls| 必选 | String | HLS地址|
| rtsp| 必选 | String | RTSP地址|
| cdn| 可选 | String | 转推CDN地址|
| transport| 必选 | String | 流传输模式，TCP/UDP|
| starttime| 必选 | String | 开始时间|
| enableaudio| 必选 | Boolean | 是否开启音频|

   #### 3.2.7.5 设置实时流CDN加速转推
   ```GET　/streamapi/v1/livestream/streaminfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| cdn| 必选 | String | 转推CDN地址|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|
| cdncount| 必选 | Number | 转推总数|
| cdnlist| 必选 | Array | 转推列表|
| - cdn| 必选 | String | CDN地址|
| - starttime| 必选 | String | 开始时间|

   #### 3.2.7.6 获取CDN加速列表
   ```GET　/streamapi/v1/livestream/cdnlist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|
| cdncount| 必选 | Number | 转推总数|
| cdnlist| 必选 | Array | 转推列表|
| - cdn| 必选 | String | CDN地址|
| - starttime| 必选 | String | 开始时间|

   #### 3.2.7.7 停止转推
   ```GET　/streamapi/v1/livestream/stopcdn```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|

   ```HTTP/1.1 200 OK```

   ### 3.2.8 录像回放
   #### 3.2.8.1 查询设备录像列表
   ```GET　/streamapi/v1/playback/queryrecordlist```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 设备SIP标识|
| starttime| 可选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| stoptime| 可选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| type| 可选 | Number | 录像类型, 1-所有, 2-定时录像, 3-报警录像, 4-手动录像|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| name| 必选 | String | 通道名称|
| recordcount| 必选 | Number | 录像总数|
| recordlist| 必选 | Array | 录像列表|
| - channelid| 必选 | String | 通道SIP标识|
| - name| 必选 | String | 通道名称|
| - filepath| 必选 | String | 文件路径|
| - address| 必选 | String | 录像地址|
| - starttime| 必选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| - stoptime| 必选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| - type| 必选 | Number | 录像类型, 1-所有, 2-定时录像, 3-报警录像, 4-手动录像|

   #### 3.2.8.2 设置录像回放
   ```GET　/streamapi/v1/playback/startplay```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| starttime| 必选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| stoptime| 可选 | String | 开始时间, YYYY-MM-DDTHH:mm:ss|
| audio| 必选 | Boolean | 是否开启音频|
| transport| 必选 | String | 流传输模式，TCP/UDP|
| transport_mode| 必选 | String | 当 transport=TCP 时有效, active/passive|

   ```HTTP/1.1 200 OK```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| streamid| 必选 | String | 流标识|
| smsid| 必选 | String | 流媒体标识|
| deviceid| 必选 | String | 设备ID标识|
| channelid| 必选 | String | 通道ID标识|
| channelname| 必选 | String | 通道名称|
| webrtc| 必选 | String | WEBRTC地址|
| flv| 必选 | String | FLV地址|
| wsflv| 必选 | String | Websocket-FLV地址|
| rtmp| 必选 | String | RTMP地址|
| hls| 必选 | String | HLS地址|
| rtsp| 必选 | String | RTSP地址|
| cdn| 可选 | String | 转推CDN地址|
| transport| 必选 | String | 流传输模式，TCP/UDP|
| starttime| 必选 | String | 开始时间|
| enableaudio| 必选 | Boolean | 是否开启音频|
| transport| 必选 | String | 流传输模式，TCP/UDP|
| totalseconds| 必选 | Number | 文件总时长，单位：秒|
| timestamp| 必选 | Number | 当前回放时间，单位：秒|
| progress| 必选 | Number | 当前回放进度[0~100]|

   #### 3.2.8.3 停止录像回放
   ```GET　/streamapi/v1/playback/stopplay```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| streamid| 必选 | String | 流标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.8.4 设置录像回放控制
   ```GET　/streamapi/v1/playback/playcontrol```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| sipid| 必选 | String | 通道SIP标识|
| streamid| 必选 | String | 流标识|
| command| 必选 | Number | 回放控制命令，0-stop， 1-play，2-teardown，3=scale|
| range| 可选 | String | command=play 时有效,表示从当前位置以当前播放速度跳转到指定range(单位s)的时间点播放;range=now 表示从当前位置开始播放,比如:暂停后恢复播放,则指定range=now|
| scale| 可选 | Number | command=scale 时有效,倍数播放倍率,1=正常播放,大于0小于1为慢放,如:0.5=以0.5倍的速度慢放;大于1为快放,如:2=以2倍的速度快放,负数为倒放,参数意义相同|

   ```HTTP/1.1 200 OK```

   ### 3.2.9 云端实时录像

   #### 3.2.9.1 查询云端录像通道信息
   ```GET　/streamapi/v1/cloudrecord/querychannelinfo```
|参数名称|规则|类型|说明||
|-|-|-|-|-|

   ```HTTP/1.1 200 OK```
   |参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelcount| 必选 | Number | 通道总数|
| channellist| 必选 | Array | 通道列表|
| - deviceid| 必选 | String | 设备标识|
| - channelid| 必选 | String | 通道标识|
| - channelname| 必选 | String | 通道名称|

   #### 3.2.9.4 查询通道所有录像统计信息
   ```GET　/streamapi/v1/cloudrecord/queryallrecordflag```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|

   ```HTTP/1.1 200 OK```
   |参数名称|规则|类型|说明||
|-|-|-|-|-|
| channelcount| 必选 | Number | 通道总数|
| channellist| 必选 | Array | 通道列表|
| - time| 必选 | String | 时间， YYYYMM|
| - result| 必选 | String | 标记当月每一天是否有录像, 0 - 没有录像, 1 - 有录像|

   #### 3.2.9.5 查询通道按月录像统计信息
   ```GET　/streamapi/v1/cloudrecord/querymonthrecordflag```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| month| 必选 | String | 时间， YYYYMM|

   ```HTTP/1.1 200 OK```
   |参数名称|规则|类型|说明||
|-|-|-|-|-|
| result| 必选 | String | 标记当月每一天是否有录像, 0 - 没有录像, 1 - 有录像|

   #### 3.2.9.6 查询通道按日录像统计信息
   ```GET　/streamapi/v1/cloudrecord/querydailyrecordflag```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| day| 必选 | String | 时间， YYYYMMDD|

   ```HTTP/1.1 200 OK```
   |参数名称|规则|类型|说明||
|-|-|-|-|-|
| name| 必选 | String | 通道名称|
| recordlist| 必选 | Array | 录像列表|
| - start| 必选 | String | 开始时间, YYYYMMDDHHmmss|
| - duration| 必选 | Number | 时间长度，单位：秒|
| - important| 必选 | Boolean | 重要标识|

   #### 3.2.9.7 删除单个录像记录
   ```GET　/streamapi/v1/cloudrecord/removerecord```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| period| 必选 | String | 录像时间, YYYYMMDDHHmmss|

   ```HTTP/1.1 200 OK```

   #### 3.2.9.8 删除通道所有录像信息
   ```GET　/streamapi/v1/cloudrecord/removeallrecord```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|

   ```HTTP/1.1 200 OK```

   #### 3.2.9.9 删除通道按日录像信息
   ```GET　/streamapi/v1/cloudrecord/removedailyrecord```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| period| 必选 | String | 录像时间, YYYYMMDDHHmmss|

   ```HTTP/1.1 200 OK```

   #### 3.2.9.10 设置录像文件重要标记
   ```GET　/streamapi/v1/cloudrecord/removeimportantflag```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| period| 必选 | String | 录像时间, YYYYMMDDHHmmss|
| important| 必选 | Boolean | 重要标记|

   ```HTTP/1.1 200 OK```

   #### 3.2.9.11 启动录像文件下载
   ```GET　/streamapi/v1/cloudrecord/downloadfile```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| period| 必选 | String | 录像时间, YYYYMMDDHHmmss|

   ```HTTP/1.1 200 OK```

   #### 3.2.9.12 合并下载录像mp4文件
   ```GET　/streamapi/v1/cloudrecord/downloadmp4file```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| deivceid| 必选 | String | 设备标识|
| channelid| 必选 | String | 通道标识|
| starttime| 必选 | String | 录像时间, YYYYMMDDHHmmss|
| stoptime| 必选 | String | 录像时间, YYYYMMDDHHmmss|

   ```HTTP/1.1 200 OK```

   # 4. 通信协议
   ## 4.1 协议格式
   >>> ``` 信令名称://目的地址 [ /路由地址/... [ ? 参数名=参数值 [ & ...] ] ]```

   ## 4.2 xmq_host_service
   
   ### 4.2.1 服务查询请求
   >>> ```query://xmq_host_service```

   ### 4.2.2 服务查询应答
   >>> ```query://video_cv_host_service?name=dvs_host_service&name=video_cv_host_service```

   ## 4.3 dvs_host_service
   
   ### 4.3.1 注册请求
   >>> ```register://dvs_host_service?timestamp=*[&sequence=*]```

   ### 4.3.2 注册应答
   >>> ```register://xmq_host_service?timestamp=*[&sequence=*]```

   ### 4.3.3 服务查询请求
   >>> ```query://xmq_host_service[?&sequence=1]```

   ### 4.3.4 服务查询应答
   >>> ```query://dvs_host_service[?name=*[&name=*[&sequence=1]]]```
   
   ### 4.3.5 设备列表查询请求
   >>> ```config://dvs_host_service?from=client_uuid&command=query```
   
   ### 4.3.6 设备列表查询应答
   >>> ```config://client_uuid?from=dvs_host_service&command=query[&dvs=1_192.168.0.100_4_test[&dvs=*]]```
   
   ### 4.3.7 设备新增请求
   >>> ```config://dvs_host_service?from=client_uuid&command=add&ip=192.168.0.100&port=8000&user=admin&passwd=Vrc123456&name=test```
   
   ### 4.3.8 设备新增应答
   >>> ```config://client_uuid?from=dvs_host_service&command=add&error=0[&dvs=1_192.168.0.100_4_test]```
   
   ### 4.3.9 设备删除请求
   >>> ```config://dvs_host_service?from=client_uuid&command=remove&id=1```
   
   ### 4.3.10 设备删除应答
   >>> ```config://client_uuid?from=dvs_host_service&command=remove&error=0&id=1```

   # 5. 开发计划
   # 6. 测试计划
   # 7. 迭代计划
   # 8. 版本
   # 9. 授权
   # 10. 附表
   ## 10.1 第三方依赖库编译方法
   ### 10.1.1 boost
   >>> + ./bootstrap.sh

   >>> + ./b2 cxxflags=-fPIC cflags=-fPIC address-model=64 link=static threading=multi runtime-link=static

   >>> + ./b2 install

   ### 10.1.2 libzmq

   >>> + apt install gnutls-dev

   >>> + apt install pkg-config

   >>> + apt install libbsd-dev

   ### 10.1.3 glog
   >>> + cmake -S . -B build -G "Unix Makefiles"
   >>> + apt install libgoogle-glog-dev

   ### 10.1.4 hiredis

   ### 10.1.5 redis-plus-plus

   ### 10.1.6 ffmpeg
   >>> + apt install ubuntu-restricted-addons

   >>> + apt install va-driver-all vdpau-driver-all

   >>> + apt update & apt upgrade
   

   ## 10.2 依赖库
   ### 10.2.1 openssl
   >>> + apt install libssl-dev

   ### 10.2.2 srtp
   >>> + apt install libsrtp2-dev
   
   # Linux
   >>> + ./configure --prefix=/usr/local --enable-gpl --enable-static --enable-shared --enable-small --disable-swscale-alpha --disable-programs --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --disable-avdevice --disable-avfilter --disable-network --disable-vaapi --disable-vdpau --disable-everything --enable-decoder=h264 --enable-asm --enable-mmx --enable-mmxext --enable-sse --enable-sse2 --enable-sse3 --enable-ssse3 --enable-sse4 --enable-sse42 --enable-avx --enable-avx2 --enable-inline-asm --enable-x86asm --disable-iconv --disable-debug --enable-fma3 --enable-fma4 --enable-pic --extra-cflags=-fPIC --extra-cxxflags=-fPIC

   # Windows
   >>> pacman -S mingw-w64-x86_64-toolchain
   >>> pacman -S msys/make
   >>> pacman -S --needed base-devel mingw-w64-x86_64-toolchain
   >>> + ./configure --prefix=./x64 --target-os=win64 --arch=x86_64 --enable-gpl --enable-static --disable-shared --enable-small --disable-swscale-alpha --disable-programs --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --disable-avdevice --disable-avfilter --disable-network --disable-vaapi --disable-vdpau --disable-everything --enable-decoder=h264 --enable-asm --enable-mmx --enable-mmxext --enable-sse --enable-sse2 --enable-sse3 --enable-ssse3 --enable-sse4 --enable-sse42 --enable-avx --enable-avx2 --enable-inline-asm --disable-x86asm --disable-iconv --disable-debug --enable-fma3 --enable-fma4 --enable-pic --extra-cflags=-fPIC --extra-cxxflags=-fPIC --enable-cross-compile --enable-dxva2

	Usage: configure [options]
Options: [defaults in brackets after descriptions]

Help options:
  --help                   print this message
  --quiet                  Suppress showing informative output
  --list-decoders          show all available decoders
  --list-encoders          show all available encoders
  --list-hwaccels          show all available hardware accelerators
  --list-demuxers          show all available demuxers
  --list-muxers            show all available muxers
  --list-parsers           show all available parsers
  --list-protocols         show all available protocols
  --list-bsfs              show all available bitstream filters
  --list-indevs            show all available input devices
  --list-outdevs           show all available output devices
  --list-filters           show all available filters

Standard options:
  --logfile=FILE           log tests and output to FILE [ffbuild/config.log]
  --disable-logging        do not log configure debug information
  --fatal-warnings         fail if any configure warning is generated
  --prefix=PREFIX          install in PREFIX [/usr/local]
  --bindir=DIR             install binaries in DIR [PREFIX/bin]
  --datadir=DIR            install data files in DIR [PREFIX/share/ffmpeg]
  --docdir=DIR             install documentation in DIR [PREFIX/share/doc/ffmpeg]
  --libdir=DIR             install libs in DIR [PREFIX/lib]
  --shlibdir=DIR           install shared libs in DIR [LIBDIR]
  --incdir=DIR             install includes in DIR [PREFIX/include]
  --mandir=DIR             install man page in DIR [PREFIX/share/man]
  --pkgconfigdir=DIR       install pkg-config files in DIR [LIBDIR/pkgconfig]
  --enable-rpath           use rpath to allow installing libraries in paths
                           not part of the dynamic linker search path
                           use rpath when linking programs (USE WITH CARE)
  --install-name-dir=DIR   Darwin directory name for installed targets

Licensing options:
  --enable-gpl             allow use of GPL code, the resulting libs
                           and binaries will be under GPL [no]
  --enable-version3        upgrade (L)GPL to version 3 [no]
  --enable-nonfree         allow use of nonfree code, the resulting libs
                           and binaries will be unredistributable [no]

Configuration options:
  --disable-static         do not build static libraries [no]
  --enable-shared          build shared libraries [no]
  --enable-small           optimize for size instead of speed
  --disable-runtime-cpudetect disable detecting CPU capabilities at runtime (smaller binary)
  --enable-gray            enable full grayscale support (slower color)
  --disable-swscale-alpha  disable alpha channel support in swscale
  --disable-all            disable building components, libraries and programs
  --disable-autodetect     disable automatically detected external libraries [no]

Program options:
  --disable-programs       do not build command line programs
  --disable-ffmpeg         disable ffmpeg build
  --disable-ffplay         disable ffplay build
  --disable-ffprobe        disable ffprobe build

Documentation options:
  --disable-doc            do not build documentation
  --disable-htmlpages      do not build HTML documentation pages
  --disable-manpages       do not build man documentation pages
  --disable-podpages       do not build POD documentation pages
  --disable-txtpages       do not build text documentation pages

Component options:
  --disable-avdevice       disable libavdevice build
  --disable-avcodec        disable libavcodec build
  --disable-avformat       disable libavformat build
  --disable-swresample     disable libswresample build
  --disable-swscale        disable libswscale build
  --disable-postproc       disable libpostproc build
  --disable-avfilter       disable libavfilter build
  --disable-pthreads       disable pthreads [autodetect]
  --disable-w32threads     disable Win32 threads [autodetect]
  --disable-os2threads     disable OS/2 threads [autodetect]
  --disable-network        disable network support [no]
  --disable-dct            disable DCT code
  --disable-dwt            disable DWT code
  --disable-error-resilience disable error resilience code
  --disable-lsp            disable LSP code
  --disable-lzo            disable LZO decoder code
  --disable-mdct           disable MDCT code
  --disable-rdft           disable RDFT code
  --disable-fft            disable FFT code
  --disable-faan           disable floating point AAN (I)DCT code
  --disable-pixelutils     disable pixel utils in libavutil

Individual component options:
  --disable-everything     disable all components listed below
  --disable-encoder=NAME   disable encoder NAME
  --enable-encoder=NAME    enable encoder NAME
  --disable-encoders       disable all encoders
  --disable-decoder=NAME   disable decoder NAME
  --enable-decoder=NAME    enable decoder NAME
  --disable-decoders       disable all decoders
  --disable-hwaccel=NAME   disable hwaccel NAME
  --enable-hwaccel=NAME    enable hwaccel NAME
  --disable-hwaccels       disable all hwaccels
  --disable-muxer=NAME     disable muxer NAME
  --enable-muxer=NAME      enable muxer NAME
  --disable-muxers         disable all muxers
  --disable-demuxer=NAME   disable demuxer NAME
  --enable-demuxer=NAME    enable demuxer NAME
  --disable-demuxers       disable all demuxers
  --enable-parser=NAME     enable parser NAME
  --disable-parser=NAME    disable parser NAME
  --disable-parsers        disable all parsers
  --enable-bsf=NAME        enable bitstream filter NAME
  --disable-bsf=NAME       disable bitstream filter NAME
  --disable-bsfs           disable all bitstream filters
  --enable-protocol=NAME   enable protocol NAME
  --disable-protocol=NAME  disable protocol NAME
  --disable-protocols      disable all protocols
  --enable-indev=NAME      enable input device NAME
  --disable-indev=NAME     disable input device NAME
  --disable-indevs         disable input devices
  --enable-outdev=NAME     enable output device NAME
  --disable-outdev=NAME    disable output device NAME
  --disable-outdevs        disable output devices
  --disable-devices        disable all devices
  --enable-filter=NAME     enable filter NAME
  --disable-filter=NAME    disable filter NAME
  --disable-filters        disable all filters

External library support:

  Using any of the following switches will allow FFmpeg to link to the
  corresponding external library. All the components depending on that library
  will become enabled, if all their other dependencies are met and they are not
  explicitly disabled. E.g. --enable-libopus will enable linking to
  libopus and allow the libopus encoder to be built, unless it is
  specifically disabled with --disable-encoder=libopus.

  Note that only the system libraries are auto-detected. All the other external
  libraries must be explicitly enabled.

  Also note that the following help text describes the purpose of the libraries
  themselves, not all their features will necessarily be usable by FFmpeg.

  --disable-alsa           disable ALSA support [autodetect]
  --disable-appkit         disable Apple AppKit framework [autodetect]
  --disable-avfoundation   disable Apple AVFoundation framework [autodetect]
  --enable-avisynth        enable reading of AviSynth script files [no]
  --disable-bzlib          disable bzlib [autodetect]
  --disable-coreimage      disable Apple CoreImage framework [autodetect]
  --enable-chromaprint     enable audio fingerprinting with chromaprint [no]
  --enable-frei0r          enable frei0r video filtering [no]
  --enable-gcrypt          enable gcrypt, needed for rtmp(t)e support
                           if openssl, librtmp or gmp is not used [no]
  --enable-gmp             enable gmp, needed for rtmp(t)e support
                           if openssl or librtmp is not used [no]
  --enable-gnutls          enable gnutls, needed for https support
                           if openssl, libtls or mbedtls is not used [no]
  --disable-iconv          disable iconv [autodetect]
  --enable-jni             enable JNI support [no]
  --enable-ladspa          enable LADSPA audio filtering [no]
  --enable-libaom          enable AV1 video encoding/decoding via libaom [no]
  --enable-libaribb24      enable ARIB text and caption decoding via libaribb24 [no]
  --enable-libass          enable libass subtitles rendering,
                           needed for subtitles and ass filter [no]
  --enable-libbluray       enable BluRay reading using libbluray [no]
  --enable-libbs2b         enable bs2b DSP library [no]
  --enable-libcaca         enable textual display using libcaca [no]
  --enable-libcelt         enable CELT decoding via libcelt [no]
  --enable-libcdio         enable audio CD grabbing with libcdio [no]
  --enable-libcodec2       enable codec2 en/decoding using libcodec2 [no]
  --enable-libdav1d        enable AV1 decoding via libdav1d [no]
  --enable-libdavs2        enable AVS2 decoding via libdavs2 [no]
  --enable-libdc1394       enable IIDC-1394 grabbing using libdc1394
                           and libraw1394 [no]
  --enable-libfdk-aac      enable AAC de/encoding via libfdk-aac [no]
  --enable-libflite        enable flite (voice synthesis) support via libflite [no]
  --enable-libfontconfig   enable libfontconfig, useful for drawtext filter [no]
  --enable-libfreetype     enable libfreetype, needed for drawtext filter [no]
  --enable-libfribidi      enable libfribidi, improves drawtext filter [no]
  --enable-libglslang      enable GLSL->SPIRV compilation via libglslang [no]
  --enable-libgme          enable Game Music Emu via libgme [no]
  --enable-libgsm          enable GSM de/encoding via libgsm [no]
  --enable-libiec61883     enable iec61883 via libiec61883 [no]
  --enable-libilbc         enable iLBC de/encoding via libilbc [no]
  --enable-libjack         enable JACK audio sound server [no]
  --enable-libklvanc       enable Kernel Labs VANC processing [no]
  --enable-libkvazaar      enable HEVC encoding via libkvazaar [no]
  --enable-liblensfun      enable lensfun lens correction [no]
  --enable-libmodplug      enable ModPlug via libmodplug [no]
  --enable-libmp3lame      enable MP3 encoding via libmp3lame [no]
  --enable-libopencore-amrnb enable AMR-NB de/encoding via libopencore-amrnb [no]
  --enable-libopencore-amrwb enable AMR-WB decoding via libopencore-amrwb [no]
  --enable-libopencv       enable video filtering via libopencv [no]
  --enable-libopenh264     enable H.264 encoding via OpenH264 [no]
  --enable-libopenjpeg     enable JPEG 2000 de/encoding via OpenJPEG [no]
  --enable-libopenmpt      enable decoding tracked files via libopenmpt [no]
  --enable-libopenvino     enable OpenVINO as a DNN module backend
                           for DNN based filters like dnn_processing [no]
  --enable-libopus         enable Opus de/encoding via libopus [no]
  --enable-libpulse        enable Pulseaudio input via libpulse [no]
  --enable-librabbitmq     enable RabbitMQ library [no]
  --enable-librav1e        enable AV1 encoding via rav1e [no]
  --enable-librist         enable RIST via librist [no]
  --enable-librsvg         enable SVG rasterization via librsvg [no]
  --enable-librubberband   enable rubberband needed for rubberband filter [no]
  --enable-librtmp         enable RTMP[E] support via librtmp [no]
  --enable-libshaderc      enable GLSL->SPIRV compilation via libshaderc [no]
  --enable-libshine        enable fixed-point MP3 encoding via libshine [no]
  --enable-libsmbclient    enable Samba protocol via libsmbclient [no]
  --enable-libsnappy       enable Snappy compression, needed for hap encoding [no]
  --enable-libsoxr         enable Include libsoxr resampling [no]
  --enable-libspeex        enable Speex de/encoding via libspeex [no]
  --enable-libsrt          enable Haivision SRT protocol via libsrt [no]
  --enable-libssh          enable SFTP protocol via libssh [no]
  --enable-libsvtav1       enable AV1 encoding via SVT [no]
  --enable-libtensorflow   enable TensorFlow as a DNN module backend
                           for DNN based filters like sr [no]
  --enable-libtesseract    enable Tesseract, needed for ocr filter [no]
  --enable-libtheora       enable Theora encoding via libtheora [no]
  --enable-libtls          enable LibreSSL (via libtls), needed for https support
                           if openssl, gnutls or mbedtls is not used [no]
  --enable-libtwolame      enable MP2 encoding via libtwolame [no]
  --enable-libuavs3d       enable AVS3 decoding via libuavs3d [no]
  --enable-libv4l2         enable libv4l2/v4l-utils [no]
  --enable-libvidstab      enable video stabilization using vid.stab [no]
  --enable-libvmaf         enable vmaf filter via libvmaf [no]
  --enable-libvo-amrwbenc  enable AMR-WB encoding via libvo-amrwbenc [no]
  --enable-libvorbis       enable Vorbis en/decoding via libvorbis,
                           native implementation exists [no]
  --enable-libvpx          enable VP8 and VP9 de/encoding via libvpx [no]
  --enable-libwebp         enable WebP encoding via libwebp [no]
  --enable-libx264         enable H.264 encoding via x264 [no]
  --enable-libx265         enable HEVC encoding via x265 [no]
  --enable-libxavs         enable AVS encoding via xavs [no]
  --enable-libxavs2        enable AVS2 encoding via xavs2 [no]
  --enable-libxcb          enable X11 grabbing using XCB [autodetect]
  --enable-libxcb-shm      enable X11 grabbing shm communication [autodetect]
  --enable-libxcb-xfixes   enable X11 grabbing mouse rendering [autodetect]
  --enable-libxcb-shape    enable X11 grabbing shape rendering [autodetect]
  --enable-libxvid         enable Xvid encoding via xvidcore,
                           native MPEG-4/Xvid encoder exists [no]
  --enable-libxml2         enable XML parsing using the C library libxml2, needed
                           for dash demuxing support [no]
  --enable-libzimg         enable z.lib, needed for zscale filter [no]
  --enable-libzmq          enable message passing via libzmq [no]
  --enable-libzvbi         enable teletext support via libzvbi [no]
  --enable-lv2             enable LV2 audio filtering [no]
  --disable-lzma           disable lzma [autodetect]
  --enable-decklink        enable Blackmagic DeckLink I/O support [no]
  --enable-mbedtls         enable mbedTLS, needed for https support
                           if openssl, gnutls or libtls is not used [no]
  --enable-mediacodec      enable Android MediaCodec support [no]
  --enable-mediafoundation enable encoding via MediaFoundation [auto]
  --disable-metal          disable Apple Metal framework [autodetect]
  --enable-libmysofa       enable libmysofa, needed for sofalizer filter [no]
  --enable-openal          enable OpenAL 1.1 capture support [no]
  --enable-opencl          enable OpenCL processing [no]
  --enable-opengl          enable OpenGL rendering [no]
  --enable-openssl         enable openssl, needed for https support
                           if gnutls, libtls or mbedtls is not used [no]
  --enable-pocketsphinx    enable PocketSphinx, needed for asr filter [no]
  --disable-sndio          disable sndio support [autodetect]
  --disable-schannel       disable SChannel SSP, needed for TLS support on
                           Windows if openssl and gnutls are not used [autodetect]
  --disable-sdl2           disable sdl2 [autodetect]
  --disable-securetransport disable Secure Transport, needed for TLS support
                           on OSX if openssl and gnutls are not used [autodetect]
  --enable-vapoursynth     enable VapourSynth demuxer [no]
  --enable-vulkan          enable Vulkan code [no]
  --disable-xlib           disable xlib [autodetect]
  --disable-zlib           disable zlib [autodetect]

  The following libraries provide various hardware acceleration features:
  --disable-amf            disable AMF video encoding code [autodetect]
  --disable-audiotoolbox   disable Apple AudioToolbox code [autodetect]
  --enable-cuda-nvcc       enable Nvidia CUDA compiler [no]
  --disable-cuda-llvm      disable CUDA compilation using clang [autodetect]
  --disable-cuvid          disable Nvidia CUVID support [autodetect]
  --disable-d3d11va        disable Microsoft Direct3D 11 video acceleration code [autodetect]
  --disable-dxva2          disable Microsoft DirectX 9 video acceleration code [autodetect]
  --disable-ffnvcodec      disable dynamically linked Nvidia code [autodetect]
  --enable-libdrm          enable DRM code (Linux) [no]
  --enable-libmfx          enable Intel MediaSDK (AKA Quick Sync Video) code via libmfx [no]
  --enable-libnpp          enable Nvidia Performance Primitives-based code [no]
  --enable-mmal            enable Broadcom Multi-Media Abstraction Layer (Raspberry Pi) via MMAL [no]
  --disable-nvdec          disable Nvidia video decoding acceleration (via hwaccel) [autodetect]
  --disable-nvenc          disable Nvidia video encoding code [autodetect]
  --enable-omx             enable OpenMAX IL code [no]
  --enable-omx-rpi         enable OpenMAX IL code for Raspberry Pi [no]
  --enable-rkmpp           enable Rockchip Media Process Platform code [no]
  --disable-v4l2-m2m       disable V4L2 mem2mem code [autodetect]
  --disable-vaapi          disable Video Acceleration API (mainly Unix/Intel) code [autodetect]
  --disable-vdpau          disable Nvidia Video Decode and Presentation API for Unix code [autodetect]
  --disable-videotoolbox   disable VideoToolbox code [autodetect]

Toolchain options:
  --arch=ARCH              select architecture []
  --cpu=CPU                select the minimum required CPU (affects
                           instruction selection, may crash on older CPUs)
  --cross-prefix=PREFIX    use PREFIX for compilation tools []
  --progs-suffix=SUFFIX    program name suffix []
  --enable-cross-compile   assume a cross-compiler is used
  --sysroot=PATH           root of cross-build tree
  --sysinclude=PATH        location of cross-build system headers
  --target-os=OS           compiler targets OS []
  --target-exec=CMD        command to run executables on target
  --target-path=DIR        path to view of build directory on target
  --target-samples=DIR     path to samples directory on target
  --tempprefix=PATH        force fixed dir/prefix instead of mktemp for checks
  --toolchain=NAME         set tool defaults according to NAME
                           (gcc-asan, clang-asan, gcc-msan, clang-msan,
                           gcc-tsan, clang-tsan, gcc-usan, clang-usan,
                           valgrind-massif, valgrind-memcheck,
                           msvc, icl, gcov, llvm-cov, hardened)
  --nm=NM                  use nm tool NM [nm -g]
  --ar=AR                  use archive tool AR [ar]
  --as=AS                  use assembler AS []
  --ln_s=LN_S              use symbolic link tool LN_S [ln -s -f]
  --strip=STRIP            use strip tool STRIP [strip]
  --windres=WINDRES        use windows resource compiler WINDRES [windres]
  --x86asmexe=EXE          use nasm-compatible assembler EXE [nasm]
  --cc=CC                  use C compiler CC [gcc]
  --cxx=CXX                use C compiler CXX [g++]
  --objcc=OCC              use ObjC compiler OCC [gcc]
  --dep-cc=DEPCC           use dependency generator DEPCC [gcc]
  --nvcc=NVCC              use Nvidia CUDA compiler NVCC or clang []
  --ld=LD                  use linker LD []
  --metalcc=METALCC        use metal compiler METALCC [xcrun metal]
  --metallib=METALLIB      use metal linker METALLIB [xcrun metallib]
  --pkg-config=PKGCONFIG   use pkg-config tool PKGCONFIG [pkg-config]
  --pkg-config-flags=FLAGS pass additional flags to pkgconf []
  --ranlib=RANLIB          use ranlib RANLIB [ranlib]
  --doxygen=DOXYGEN        use DOXYGEN to generate API doc [doxygen]
  --host-cc=HOSTCC         use host C compiler HOSTCC
  --host-cflags=HCFLAGS    use HCFLAGS when compiling for host
  --host-cppflags=HCPPFLAGS use HCPPFLAGS when compiling for host
  --host-ld=HOSTLD         use host linker HOSTLD
  --host-ldflags=HLDFLAGS  use HLDFLAGS when linking for host
  --host-extralibs=HLIBS   use libs HLIBS when linking for host
  --host-os=OS             compiler host OS []
  --extra-cflags=ECFLAGS   add ECFLAGS to CFLAGS []
  --extra-cxxflags=ECFLAGS add ECFLAGS to CXXFLAGS []
  --extra-objcflags=FLAGS  add FLAGS to OBJCFLAGS []
  --extra-ldflags=ELDFLAGS add ELDFLAGS to LDFLAGS []
  --extra-ldexeflags=ELDFLAGS add ELDFLAGS to LDEXEFLAGS []
  --extra-ldsoflags=ELDFLAGS add ELDFLAGS to LDSOFLAGS []
  --extra-libs=ELIBS       add ELIBS []
  --extra-version=STRING   version string suffix []
  --optflags=OPTFLAGS      override optimization-related compiler flags
  --nvccflags=NVCCFLAGS    override nvcc flags []
  --build-suffix=SUFFIX    library name suffix []
  --enable-pic             build position-independent code
  --enable-thumb           compile for Thumb instruction set
  --enable-lto             use link-time optimization
  --env="ENV=override"     override the environment variables

Advanced options (experts only):
  --malloc-prefix=PREFIX   prefix malloc and related names with PREFIX
  --custom-allocator=NAME  use a supported custom allocator
  --disable-symver         disable symbol versioning
  --enable-hardcoded-tables use hardcoded tables instead of runtime generation
  --disable-safe-bitstream-reader
                           disable buffer boundary checking in bitreaders
                           (faster, but may crash)
  --sws-max-filter-size=N  the max filter size swscale uses [256]

Optimization options (experts only):
  --disable-asm            disable all assembly optimizations
  --disable-altivec        disable AltiVec optimizations
  --disable-vsx            disable VSX optimizations
  --disable-power8         disable POWER8 optimizations
  --disable-amd3dnow       disable 3DNow! optimizations
  --disable-amd3dnowext    disable 3DNow! extended optimizations
  --disable-mmx            disable MMX optimizations
  --disable-mmxext         disable MMXEXT optimizations
  --disable-sse            disable SSE optimizations
  --disable-sse2           disable SSE2 optimizations
  --disable-sse3           disable SSE3 optimizations
  --disable-ssse3          disable SSSE3 optimizations
  --disable-sse4           disable SSE4 optimizations
  --disable-sse42          disable SSE4.2 optimizations
  --disable-avx            disable AVX optimizations
  --disable-xop            disable XOP optimizations
  --disable-fma3           disable FMA3 optimizations
  --disable-fma4           disable FMA4 optimizations
  --disable-avx2           disable AVX2 optimizations
  --disable-avx512         disable AVX-512 optimizations
  --disable-aesni          disable AESNI optimizations
  --disable-armv5te        disable armv5te optimizations
  --disable-armv6          disable armv6 optimizations
  --disable-armv6t2        disable armv6t2 optimizations
  --disable-vfp            disable VFP optimizations
  --disable-neon           disable NEON optimizations
  --disable-inline-asm     disable use of inline assembly
  --disable-x86asm         disable use of standalone x86 assembly
  --disable-mipsdsp        disable MIPS DSP ASE R1 optimizations
  --disable-mipsdspr2      disable MIPS DSP ASE R2 optimizations
  --disable-msa            disable MSA optimizations
  --disable-mipsfpu        disable floating point MIPS optimizations
  --disable-mmi            disable Loongson MMI optimizations
  --disable-lsx            disable Loongson LSX optimizations
  --disable-lasx           disable Loongson LASX optimizations
  --disable-fast-unaligned consider unaligned accesses slow

Developer options (useful when working on FFmpeg itself):
  --disable-debug          disable debugging symbols
  --enable-debug=LEVEL     set the debug level []
  --disable-optimizations  disable compiler optimizations
  --enable-extra-warnings  enable more compiler warnings
  --disable-stripping      disable stripping of executables and shared libraries
  --assert-level=level     0(default), 1 or 2, amount of assertion testing,
                           2 causes a slowdown at runtime.
  --enable-memory-poisoning fill heap uninitialized allocated space with arbitrary data
  --valgrind=VALGRIND      run "make fate" tests through valgrind to detect memory
                           leaks and errors, using the specified valgrind binary.
                           Cannot be combined with --target-exec
  --enable-ftrapv          Trap arithmetic overflows
  --samples=PATH           location of test samples for FATE, if not set use
                           $FATE_SAMPLES at make invocation time.
  --enable-neon-clobber-test check NEON registers for clobbering (should be
                           used only for debugging purposes)
  --enable-xmm-clobber-test check XMM registers for clobbering (Win64-only;
                           should be used only for debugging purposes)
  --enable-random          randomly enable/disable components
  --disable-random
  --enable-random=LIST     randomly enable/disable specific components or
  --disable-random=LIST    component groups. LIST is a comma-separated list
                           of NAME[:PROB] entries where NAME is a component
                           (group) and PROB the probability associated with
                           NAME (default 0.5).
  --random-seed=VALUE      seed value for --enable/disable-random
  --disable-valgrind-backtrace do not print a backtrace under Valgrind
                           (only applies to --disable-optimizations builds)
  --enable-ossfuzz         Enable building fuzzer tool
  --libfuzzer=PATH         path to libfuzzer
  --ignore-tests=TESTS     comma-separated list (without "fate-" prefix
                           in the name) of tests whose result is ignored
  --enable-linux-perf      enable Linux Performance Monitor API
  --enable-macos-kperf     enable macOS kperf (private) API
  --disable-large-tests    disable tests that use a large amount of memory
  --disable-ptx-compression don't compress CUDA PTX code even when possible

# 5. <a name='Gstreamerplugins'></a>GStreamer插件编写

## 5.1 查看gst-template git模块的副本，以获取一个重要的工具和基本GStreamer插件的源代码模板。
       git clone https://gitlab.freedesktop.org/gstreamer/gst-template.git
   