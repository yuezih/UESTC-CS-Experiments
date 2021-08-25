#定义用于存储进程、内存、就绪队列、阻塞队列、运行信息的变量
global Process_List
global Ready_List
global Block_List
global Resource_Vector
global Running

Process_List=[]
Ready_List=[]
Block_List=[]
Resource_Vector=[]
Running='null'

# 进程控制块类
class PCB(object):
    def __init__(self):
        self.PID='' # process name
        self.occupied_resources=[0,0,0,0] # occupied resource
        self.status='' # type: ready, block, running
        self.creation_tree=['null'] # creation_tree[0]:parent, creation_tree[1,2,...]:childs
        self.priority=0 # 0,1,2 (Init,User,System)
        self.blocked_RID_index=-1 # blocked by which kind of resource. index=0 -> R1, index=1 -> R2...
        self.blocked_request_amount=0 # how much resource it needs to return ready status
# 资源类
class Resource(object):
    def __init__(self):
        self.total=0
        self.available=0

# 初始化资源表
def set_resource_vector():
    global Resource_Vector
    R1=Resource()
    R2=Resource()
    R3=Resource()
    R4=Resource()
    Resource_Vector=[R1,R2,R3,R4]
    for i in range(4):
        Resource_Vector[i].total=Resource_Vector[i].available=i+1
# 创建一个新进程
def Create(name, priority):
    global Process_List
    global Ready_List
    global Running
    global Creation_Tree
    # 同名判断
    for each in Process_List:
        if each.PID==name:
            print("Failed to create a new process. {} is already created!".format(name))
            return
    new=PCB()
    new.PID=name
    new.priority=int(priority)
    Process_List.append(new) #添加至进程列表

    # 连接父进程
    if Running=="null":
        new.creation_tree[0]=new.PID
    else:
        new.creation_tree[0]=Running
        # 为父进程添加子进程
        running=get_PCB(Running) # 获取当前正在运行的进程
        running.creation_tree.append(new.PID) # 将新进程添加进其父进程的子树中

    preempt_or_not(new.PID)
# 执行抢占或不抢占
def preempt_or_not(PID):
    global Running
    global Process_List
    # 决定进程状态

    process=get_PCB(PID)
    running=get_PCB(Running)
    # 若当前没有进程正在运行，则运行该进程
    if Running=='null':
        process.status='running'
        Running=process.PID
    # 根据优先级判断是否抢占
    else:
        running=get_PCB(Running)
        if running.priority<process.priority:
            process.status='running'
            running.status='ready'
            Running=process.PID
            insert_RL(running) # 将当前进程插入就绪队列
        else:
            process.status='ready'
            insert_RL(process) # 将新建进程插入就绪队列
# 将进程插入就绪队列
def insert_RL(process):
    global Ready_List
    # 往有序数列表中插入元素的算法
    insert_index=len(Ready_List)
    for i in range(len(Ready_List)):
        if Ready_List[i].priority < process.priority:
            insert_index=i
            break
    Ready_List.insert(insert_index,process)
# 将进程插入阻塞队列
def insert_BL(process):
    global Block_List
    insert_index=len(Block_List)
    for i in range(len(Block_List)):
        if Block_List[i].priority < process.priority:
            insert_index=i
            break
    Block_List.insert(insert_index,process)
# [子函数]释放进程占用的所有资源
def release_all_by_pid(PID):
    global Process_List
    global Ready_List
    global Block_List
    global Resource_Vector
    global Running

    process=get_PCB(PID)
    # 判断是否存在该进程
    if process=="null":
        print('Invalid requirement: there is no process which is named {} in the process list'.format(PID))
        return
    # 释放资源，更新资源向量表
    for i in range(4):
        Resource_Vector[i].available=Resource_Vector[i].available+process.occupied_resources[i]
        process.occupied_resources[i]=0

    return check_block_list()
# 释放当前进程占用的部分资源
def release_running(RID_index,amount):
    global Running
    global Resource_Vector
    global Process_List
    # 获取当前进程
    process=get_PCB(Running)

    if RID_index not in [0,1,2,3]:
        print('Invalid requirement: there is no resource named {}'.format(RID_index+1))
        return
    if process.occupied_resources[RID_index]<amount:
        print('Invalid requirement: process {} only occupies {} R{}'.format(process.PID,process.occupied_resources[RID_index],RID_index+1))
        return
    # 更新可用资源表与当前进程占用资源表
    Resource_Vector[RID_index].available+=amount
    process.occupied_resources[RID_index]-=amount
    # 检查阻塞队列，尝试唤醒进程
    check_block_list()
# 检查阻塞队列，尝试唤醒进程
def check_block_list():
    global Block_List
    global Resource_Vector

    wakeup=[]
    for each in Block_List:
        RID_index=each.blocked_RID_index
        if each.blocked_request_amount > Resource_Vector[RID_index].available:
            continue
        else:
            Block_List.remove(each) # 移出阻塞队列
            preempt_or_not(each.PID) # 判断是否抢占
            Resource_Vector[RID_index].available-=each.blocked_request_amount # 满足其请求，更新资源表
            # each.blocked_RID_index=-1  # 重置PCB中的阻塞资源种类（该属性暂时保留，等输出结果后再更改）
            each.blocked_request_amount=0 # 重置PCB中的阻塞资源需求量
            wakeup.append(each) # 添加至唤醒列表，以便反馈信息给用户
    return wakeup
# 撤销进程
def Destroy(PID):
    global Process_List
    global Ready_List
    global Block_List
    global Resource_Vector
    global Running

    process=get_PCB(PID) # 获取当前进程
    # 两级return: check_block_list() -> release_all_by_pid() -> wakeup
    wakeup=release_all_by_pid(PID)  #释放PID占用资源并获取唤醒进程

    Process_List.remove(process) # 从进程队列中移除
    # 若撤销正在运行的进程，则就绪队列首个进程换入
    if process.status=='running':
        Running=Ready_List[0].PID
    # 若撤销阻塞进程，则将其从阻塞队列中移除
    elif process.status=='blocked':
        Block_List.remove(process)
    # 若撤销就绪队列，则将其从就绪队列中移除
    elif process.status=='ready':
        Ready_List.remove(process)

    #递归撤销所有子孙进程
    for each in process.creation_tree[1:]:
        if(each=='null'):
            return
        else:
            Destroy(each)

    # 唤醒每一个可唤醒的进程（考虑一次唤醒多个进程的情况）
    for each in wakeup:
        if each in Process_List:
            print("release R{}. wake up process {}".format(each.blocked_RID_index+1,each.PID))
            each.blocked_RID_index = -1  # 重置PCB中的阻塞资源种类

# 运行中的进程请求资源
def Request(RID_index,amount):
    global Process_List
    global Ready_List
    global Block_List
    global Resource_Vector
    global Running
    # 检查指令是否合理
    if(Running=="null"):
        print('Invalid requirement: there is no running process')
        return
    if RID_index not in [0,1,2,3]:
        print('Invalid requirement: there is no resource named R{}'.format(RID_index+1))
        return
    running=get_PCB(Running)
    # 如果可用资源无法满足请求，则阻塞
    if(Resource_Vector[RID_index].available < amount):
        # print("Failed to request: there is no enough available R{} resource".format(RID_index+1))
        # 阻塞当前进程
        running.status='blocked'
        insert_BL(running)
        running.blocked_RID_index=RID_index
        running.blocked_request_amount=amount
        # 从就绪队列中选择一个进程运行
        last_running_PID=Running
        if len(Ready_List)==0:
            Running="null"
        else:
            Running=Ready_List[0].PID
            Ready_List[0].status='running'
            Ready_List.pop(0)
        print('process {} is running. process {} is blocked.'.format(Running,last_running_PID))
    # 如果可用资源可以满足请求，则分配
    else:
        running.occupied_resources[RID_index]+=amount
        Resource_Vector[RID_index].available-=amount
        print('process {} requests {} R{}'.format(Running,amount,RID_index+1))
# 超时
def time_out():
    global Running
    global Ready_List
    # 将当前运行的进程移至就绪队列
    running=get_PCB(Running)
    running.status='ready'
    if running.priority>1:
        running.priority-=1
    last_running_PID=Running
    if len(Ready_List)!=0:
        insert_RL(running)
        # 将就绪队列的第一个进程调整为运行状态
        Running=Ready_List[0].PID
        Ready_List[0].status="running"
        # 将替换超时进程从就绪队列中弹出
        Ready_List.pop(0)
        print("process {} is running. ".format(Running),end='')
        if last_running_PID!=Running:
            print("process {} is ready. ".format(last_running_PID),end='')
        print('')
# 通过PID获取进程对象
def get_PCB(PID):
    global Process_List
    for each in Process_List:
        if each.PID==PID:
            return each
    return "null"
# 打印可用资源表
def print_available_resource_list():
    global Resource_Vector
    for i in range(4):
        print('R{} {}'.format(i+1,Resource_Vector[i].available))
# 打印就绪队列
def print_ready_list():
    running=get_PCB(Running)
    print_running_flag=0
    # 按优先级依次降低的顺序输出就绪队列中的进程
    for p in range(2,-1,-1):
        print(p,end=':')
        # （按实验要求）将正在运行的进程作为就绪队列同等优先级的首位打印出来
        if running.priority==p:
            print('{}'.format(running.PID), end='')
            print_running_flag=1
        for i in range(len(Ready_List)):
            # 莫名其妙的if-else问就是为了保证输出格式！
            if Ready_List[i].priority==p:
                if print_running_flag and running.priority==p:
                    print('-',end='')
                    print_running_flag=0
                print(Ready_List[i].PID,end='')
                if i==len(Ready_List)-1:
                    break
                elif Ready_List[i+1].priority!=p:
                    break
            if Ready_List[i].priority==p:
                print('-',end='')
        print(end='\n')
# 打印阻塞队列
def print_block_list():
    for p in range(2,-1,-1):
        print(p,end=':')
        for i in range(len(Block_List)):
            if Block_List[i].priority==p:
                print(Block_List[i].PID,end='')
                if i==len(Block_List)-1:
                    break
                elif Block_List[i+1].priority!=p:
                    break
            if Block_List[i].priority==p:
                print('-',end='')
        print(end='\n')
# 打印各种资源下的阻塞情况
def print_list_block():
    global Block_List
    for i in range(4):
        print('R{} '.format(i+1),end='')
        print_line_flag=0
        for j in range(len(Block_List)):
            if Block_List[j].blocked_RID_index==i:
                if print_line_flag==1:
                    print('-',end='')
                    print_line_flag=0
                print(Block_List[j].PID,end='')
                if j==len(Block_List)-1:
                    break
                if print_line_flag==0:
                    print_line_flag=1
        print(end='\n')
# 打印进程信息
def print_pcb_info(PID):
    global Process_List

    process=get_PCB(PID)
    if process!="null":
        print('PCB information about {}:'.format(PID))
        print('* PID:\t\t\t\t\t{}'.format(PID))
        print('* priority:\t\t\t\t{}'.format(process.priority))
        print('* status:\t\t\t\t{}'.format(process.status))
        print('* parent:\t\t\t\t{}'.format(process.creation_tree[0]))
        print('* child:\t\t\t\t{}'.format(process.creation_tree[1:]))
        print('* Occupied Resource:\t{}'.format(process.occupied_resources))
    else:
        print('Invalid requirement: there is no process which is named {} in the process list'.format(PID))

if __name__=='__main__':
    set_resource_vector() # 初始化资源向量表
    command = []

    # =============终端输入方法=============
    # while(True):
    #     command=input("shell>").split()
    # =============终端输入方法=============

    # =============读取文件方法=============
    with open('input.txt') as f:
        instruction_list=[]
        instruction = f.readline()
        while instruction:
            instruction_list.append(instruction)
            instruction = f.readline()
    for instruction in instruction_list:
        command=instruction.split()
    # =============读取文件方法=============

        Running_PCB = get_PCB(Running)
        # 跳过空白指令
        if len(command)==0:
            continue
        # 读取init指令
        elif command[0]=='init' and len(command)==1:
            Create('init',0)
            print("{} process is running".format(Running))
        # 读取cr指令
        elif command[0]=='cr' and len(command)==3:
            if command[2] in ['0','1','2']:
                Create(command[1],command[2])
                new_PCB=get_PCB(command[1])
                print("process {} is running".format(Running))
        # 读取de指令
        elif command[0]=='de' and len(command)==2:
            if get_PCB(command[1])!="null":
                Destroy(command[1])
            else:
                print('Invalid requirement: {} is not in process list.'.format(command[1]))
        # 读取req指令
        elif command[0]=='req' and len(command)==3:
            if command[1] in ['R1','R2','R3','R4']:
                if command[2].isalnum():
                    amount=int(command[2])
                    if amount>=0:
                        RID_index = int(command[1][-1:]) - 1
                        Request(RID_index,amount)
                    else:
                        print('Invalid requirement: resource requirement amount must be natural number.')
                else:
                    print('invalid requirement: resource requirement amount must be integers.')
            else:
                print('Invalid requirement: {} is not a valid resource name.'.format(command[1]))
        # 读取rel指令
        elif command[0]=='rel' and len(command)==3:
            if command[1] in ['R1','R2','R3','R4']:
                if command[2].isalnum():
                    amount=int(command[2])
                    if amount>=0:
                        RID_index = int(command[1][-1:]) - 1
                        release_running(RID_index,amount)
                    else:
                        print('Invalid requirement: resource release amount must be natural number.')
                else:
                    print('invalid requirement: resource release amount must be integers.')
            else:
                print('Invalid requirement: {} is not a valid resource name.'.format(command[1]))
        # 读取to指令
        elif command[0]=='to' and len(command)==1:
            time_out()
        # 读取list ready / list res / list block指令
        elif command[0]=='list' and len(command)==2:
            if command[1]=='ready':
                print_ready_list()
            elif command[1]=='res':
                print_available_resource_list()
            elif command[1]=='block':
                print_list_block()
        # 读取pr指令
        elif command[0]=='pr' and len(command)==2:
            print_pcb_info(command[1])
        # 无效指令反馈
        else:
            print('Invalid command.')
