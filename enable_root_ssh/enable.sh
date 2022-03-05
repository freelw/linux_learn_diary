#!/bin/bash

if [ `id -u` -eq 0 ];then
    echo 'root'
    grep -w "#PermitRootLogin" /etc/ssh/sshd_config && sed -i "s/#PermitRootLogin prohibit-password/PermitRootLogin yes/" /etc/ssh/sshd_config
    grep -w "#MaxSessions" /etc/ssh/sshd_config && sed -i "s/#MaxSessions 10/MaxSessions 100/" /etc/ssh/sshd_config
    systemctl restart sshd
    echo -e "oE9iwCfzRZs32g4dorrg\noE9iwCfzRZs32g4dorrg"|passwd  root
    ssh root@127.0.0.1
else
	echo 'not root'
fi
