# BigInteger
一个轻量级的大整数类(A lightweight large integer class)

# 用法
用法与 c++ 基础的整数类型一致。 
额外的方法：

```c++
//转为字符串，带符号
const std::string to_string()const;

//数位长度
const size_type size()const;

//比较大小
//equal: 0 ; this greater: 1 ;this smaller: -1
const int compare(const BigInteger& x)const;

//将符号位变为正
void abs();
```

# 复杂度
+ 空间复杂度
  + 数据储存使用的的是char储存一个十进制位
+ 时间复杂度
  + 构造
    + 复杂度都为O（n），n指数位长度
  + 加减法
    + 复杂度为O（n），减法常数较加法要大 
  + 乘法
    + 使用fft实现，具有O（nlogn）的复杂度
  + 除法
    + 使用fft+牛顿迭代法实现，具有O（nlogn）复杂度，但常数稍大。
   
# 正确性
代码经过大量的单元测试，包括固定样例测试与随机样例测试。
并且通过如下oj平台的题目测评
+ https://www.51nod.com/Challenge/Problem.html#problemId=1029
+ https://loj.ac/p/164
+ https://www.luogu.com.cn/problem/P1919
+ https://www.luogu.com.cn/problem/P5432
+ https://vjudge.net/problem/OpenJ_Bailian-2737/origin  

