������centos7
���libcurl�Ƿ���
rpm -qa | grep libcrul


ftp���
#��װ
yum install -y vsftpd

#���ÿ�������
systemctl enable vsftpd.service

#����
systemctl start vsftpd.service

#ֹͣ
systemctl stop vsftpd.service

#�鿴״̬
systemctl status vsftpd.service


����ϵͳ����
setsebool -P ftp_home_dir 1