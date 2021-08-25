symbol_list=[0, 'begin', 'end', 'integer', 'if', 'then', 'else', 'function','read', 'write', 10, 11, '=', '<>', '<=', '<', '>=','>', '-', '*', ':=', '(', ')', ';']

if __name__=='__main__':

    input=open('sample.pas','rb')
    output=open('sample.dyd','w')
    error=open('sample.err','w')

    idx=0
    line=1

    while True:

        error_type=0
        ch=input.read(1)
        idx+=1
        temp=b''

        if ch==b' ':         # 空格
            continue

        elif ch==b'\n' or ch==b'\r':      # 换行
            ch=input.read(1)
            temp=b'\n'
            line+=1
            idx=0
            output.write('{} {}\n'.format('EOLN'.rjust(16, ' '), 24))
            continue

        elif ch==b'':          # 文件结束
            temp=b''
            output.write('{} {}\n'.format('EOF'.rjust(16, ' '), 25))
            break

        elif ch.isalpha():    # 标识符
            while ch.isalnum():
                temp+=ch
                idx+=1
                ch=input.read(1)
                if len(temp)>16:
                    error_type=3
                    continue
            if ch != b'':
                input.seek(-1, 1)
                idx-=1

        elif ch.isdigit():  # 常数
            while ch.isdigit():
                temp+=ch
                idx+=1
                ch=input.read(1)
                input.seek(-1,1)
                idx-=1

        elif ch in [b'-', b'*', b'(', b')', b';']:
            temp=ch

        elif ch==b':':
            ch=input.read(1)
            idx+=1
            if ch==b'=':
                temp=b':='
            else:
                if ch!=b'':
                    input.seek(-1,1)
                    idx-=1
                error_type=1

        elif ch==b'>':
            ch=input.read(1)
            idx+=1
            if ch==b'=':
                temp=b'>='
            else:
                if ch!=b'':
                    input.seek(-1,1)
                    idx-=1

        elif ch==b'<':
            ch=input.read(1)
            idx+=1
            if ch==b'=':
                temp=b'>='
            else:
                if ch!=b'':
                    input.seek(-1,1)
                    idx-=1

        else:
            error_type=2
            # continue


        # 分析
        temp=temp.decode()
        if temp.isdigit():
            output.write('{} {}\n'.format(temp.rjust(16,' '),11))
            continue


        if temp in symbol_list:
            symbol_index=symbol_list.index(temp)
            output.write('{} {}\n'.format(temp.rjust(16,' '),symbol_index))
        else:
            output.write('{} {}\n'.format(temp.rjust(16,' '),10))

        if error_type:
            if error_type==1:
                error.write('LINE:{}  冒号不匹配\n'.format(line))
            elif error_type==2:
                ch=ch.decode()
                error.write('LINE:{}  非法字符\'{}\'\n'.format(line,ch))
            elif error_type==3:
                error.write('LINE:{}  标识符\'{}\'长度溢出\n'.format(line,temp))

    input.close()
    output.close()
    error.flush()
