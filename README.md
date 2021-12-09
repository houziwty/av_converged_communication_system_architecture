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

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| author| 必选 | String | 授权对象|
| hardware| 必选 | String | 硬件信息|
| interfaceversion| 必选 | String | 接口版本|
| apiauth| 必选 | Boolean | 是否接口鉴权|
| remaindays| 必选 | String | 授权剩余时间，单位：天|
| runtime| 必选 | String | 运行时间|
| servicetime| 必选 | String | 系统时间|
| starttime| 必选 | String | 服务启动时间|
| server| 必选 | String | 软件信息|
| preferstreamfmt| 必选 | String | 首选播放格式|
| channelcount| 必选 | Number | 通道总数|
| copyrighttext| 必选 | String | 版权信息|

   #### 3.2.1.5 修改密码
   ```GET　/streamapi/v1/system/modifypassword```
|参数名称|规则|类型|说明||
|-|-|-|-|-|
| oldpassword| 必选 | String | 旧密码|
| newpassword| 必选 | String | 新密码|
   
   ```HTTP/1.1 200 OK```

|参数名称|规则|类型|说明||
|-|-|-|-|-|
| timeout| 必选 | String | token超时，单位：秒|

   #### 3.2.1.6 重启服务
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
   >>> ``` 协议名称://目的地址 [ /路由地址/... [ ? 参数名=参数值 [ & ...] ] ]```

   ## 4.2 xmq_host_service
   ### 4.2.1 服务查询请求
   >>> 系统内个服务可以向XMQ服务查询系统内在线的服务信息。

   >>> ```query://xmq_host_service?from=video_cv_host_service```

   ### 4.2.2 服务查询应答
   >>> ```query://video_cv_host_service?name=dvs_host_service&name=video_cv_host_service```

   或者

   >>> ```query://video_cv_host_service?content_type=json&content=(序列化JSON字节流)```

   >>> content_type建议支持json、xml和protobuf。

   ## 4.3 dvs_host_service
   ### 4.3.1 注册请求
   >>> ```register://dvs_host_service?tick=(时间戳)[&sequence=1]```

   ### 4.3.2 注册应答
   >>> 无

   ### 4.3.3 设备查询请求
   >>> ```register://dvs_host_service?from=client_uuid[&sequence=1]```

   ### 4.3.4 设备查询应答
   >>> ```query://client_uuid?data_encoding=json&data=(序列化JSON字节流)[&sequence=1]```

   >>> data_encoding建议支持json、xml和protobuf。

   ### 4.3.5 设备配置请求
   >>> ```config://dvs_host_service?data_encoding=json&data=(序列化JSON字节流)[&sequence=1]```

   ### 4.3.6 设备配置应答
   >>> ```config://client_uuid?data_encoding=json&data=(序列化JSON字节流)[&sequence=1]```

   ### 4.3.7 通道实时流抓图请求
   >>> ```config://dvs_host_service?data_encoding=json&data=(序列化JSON字节流)[&sequence=1]```

   ### 4.3.8 通道实时流抓图应答
   >>> ```config://client_uuid?data_encoding=json&data=(序列化JSON字节流)[&sequence=1]```

   # 5. 开发计划
   # 6. 测试计划
   # 7. 迭代计划
   # 8. 版本
   # 9. 授权
   # 10. 附表
   ## 10.1 第三方依赖库编译方法
   ### 10.1.1 boost
   >>> + ./bootstrap.sh

   >>> + ./b2 cxxflags=-fPIC cflags=-fPIC link=static threading=multi runtime-link=static

   >>> + ./b2 install

   ### 10.1.2 libzmq
   >>> + apt install libzmq-dev

   ### 10.1.3 glog
   >>> + apt install libgoogle-glog-dev

   ### 10.1.4 ffmpeg
   >>> + apt install ubuntu-restricted-addons

   >>> + apt install va-driver-all vdpau-driver-all

   >>> + apt update & apt upgrade
   
   >>> + ./configure --prefix=/usr/local --enable-gpl --enable-static --enable-shared --enable-small --disable-swscale-alpha --disable-programs --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --disable-avdevice --disable-avfilter --disable-network --disable-vaapi --disable-vdpau --disable-everything --enable-decoder=h264 --enable-asm --enable-mmx --enable-mmxext --enable-sse --enable-sse2 --enable-sse3 --enable-ssse3 --enable-sse4 --enable-sse42 --enable-avx --enable-avx2 --enable-inline-asm --enable-x86asm --disable-iconv --disable-debug --enable-fma3 --enable-fma4 --enable-pic --extra-cflags=-fPIC --extra-cxxflags=-fPIC


   