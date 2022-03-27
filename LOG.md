# LiteDB - Project Logging

![avatar](https://badgen.net/badge/Language/C++11/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)

## Task List

- [x] 基本表结构
- [x] 终端控制
- [x] 命令解析器
- [x] 命令执行器
- [x] B+Tree
- [X] 数据表创建
- [X] 数据插入
- [X] 数据查找
- [ ] 数据修改
- [ ] 数据删除
- [ ] 非主键索引
- [ ] 文件存取
- [ ] 外部接口

---
##### 2022.3.28
- 完成数据查找指令 **`SELECT`**
- 更新操作文档
- 数据插入Bug修复

---
##### 2022.3.27
- 完成表结构文件及数据文件读写
- 完成数据插入指令 **`INSERT`**

---
##### 2022.3.22
- 确定**LiteDB**数据类型 **`INT`**, **`INT64`**, **`REAL`**, **`TEXT`**, **`LONGTEXT`**
- 完成动态索引类 **`Index`** 构建, 详见 **`Index.h`** | **`Index.cpp`** 
   
---
##### 2022.3.20
- 完成B+树模板构建, 详见  **`BalanceTree.h`**
- 测试B+树节点插入 & 删除
- 测试叶结点指针

---

##### --过于久远--

---