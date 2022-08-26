## 事务

### 事务的性质(ACID)

```
原子性：事务中的操作要么都发生，要么都不发生
一致性：事务执行的前后，数据的完整性保持一致，且事务执行前后数据的总和不变
隔离性：一个事务的执行不能被其他事务干扰；(隔离性与隔离级别有关，不可能到100%)
持久性：一个事物完成后，它对数据库的改变是永久的，即使出现系统故障。
```



### 事务的基本操作

```mysql
#事务的开始，两条等效
start transaction;
begin;

#事务的提交
commit;

#回滚事务，回到事务起始状态或指定回滚点
#要想rollback生效，必须要commit
rollback;
rollback pointname;

#设置回滚点
savepoint pointname;

#删除回滚点
release savepoint pointname;

#事务的异常中断，相当于事务完全没执行

#设置隔离级别
set session transaction isolation level read uncommitted;

#查看隔离级别
select @@session.transaction_isolation;
```



### 并发执行事务的问题

```mysql
脏写：当多个事务并发写同一数据时，先执行的事务被后执行的事务覆盖
脏读：当一个事务A向数据库写数据，但还没提交或终止，另一个事务B就看到了A写的数据（与隔离级别有关）
不可重复读：一个事务对同一个数据项的多次读取，但在某两个读取之间，另一个事务B就更新了该数据项并提交了，导致获取的数据项前后不一致
幻读：一个事务需要前后两次统计，但在两次统计之间，另一个事务B插入了新的符合统计条件的记录，并提交了，导致前后两次统计的数据不一致
```



### 隔离级别

```
读未提交(read uncommitted)：	可能 幻读、不可重复读、脏读 -->脏写会卡死
读已提交(read commited)：	可能 幻读、不可重复读 -->可以避免脏读，看不到未提交的更新数据
可重复读(repeatable read)：	可能 幻读 -->可以避免不可重复读，看不到其他并发事务提交的更新数据；幻读：事务A查询表项X，事务B插入表项X的数据M并提交，事务A查询表项X，不会显示新插入的数据M，但是A如果尝试插入M也会失败
可串行化(serializable)：		最高级 -->会阻止写操作，所有并发产生的问题都可以避免
#均不会脏写
```



## MySQL操作

```mysql
#查看表属性
desc tablename;
show create table tablename;

#查看数据的条数，两种等效
select count(*) from tablename;
select count(1) from tablename;

#显示表属于哪个数据库
select database();

#显示MySQL版本
select version();
```

## MySQL变量

```mysql
#自定义变量
set @var_name = expr;
set @rownum = 0;

#获取自定义变量的值
select @var_name from table_name;
select @rownum from user;

#查看全部系统变量
show global variables;

#查看单个系统变量
show global variables like 'wait_timeout';

#设置系统变量
set global var_name = var_value;
set global wait_timeout = 604800;

#获取系统变量值
select @@wait_timeout from user;

#获取行号
select
	@rownum := @rownum + 1 as rownum,
	price
from
	(select @rownum := 0) as t, 'order_table'
order by
	price desc;
```

## 其他

```
MySQL8.0取消了.frm文件，只保留了ibd文件，在/var/lib/mysql目录下
从ibd文件中获取索引表结构: ibd2sdi --dump-file=***.txt  ***.ibd
```

