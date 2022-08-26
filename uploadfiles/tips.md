1、new的返回值

```c++
//new操作失败的时候不会返回nullptr，而是抛出异常，因此对new的结果进行检查是无意义的
char *pstr = new char[10]();
if(nullptr == pstr)	//没有意义，有返回值则一定不为空
{
    //do something
}

//应该使用try_catch语句
try{
    char *pstr = new char[10]();
} catch(const bad_alloc & e) {
    return -1;	//exception handler
}

//也可以要求不抛出异常，而在new操作失败时返回nullptr
char *pstr = new (std::nothrow) char[10]();
if(nullptr == pstr)
{
    //do something
}
```



2、uninitialized_copy和copy

```c++
//uninitialized_copy用于未初始化的区域的复制，会调用构造函数
//char*和wchar_t*类型是特化版本，会调用memmove
//POD类型会调用STL的copy，是最有效率的复制手法
//非POD类型会调用construct函数依次构造，是最安全的复制手法

//copy则用于已经初始化过的区域的复制，不会调用构造函数

//构造函数与析构函数应该一一对应，因此两个函数的使用是不同的
```



3、异步与IO多路复用

```
异步是文件读写完成后通知用户
IO多路复用是文件可读写的时候通知用户
```

