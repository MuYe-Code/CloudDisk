1、继承的三个概念

```
1. overload: 函数重载，发生在同一个类内部
2. oversee:  隐藏，发生在父子类之间，只要函数名相同，就是隐藏(尽量不使用)
3. override: 覆盖，发生在父子类之间，针对virtual函数
```



2、构造函数不能设定为虚函数

```
1、虚函数的执行依赖于虚函数表，但对象还没有实例化，不存在相应内存空间，因此就无法找到对应的虚表。
2、构造一个对象时，必须知道对象的实际类型，而虚函数行为时运行期间确定实际类型的。
```



3、多态的体现

```
当通过基类指针或基类的引用访问子类对象时，就会呈现动态多态。
直接通过子类对象访问体现的不是动态多态，而是覆盖的特性
```



4、

```
在构造函数和析构函数中调用虚函数时，只能表现为静态联编（静态多态）
```



5、抽象类（不能实例化的类）

```
1、定义了纯虚函数的类
2、定义类protected权限构造函数的类
```



6、虚拟继承

```c++
//虚拟继承时，需要在派生类中显示调用虚基类的构造函数。
class A{
public:
	A(long a) :_a(a) {};
private:
	long _a;
}
class B: virtual public A{
public:
	B(long a, long b)
	: A(a)
	, _b(b)
	{
	
	}
	
private:
	long _b;
}

class C: virtual public A{
public:
	C(long a, long c)
	: A(a)
	, _b(c)
	{
	
	}
	
private:
	long _c;
}

class D: public B, public C{
public:
	D(long a, long b, long c, long d)
	: B(a, b)
	: C(a, c)
	: A(a)    //若显式调用A的构造函数，会调用A的默认构造函数
	: _d(d)
	{
	
	}
	
private:
 	long _d;
}

//由于B和C都是虚拟继承，因此D中只有一份A，故不能由B,C分别调用A的构造函数，而需要D调用A的构造函数。
```



7

```
虚拟继承会在派生类中生成一个虚基类指针，若派生类中有虚函数，会产生一个虚函数指针。
普通继承会在派生类中生成虚函数指针，若派生类中有新的虚函数，会添加在第一个虚函数表中。
```



8、值语义和对象语义

![image-20220704144724607](C:\Users\Masker\Desktop\LectureNote.assets\image-20220704144724607-16569172606421.png)



9、shared_ptr的bug

```c++
//循环引用导致内存泄漏
class A{
shared_ptr<B> _b;
};

class B{
shared_ptr<A> _a;
}

shared_ptr<A> a(new A);
shared_ptr<B> b(new B);
a->_b = b; //托管B对象的sp引用计数加一
b->_a = a; //托管A对象的sp引用计数加一

//会导致无法调用析构函数(引用计数无法为0)
//解决方案weak_ptr
```



10、unique_ptr

```c++
//由于unique_ptr没有拷贝和赋值操作，因此只能使用直接初始化方式。
unique_ptr<int> a(new int());

//unique作为STL容器的元素时，不能直接进行传递，因为不可以拷贝和赋值
vector<unique_ptr<int>> a;
a.push_back(new int(20));	//error
a.emplace_back(new int(20));//ok
a.push_back(unique_ptr<int>(new int));	//ok

//会调用拷贝构造函数，因此也是错误的
vector<unique_ptr<int>> b{
    unique_ptr<int>(new int(20)),
    unique_ptr<int>(new int(20))
};
```



11、智能指针的误用

```c++
//函数返回值是裸指针时，将其转化为智能指针有风险
//因此应该返回智能指针而不是裸指针
class Point : 
public std::enable_shared_from_this<Point> 
{
public: 
    Point(int ix = 0, int iy = 0) 
    : _ix(ix) , _iy(iy) 
    { 
        cout << "Point(int = 0, int = 0)" << endl; 
    }
    
    void print() const 
    { 
        cout << "(" <<_ix << "," << _iy << ")" << endl
    }
    /* Point *addPoint(Point *pt) */ 
    shared_ptr<Point> addPoint(Point *pt) 
    { 
        _ix += pt->_ix; 
        _iy += pt->_iy; 
        //this指针是一个裸指针 
        /* return shared_ptr<Point>(this); */
        return shared_from_this(); 
    }
    ~Point() 
    { 
        cout << "~Point()" << endl; 
    } 
    
private: int _ix; int _iy; 
};


void test3() 
{ 
    shared_ptr<Point> sp1(new Point(1, 2)); 
    cout << "sp1 = "; 
    sp1->print(); 
    cout << endl; 
    
    shared_ptr<Point> sp2(new Point(3, 4)); 
    cout << "sp2 = "; 
    sp2->print(); 
    cout << endl; 
    
    shared_ptr<Point> sp3(sp1->addPoint(sp2.get())); 
    cout << "sp3 = "; 
    sp3->print(); 
}
```

```
reset函数应该传入一个未托管的裸指针，否则或造成重复释放
```



12、迭代器

```
迭代器类似于指针，对于某些容器，迭代器是一个类
对于双向迭代器，不支持<运算

容器适配器不支持迭代器：stack, queue, priority_queue
```



13、流

```
流都有对应的缓冲区，例如文件流，标准输入输出流

有缓冲区，则可以使用流迭代器

串IO的缓冲区在内存，而文件IO会操作磁盘，速度显著低于串IO
纯文件IO：
while(ifs >> word)
文件IO + 串IO：
while(getline(ifs, line)){
	istring string iss(line);
	string word;
	while(iss >> word, !iss.eof()){ //逗号表达式的值是最后一个表达式的值
		...
	}
}
```



14、函数指针

```c++
int func() { return 10; }

int (*pFunc)();				//pFunc是一个函数指针
pFunc = func;
pFunc();

typedef int (*pFunc1)();	//pFunc1是函数类型
pFunc1 f1 = func;
f1();

//函数名与函数指针的区别
printf("%p\n", &main);
printf("%p\n", main);
printf("%p\n", f1);
```



15、cache和buffer

```
cache: 更注重读操作
buffer: 更注重写操作
```

