assume cs:code,ds:data

data segment
op1 dw 0004h
op2 dw 0005h
res dw ?
data ends

code segment
start:mov ax,data
      mov ds,ax
      mov ax,op1
      mov bx,op2
      add ax,bx
      lea di,res
      mov [di],ax
      mov ah,4ch
      int 21h
code ends
end start