
###sheduler
如果打算自己实现一个非抢占式的toy coroutine scheduler（协程当然只能是非抢占式的），大概会需要以下接口:

	create_sheduler,destory_sheduler

	create_coroutine,destory_coroutine

	coroutine_yield,coroutine_status

那么，为什么需要一个调度器呢?
实际上,对于每个协程来说，其他协程的信息它是不知道的，它只管获得cpu，执行代码, 交出cpu。
不过它何时获得cpu它自己是没法控制的。在一个有数以千记的协程的进程中，我们需要一个调度器，
来控制协程之间的的切换。就像操作系统需要负责调度进程运行一样。  


###message communication

接下来考虑下协程之间的通信，有两种方案；一种是类似于erlang的基于消息的通信方式，另一种是类似与go语言channel的通信方式。 我个人感觉消息通信的方式更加轻量，耦合度更低，处理起来应该也更灵活。