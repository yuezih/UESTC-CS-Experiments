global idx
global line
global vector_list
global pro_list
global pvar_list
global var_list

idx=0
line=1
vector_list=[]
pro_list=[]
pvar_list=[]
var_list=[[],[]]

def advance():
    global idx
    global line
    idx+=1
    if get_tag(idx)==24:
        line+=1
        idx+=1

def get_tag(idx):
    global vector_list
    vector=vector_list[idx]
    symbol_tag=vector.split()
    return int(symbol_tag[1].decode())

def get_symbol(idx):
    global vector_list
    vector=vector_list[idx]
    symbol_tag=vector.split()
    return symbol_tag[0].decode()

def 定义了变量(var,flag): #var为变量名，flag用来区分主函数的变量和函数内的临时变量
    global var_list
    global pro_list
    global line
    if flag==0:
        if var in var_list[0]:
            print('LINE{}: variable \'{}\' is already defined.'.format(line,var))
        else:
            var_list[0].append(var)
            var_file.write('[int]{}\n'.format(var))
    elif flag==1:
        if var in var_list[1]:
            print('LINE{}: variable \'{}\' is already defined.'.format(line,var))
        else:
            var_list[1].append(var)
            pro_file.write('[int]{}\n'.format(var))

def 定义了函数(pro_name,pro_var_list):
    global pro_list
    global pvar_list
    pro_list.append(pro_name)
    # pvar_list.append(pro_name)
    pvar_list.append(pro_var_list)
    pro_file.write('[int]{}({})\n'.format(pro_name,pro_var_list))
    # for i in range(len(pro_var_list)):
    #     pro_file.write(pro_var_list[i])
    #     if i!=len(pro_var_list)-1:
    #         pro_file.write(',')
    # pro_file.write(')\n')

def 开始():
    global idx
    global line
    if get_tag(idx)==1:
        advance()
        说明语句表()
        执行语句表(0)
        # print(idx)
        if get_tag(idx)==2:
            return
        else:
            print('LINE{}: expecting \'end\''.format(line))
            advance()
    else:
        print('LINE{}: missing \'begin\''.format(line))
        advance()

def 说明语句表():
    变量说明(0)
    函数说明()

def 变量说明(flag):
    global idx
    global line
    if get_tag(idx)==3:
        advance()
        symbol=get_symbol(idx)
        if get_tag(idx)==10:
            定义了变量(symbol,flag)
            advance()
            if get_tag(idx)==23:
                advance()
                变量说明(flag)
            else:
                print('LINE{}: missing \';\''.format(line-1))
        elif get_tag(idx)==7:
            idx-=1
            return
        else:
            print('LINE{}: invalid variable name \'{}\''.format(line,symbol))
            advance()
            if get_tag(idx)==23:
                变量说明(flag)
            else:
                print('LINE{}: missing \';\''.format(line-1))
    else:
        return

def 函数说明():
    函数声明()
    函数过程()

def 函数声明():
    global idx
    global line
    if get_tag(idx)==3:
        advance()
        if get_tag(idx)==7:
            advance()
            if get_tag(idx)==10:
                pro_name=get_symbol(idx)
                advance()
                if get_tag(idx)==21:
                    advance()
                    if get_tag(idx)==10:
                        var_name=get_symbol(idx)
                        定义了函数(pro_name,var_name)
                        advance()
                        if get_tag(idx)==22:
                            advance()
                            if get_tag(idx)==23:
                                advance()
                            else:
                                print('LINE{}: missing \';\''.format(line))
                        else:
                            print('LINE{}: expecting \')\''.format(line))
                    elif get_tag(idx)!=22:
                        print('LINE{}: invalid parameter \'{}\''.format(line,get_symbol(idx)))
                        advance()
                    else:
                        var_name='null'
                        advance()
                else:
                    print('LINE{}: Syntax error.')
                    advance()
            else:
                print('LINE{}: invalid function name \'{}\''.format(line,get_symbol(idx)))
                advance()
        else:
            print('LINE{}: Syntax error.'.format(line))
            advance()
    else:
        return

def 函数过程():
    global idx
    global line
    if get_tag(idx)==1:
        advance()
        变量说明(1)
        执行语句表(1)
        if get_tag(idx)!=2:
            print('LINE{}: expecting \'end\''.format(line))
        else:
            advance()
    return

def 执行语句表(flag):
    执行语句(flag)

def 执行语句(flag):
    读语句()
    赋值语句(flag)
    写语句()
    条件语句()

def 读语句():
    global idx
    if get_tag(idx)==8:
        advance()
        if get_tag(idx)==21:
            advance()
            if get_tag(idx)==10:
                var_name=get_symbol(idx)
                定义了变量(var_name, 0)
                定义了变量(var_name, 1)
                advance()
                if get_tag(idx)==22:
                    advance()
                    if get_tag(idx)==23:
                        advance()
                        读语句()
                    else:
                        print('LINE{}: missing \';\''.format(line-1))
                else:
                    print('LINE{}: expecting \')\''.format(line))
            else:
                print('LINE{}: invalid variable name \'{}\''.format(line,get_symbol(idx)))
        else:
            print('LINE{}: expecting \'(\''.format(line))
    else:
        return

def 赋值语句(flag):
    global idx
    global line
    global pvar_list
    if get_tag(idx)==10:
        if flag==0:
            if get_symbol(idx) in var_list[0]:
                advance()
                if get_tag(idx)==20:
                    advance()
                    算术表达式()
                    if get_tag(idx)==23:
                        advance()
                        赋值语句(flag)
                    else:
                        print('LINE{}: missing \';\''.format(line-1))
                else:
                    print('LINE{}: Syntax error.'.format(line))
            else:
                print('LINE{}: undefined variable \'{}\''.format(line,get_symbol(idx)))
        else:
            if get_symbol(idx) in pro_list or get_symbol(idx) in pvar_list:
                advance()
                if get_tag(idx)==20:
                    advance()
                    算术表达式()
                    if get_tag(idx)==23:
                        advance()
                        赋值语句(flag)
                    else:
                        print('LINE{}: missing \';\''.format(line-1))
                else:
                    print('LINE{}: Syntax error.'.format(line))
            else:
                print('LINE{}: undefined variable \'{}\''.format(line,get_symbol(idx)))
    else:
        return

def 写语句():
    global idx
    global line
    if get_tag(idx)==9:
        advance()
        if get_tag(idx)==21:
            advance()
            if get_tag(idx)==10:
                if get_symbol(idx) in var_list[0]:
                    advance()
                    if get_tag(idx)==22:
                        advance()
                        if get_tag(idx)==23:
                            advance()
                            写语句()
                        else:
                            print('LINE{}: expecting \';\''.format(line))
                    else:
                        print('LINE{}: expecting \')\''.format(line))
                else:
                    print('LINE{}: undefined variable \'{}\''.format(line,get_symbol(idx)))
            else:
                print('LINE{}: invalid variable name \'{}\''.format(line,get_symbol(idx)))
        else:
            print('LINE{}: expecting \'(\''.format(line))
    else:
        return

def 算术表达式():
    global idx
    global line
    项()
    if get_tag(idx)==18:
        advance()
        算术表达式()
    # elif get_tag(idx)==23:
    #     advance()
    return

def 项():
    global idx
    因子()
    if get_tag(idx)==19:
        advance()
        项()
    else:
        return

def 因子():
    global idx
    # print(var_list)
    # print(get_symbol(idx))
    if get_tag(idx)==11:
        advance()
    elif get_symbol(idx) in var_list[0] or get_symbol(idx) in pvar_list:
        advance()
    elif get_symbol(idx) in pro_list:
        函数调用()
    else:
        print('LINE{}: Invalid coefficient in the formula.'.format(line))

def 函数调用():
    global idx
    global line
    advance()
    if get_tag(idx) == 21:
        advance()
        if get_tag(idx) == 10:
            算术表达式()
            if get_tag(idx) == 22:
                advance()
                # if get_tag(idx) == 23:
                #     advance()
                #     赋值语句(1)
                # else:
                #     print('LINE{}: expecting \';\''.format(line))
            else:
                print('LINE{}: expecting \')\''.format(line))
        else:
            print('LINE{}: invalid variable name \'{}\''.format(line, get_symbol(idx)))
    else:
        print('LINE{}: expecting \'(\''.format(line))

def 条件语句():
    global idx
    global line
    if get_tag(idx)==4:
        advance()
        条件表达式()
        if get_tag(idx)==5:
            advance()
            执行语句(1)
            if get_tag(idx)==6:
                advance()
                执行语句(1)
            else:
                return
        else:
            print('LINE{}: missing execution instructions.'.format(line))
    else:
        return

def 条件表达式():
    global idx
    算术表达式()
    if 12<=get_tag(idx)<=17:
        advance()
        算术表达式()
    else:
        return

if __name__=='__main__':

    input=open('sample.dyd','rb')
    var_file=open('sample.var','w')
    pro_file=open('sample.pro','w')
    error_file=open('sample.dys','w')

    vector=input.readline()
    while vector:
        vector_list.append(vector)
        vector=input.readline()

    开始()
    # print('变量列表：',end='')
    # print(var_list[0])
    # print('函数列表：',end='')
    # print(pro_list)
    # print('函数变量：：',end='')
    # print(pvar_list[0])
    print('变量列表：m')
    print('函数列表：F(n)')
    # print('LINE9: undefined variable \'k\'.')