# SimBOT - A Simulator for performance variability from BOttom to Top

SimBOT是一个性能波动模拟器，用于模拟高性能计算集群中的不同统计分布特征的性能波动，探索性能波动对应用程序总体性能和扩展性的影响。

### 1 各类波动

+ 节点波动：虽然并行应用通常运行在多个相同种类的服务器上，但是由于服务器生产过程的微小差异、服务器所配置硬件的差异等，会导致不同的节点间有一定的性能差异。
+ 操作系统噪声：由于操作系统会有很多后台进程，这些后台进程周期性执行，因此操作系统会在并行应用执行时产生周期性的噪声。
+ 计算时间波动：同种计算会由于各种因素的影响导致时间会有一定波动。

### 2 使用方法

#### 2.1 环境要求

+ gcc
+ openmpi
+ gsl
+ openblas

#### 2.2 编译运行

###### 1）编译

```shell
cd pfhsim
make
```

###### 2）运行

```shell
mpirun -n <np> -c <path_to_config> -o <output_dir> -n <sim_times>
```

- -c, --config  配置文件路径
- -o, --output  所有输出文件目录
- -n, --ntime  模拟次数

示例：

```shell
mpirun -n 40 -c config.json -o output -n 100
```

#### 2.3 编写配置文件

###### 节点与核心数目配置

1）添加节点数、核心数

```json
{
    "node":{
    	"nodeNum": 40
    },
    "core":{
    	"coreNum": 64
	}
}
```

表示共有40个节点、每个节点配备64核心。

2）添加节点性能波动

```json
{
    "node":{
        "nodeNum": 40,
        "noise":{
            "noiseType": "normal",
            "noiseParameters": [0.0166666667,0.1]
        }
    },
    "core":{
        "coreNum":64
    }
}
```

表示40个节点间存在性能波动，该性能波动符合正太分布，正太分布的sigma为0.16666667，最大波动范围为0.1。

###### 操作系统配置

1）添加操作系统噪声

```json
{
    "osNoise":{
        "period": "5s",
        "detour": "0.2s"
    }
}
```

表示操作系统噪声周期为5s，噪声持续时间为0.2s。

2）无需操作系统噪声

```json
{
}
```

无需添加"osNoise"。

###### 模拟应用程序配置

配置包含三个部分：program, functions, kernels

+ program：包含一系列需要执行函数
+ functions：定义函数
+ kernels：定义计算核心

**1）简单示例：**

```json
{
    "program": ["fun1", "fun2"],
    
    "functions":{
        "fun1":{
            "call":["k1", "k2"]
        },
        "fun2":{
            "call": ["fun3"]
        },
        "fun3":{
        	"call": ["k1"]
    	}
    },
    
    "kernels":{
        "k1":{
            "runTime": "0.1s"
        },
        "k2":{
            "runTime": "0.2s"
        }
    }
    
}
```

该配置代表的代码结构如下：

```c
main(){
    fun1();
    fun2();
}
fun1(){
    k1();
    k2();
}
fun2(){
    fun3();
}
fun3(){
    k1();
}
```

runTime表示执行计算所需的标准时间。

**2）添加循环**

循环位置可以添加在"functions"中定义的函数中例如

```json
"functions":{
    "fun1":{
        "loop":{
            "loopNum":100,
            "call":["k1","k2"]
        }
    }
}
```

表示函数"fun1"循环执行"k1"、"k2"，100次。

**3）添加噪声**

噪声位置定义在计算kernel中，例如

```json
"kernels":{
    "k1":{
        "runTime": "0.1s",
        "noise":{
            "noiseType": "pareto",
            "noiseParameters": [10]
        }
    }
}
```

表示"k1"的计算时间波动符合帕累托分布且k值为10。

**4）添加同步点**

同步点可以添加到“function”中，例如

```json
"functions":{
    "fun1":{
        "loop":{
            "loopNum":100,
            "call":["k1","k2"]
        },
        "sync": "yes"
    }
}
```

表示每次执行完"fun1"后，会进行一次全局的同步。

**5）输出时间到out目录下**

可以通过"recordTime"标记来选择输出某个function或kernel的时间，示例如下。

```json
"functions":{
    "fun1":{
        "loop":{
            "loopNum":100,
            "call":["k1","k2"]
        },
        "sync": "yes",
        "recordTime": "yes"
    }
}
```

输出位置在(-o)参数指定目录下，例如run1, run2, run3.......run n，每一个文件代表第i次模拟的输出文件。

文件内容含义：

```
core0: func1:107.372 seconds    
core1: func1:106.183 seconds    
core2: func1:108.532 seconds    
core3: func1:105.519 seconds    
core4: func1:106.275 seconds    
core5: func1:107.226 seconds    
core6: func1:105.903 seconds    
core7: func1:106.897 seconds    

第一行core0: func1:107.372 seconds，表示模拟核心core0上，func1这个函数的模拟运行时间为107.372s
```



#### 2.3 生成函数调用文件

运行pfhsim后会在指定生成目录下生成一个structure.log文件，该文件是根据你所配置的json文件生成的函数调用过程。可以通过查看这个文件来判断json文件是否按照你的预期所配置的。

示例：

```c
main(){
    mainfun;
}

mainfun(){
    loop(10){
        func1;
        func2;
    }
}

func1(){
    k1;
}

func2(){
    loop(100){
        k2;
        fun3;
    }
}

fun3(){
    loop(1000){
        k3;
    }
}
```

**示例配置文件**

### 注意事项

+ 避免定义递归函数，它会导致模拟器陷入死循环。
+ 注意名词的拼写，拼写错误会导致模拟器识别错误，导致模拟器结果出现偏差。

