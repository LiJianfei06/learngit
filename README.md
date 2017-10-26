git config --global user.name "LiJianfei06"

git config --global user.email "976491174@qq.com"

注意git config命令的--global参数，用了这个参数，表示你这台机器上所有的Git仓库都会使用这个配置，当然也可以对某个仓库指定不同的用户名和Email地址。



git init                             	新的仓库就被初始化了。 
  
git add readme.txt						命令可以将其加入跟踪，并同时放入暂存区。

git commit -m "20171026"				用命令git commit告诉Git，把文件提交到仓库



git log									可以查看到之前提交的历史记录。

git log --pretty=oneline				如果嫌输出信息太多，看得眼花缭乱的，可以试试加上--pretty=oneline参数



git reset --hard HEAD^					回退上一个版本  上上次是 HEAD^^，也可以写成 HEAD~2，以此类推。

git reset --hard 3628164				如果想要移动到某个指定的提交，也可以直接使用提交id，id 不用输全，前几位就够


git reflog 								Git提供了一个命令git reflog用来记录你的每一次命令

git diff HEAD -- readme.txt				提交后，用git diff HEAD -- readme.txt命令可以查看工作区和版本库里面最新版本的区别

git checkout -- readme.txt				就是让这个文件回到最近一次git commit或git add时的状态。

git rm test.txt							确实要从版本库中删除该文件，那就用命令git rm删掉，并且git commit


新建一个GitHub仓库后：

git remote add origin https://github.com/LiJianfei06/tensorflow_projects.git

修改后：

git push -u origin master				我们第一次推送master分支时，加上了-u参数，Git不但会把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的master分支关联起来，在以后的推送或者拉取时就可以简化命令

以后修改后：

git status

git add README.md

git commit -m "20171026"

git push origin master


git remote rm origin   					就可以把 myremote 这个远程仓库从列表中清除了。





git clone https://github.com/LiJianfei06/tensorflow_projects.git			从远程库克隆

cd tensorflow_projects