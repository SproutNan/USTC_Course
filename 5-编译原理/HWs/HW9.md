# 编译原理作业 H9

<center>PB20111686 黄瑞轩</center>

## 1

1. $pointer(array(0...2,integer))$
2. $array(0...2,pointer(integer))$
3. $pointer(array(0...2,interger))$
4. $pointer(array(0...2,pointer(interger)))$
5. $array(0...2,pointer(pointer(interger)))$
6. $array(0...2,pointer(array(0...1,pointer(interger))))$

## 5.6

`addType(id.entry, T.type)` 表示在符号表中注册。

`lookup(id.entry)` 表示在符号表中查询 id 的类型。

```C
P --> D ; E
D --> D ; D
D --> id : T		{addType(id.entry, T.type);}
T --> char			{T.type = char;}
T --> integer		{T.type = integer;}
T --> list of T1	{T.type = list(T1.type);}
E --> literal		{E.type = char;}
E --> num			{E.type = integer;}
E --> id			{E.type = lookup(id.entry);}
E --> (L)			{E.type = list(L.type);}
E --> nil			{E.type = nil;}
L --> E				{L.type = E.type;}
L --> E, L1			{L.type = (L1.type == E.type || E.type == nil) ? E.type : ERROR;}
```

## 5.12

`lookup(recordType, name)` 表示在 recordType 中查询名字为 name 的域类型。

```C
T --> record fields end		{T.type = record(fields.type);}
fields --> fields1 ; field	{fields.type = fields1.type × field.type;}
fields --> field			{fields.type = fields.type;}
field --> id : T			{field.type = id.name × T.type;}
E --> E1.id					{E.type = (E1.type == record(s × t)) ? 
    								lookup(E1.type, id.name) : ERROR;}
```

