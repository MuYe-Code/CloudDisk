### 题目描述

(a) 使用ssh-keygen命令生成一对公钥和私钥，然后使用scp命令将公钥拷贝到另一台机器中，再使用ssh命令登录该机器，将刚才拷贝过来的公钥以追加模式加入到~/.ssh/authorized_keys中。

(b) 将今天的作业用 scp 拷贝到虚拟机中，然后使用相关的 git 命令将写好的作业上传到 gitee 仓库中。

### 答案

请在下面填写你的答案：

(a)

`Windows端`

``ssh-kengen` 	`scp C:\Users\Masker/.ssh/id_rsa.pub masker@192.168.150.128:~`

`Linux端`

`cat id_rsa.pub >> .ssh/authorized_keys`

(b)

`将作业拷贝到虚拟机`

`C:\Users\Masker\Desktop\homework\Linux>scp Linux_Day04_01.md Linux_Day04_02.md Linux_Day04_03.md masker@192.168.150.128:~`

`下载仓库`

`git clone git@gitee.com:cplusplus2020/cpp44_homework.git`

`添加文件`

`cp ~/44thCPP/Linux/Day4/44LinuxDay04_***_01.md ~/44thCPP/cpp44_homework/Linux_Day04/01`

`git add ***.md`

`git pull`

`git commit -m '提交信息`

`git push origin master`

