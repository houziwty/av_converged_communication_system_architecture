# 1. xmq
>>> ./xmq_host_server --local_port 50531 --pub_port 50927 --app_name xmq_host_server

# 2. dvs_host_service
>>> ./dvs_host_server --xmq_addr 127.0.0.1 --xmq_port 50531 --xms_port 50820 --app_name dvs_host_server