# ssh-copy-id

    ssh-copy-id -i ~/.ssh/id_rsa.pub wangli@192.168.3.236

# root 密码登录 ssh

    1. sudo su
    2. 
    grep -w "#PermitRootLogin" /etc/ssh/sshd_config && sed -i "s/#PermitRootLogin prohibit-password/PermitRootLogin yes/" /etc/ssh/sshd_config && systemctl restart sshd
    grep -w "#MaxSessions" /etc/ssh/sshd_config && sed -i "s/#MaxSessions 10/MaxSessions 100/" /etc/ssh/sshd_config && systemctl restart sshd
    3. passwd