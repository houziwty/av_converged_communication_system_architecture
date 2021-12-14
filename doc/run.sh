# !/bin/bash

./xmq_host_service --local_port 50531 --pub_port 50927 --app_name xmq_host_service

./dvs_host_service --xmq_addr 127.0.0.1 --xmq_port 50531 --xms_port 50820 --app_name dvs_host_service

./test_dvs_host_client --xmq_addr 127.0.0.1 --xmq_port 50531 --app_name test_dvs_host_client