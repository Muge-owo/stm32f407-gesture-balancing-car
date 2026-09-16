# Git 提交说明速查

这份文档用于帮助本项目统一 Git 提交说明。它参考 Conventional Commits 的基本写法，但以清楚、容易使用为优先，不要求为了格式而过度拆分或纠结。

## 1. 调用格式

提交前先将需要提交的文件加入暂存区：

```powershell
git add <文件或目录>
```

提交说明的基本格式为：

```powershell
git commit -m "<类型>(<模块>): <简短说明>"
```

例如：

```powershell
git commit -m "feat(i2c): 完成硬件 I2C 单字节及多字节收发"
```

`(<模块>)` 可以省略：

```powershell
git commit -m "docs: 补充项目接线说明"
```

## 2. 格式说明

一条提交说明通常由三部分组成：

```text
类型(模块): 简短说明
```

| 部分 | 是否必需 | 作用 |
| --- | --- | --- |
| 类型 | 是 | 说明这次改动的主要目的 |
| 模块 | 否 | 说明改动主要发生在哪个模块 |
| 简短说明 | 是 | 说明具体完成了什么 |

提交说明应该让以后阅读记录的人能够快速回答：

1. 这次改动属于什么性质？
2. 改动了哪个模块？
3. 具体完成或修复了什么？

## 3. 常用类型

### `feat`：新增功能

程序获得了以前没有的新能力时使用。

```text
feat(i2c): 添加多字节寄存器读取功能
feat(motor): 添加 TB6612 电机控制接口
feat(encoder): 添加编码器计数功能
```

### `fix`：修复错误

原来的程序存在错误，现在将其修复时使用。

```text
fix(i2c): 修复 BTF 等待条件写反的问题
fix(pid): 修复速度环积分方向错误
```

### `docs`：修改文档或说明

只修改 README、说明文档、接线说明或代码注释时使用。

```text
docs: 补充项目使用说明
docs(readme): 补充电源和接线说明
docs(i2c): 说明从机地址的传入格式
```

### `refactor`：重构代码

程序功能和行为没有改变，只是重新整理代码结构时使用。

```text
refactor(i2c): 整理多字节接收流程
refactor(motor): 提取左右电机公共控制函数
```

### `test`：添加或修改测试

编写模块测试、硬件验证程序或修改测试代码时使用。

```text
test(i2c): 添加 MPU6050 寄存器读取测试
test(motor): 添加左右电机正反转测试
```

### `chore`：工程维护和杂项调整

不直接改变程序功能的工程配置、文件整理和日常维护使用。

```text
chore(keil): 将 I2C 驱动加入工程
chore: 整理项目目录
chore: 更新 Git 忽略规则
```

### `style`：调整代码格式

只修改缩进、空格、换行或排版，不改变程序逻辑时使用。

```text
style: 统一代码缩进格式
style(i2c): 整理函数间空行
```

### `perf`：性能优化

优化执行速度、内存占用或 CPU 占用，但不改变对外功能时使用。

```text
perf(i2c): 使用 DMA 降低数据读取阻塞时间
perf(filter): 减少姿态滤波计算开销
```

### `build`：修改构建配置

修改编译器、链接器、依赖组件或构建脚本时使用。

```text
build(keil): 更新头文件搜索路径
build(keil): 添加标准外设库 I2C 组件
```

### `revert`：撤销改动

撤销之前的一次提交或功能改动时使用。

```text
revert: 撤销 I2C 时钟频率调整
```

## 4. 模块名称

模块名称不是固定的，只要能够准确描述改动范围即可。本项目可以优先使用以下名称：

```text
i2c
mpu6050
oled
motor
encoder
pid
balance
key
led
timer
scheduler
power
keil
readme
```

例如：

```text
feat(mpu6050): 添加加速度计原始数据读取
fix(motor): 修复右侧电机方向相反的问题
docs(power): 补充电池和降压模块接线说明
```

如果一次改动涉及整个项目，或者模块名称不能帮助理解，可以省略模块：

```text
docs: 更新项目开发进度
chore: 整理工程目录
```

## 5. 类型选择方法

不确定使用哪个类型时，可以按照下面的顺序判断：

```text
是否增加了新能力？
├─ 是 → feat
└─ 否
   ├─ 是否修复了错误？ → fix
   ├─ 是否只修改文档或注释？ → docs
   ├─ 是否只整理代码结构？ → refactor
   ├─ 是否添加或修改测试？ → test
   ├─ 是否只调整代码排版？ → style
   ├─ 是否优化性能？ → perf
   ├─ 是否修改构建配置？ → build
   └─ 其他工程维护 → chore
```

如果一次提交包含多种改动，选择其中最主要的目的。例如，既编写了 I2C 驱动，又将文件加入 Keil 工程，主要成果是新增 I2C 功能，因此使用：

```text
feat(i2c): 完成硬件 I2C 基础收发功能
```

## 6. 常用示例

```powershell
git commit -m "feat(i2c): 完成硬件 I2C 单字节及多字节收发"
git commit -m "test(mpu6050): 验证 WHO_AM_I 寄存器读取"
git commit -m "fix(i2c): 修复双字节接收 ACK 配置错误"
git commit -m "refactor(motor): 整理左右电机控制接口"
git commit -m "docs(readme): 补充平衡小车硬件清单"
git commit -m "chore(keil): 添加 MPU6050 驱动文件"
```

提交后可以查看最近一次提交：

```powershell
git log -1 --oneline
```

查看最近几次提交：

```powershell
git log --oneline -5
```

## 7. 注意事项

### 7.1 提交前先检查内容

```powershell
git status
git diff --cached
```

确认暂存区中的文件确实属于本次提交，避免将无关改动一起提交。

### 7.2 一次提交尽量只表达一个主要目的

建议：

```text
feat(mpu6050): 添加原始数据读取功能
```

不建议：

```text
修改了一些代码、文档、LED、I2C和电机
```

### 7.3 说明具体做了什么

建议：

```text
fix(i2c): 修复双字节接收时 POS 配置错误
```

不建议：

```text
fix: 修复问题
```

### 7.4 简短说明一般不加句号

提交说明是标题，不需要写成完整段落：

```text
feat(oled): 添加电池电压显示
```

### 7.5 不必为类型选择过度纠结

类型的作用是帮助理解和检索提交记录。只要内容真实、清楚，并且类型大致符合改动目的，就已经足够。

### 7.6 Tag 不等于每次 Commit

日常开发可以频繁提交 Commit。只有形成一个值得标记的稳定阶段或可发布版本时，才需要创建 Tag。

例如：

```text
v0.1.0  项目建立并完成基础外设测试
v0.2.0  MPU6050读取和姿态解算阶段完成
```

## 8. 快速参考

```text
feat      新增功能
fix       修复错误
docs      文档和注释
refactor  重构代码
test      测试代码
chore     工程维护
style     代码格式
perf      性能优化
build     构建配置
revert    撤销改动
```

最常用模板：

```powershell
git commit -m "feat(模块): 添加某项功能"
git commit -m "fix(模块): 修复某个问题"
git commit -m "docs: 补充某项说明"
git commit -m "refactor(模块): 整理某段代码"
git commit -m "test(模块): 添加某项测试"
git commit -m "chore: 完成某项工程维护"
```
