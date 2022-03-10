# 1. xmq
>>> ./xmq_host_server --local_port 50531 --pub_port 50927 --app_name xmq_host_server

# 2. dvs_host_service
>>> ./dvs_host_server --xmq_addr 192.1687.2.73 --xmq_port 60531 --xms_port 60820

# 3. log_host_service
>>> ./log_host_server --xmq_addr 127.0.0.1 --xmq_port 60531 --name log_host_service_log

# 4. WSL2 port forwarding
>>> netsh interface portproxy add v4tov4 listenport=50927 connectaddress=127.0.0.1 connectport=50927 listenaddress=* protocol=tcp