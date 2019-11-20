# WindowsConsoleLibrary
windows win32 console library

#项目创建
vs2015  --> 新建项目 --> win32 控制台程序 -->空项目 
属性 字符集--> 多字节字符集

拉取类库后，属性 添加包含目录。 包含 include


#push_include.bat   //.sln 所在文件夹新建 push_include.bat文件。
cd LibraryReconstruct\include
git add .
git commit -m "script push"
git push origin master
echo "push complete"
pause


#pull_include.bat   //源文件所在文件夹新建 pull_include.bat文件。
if exist include (
	cd include
	git pull origin master
) else (
	git clone https://github.com/huangshangbin/WindowsConsoleLibrary.git
	ren "WindowsConsoleLibrary" "include"
)
echo "update complete"
pause
