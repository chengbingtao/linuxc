环境：centos7
检查libcurl是否按照
rpm -qa | grep libcrul


ftp相关
#安装
yum install -y vsftpd

#设置开机启动
systemctl enable vsftpd.service

#启动
systemctl start vsftpd.service

#停止
systemctl stop vsftpd.service

#查看状态
systemctl status vsftpd.service


更改系统设置
setsebool -P ftp_home_dir 1