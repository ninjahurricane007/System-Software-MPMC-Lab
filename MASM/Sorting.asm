assume cs:code, ds:data

data segment
num dw 0034h,0044h,0004h,0024h,0014h
count equ 06h
data ends

code segment
start:mov ax,data
      mov ds,ax
      mov dx,count-1

again1:mov cx,dx
       lea si,num

again2:mov ax,[si]
       cmp ax,[si+2]
       jl print
       xchg [si+2],ax
       xchg [si],ax

print:add si,02
      loop again2
      dec dx
      jnz again1
      mov ah,4ch
      int 21h

code ends
end start