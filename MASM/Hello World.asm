assume cs:code,ds:data

data segment
msg db "Hello World$"
data ends

code segment
start:mov ax,data
      mov ds,ax
      lea dx,msg
      mov ah,09h
      int 21h
      mov ah,04h
      int 21h
code ends
end start