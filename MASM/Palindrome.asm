assume cs:code,ds:data,es:extra

data segment
msg1 db "Palindrome$"
msg2 db "Not Palindrome$"
str db "malayalam$"
data ends

print macro msg
mov ah,09h
lea dx,msg
int 21h
int 3h
endm

extra segment
blk db 9 dup(?)
extra ends

code segment
start:mov ax,data
      mov ds,ax
      mov ax,extra
      mov es,ax
      lea si,str
      lea di,blk+8
      mov cx,0009h

back:cld
     lodsb
     std
     stosb
     loop back

     lea si,str
     lea di,blk
     mov cx,0009h
     cld
     repz cmpsb
     jnz skip
     print msg1
     jmp skip2
     skip:print msg2
     skip2:mov ah,4ch
     int 21h
code ends
end start
