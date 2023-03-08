# sql 通用语法

DDL 定义语句
DML 操作语句
DQL 查询语句
DCL 权限语句

# DDL
## 查询

    查询所有数据库
    show databases;
    查询当前数据库
    select database();
    创建数据库
    create database if not exists itcast;
    create database if not exists itheima default charset utf8mb4;
    删除数据库
    drop database if exists itcast;
    使用数据库
    use itcast;
    查询表
    show tables;
    查看表结构
    desc xxx;

    创建表
    create table tb_user(
        id int comment '编号',
        name varchar(50) comment '姓名',
        age int comment '年龄',
        gender varchar(1) comment '性别'
    ) comment '用户表';

    show create table tb_user;

    修改表
    alter table tb_user add nickname varchar(20) comment '昵称';

# DML

    insert
    update
    delete

# DQL

    select 
    from
    where
    group by
    having
    order by
    limit