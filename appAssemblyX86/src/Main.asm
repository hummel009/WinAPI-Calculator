format PE GUI 4.0
entry start

include 'win32a.inc'   
include 'Engine.asm' 
include 'AtoF.asm'    
include 'FtoA.asm'  
include 'AtoI.asm'   
include 'ItoA.asm'     

section '.text' code readable executable

start:
  invoke RegisterClass, wc                            
 
  invoke GetSystemMetrics, SM_CXSCREEN
  mov [screenWidth], eax

  invoke GetSystemMetrics, SM_CYSCREEN
  mov [screenHeight], eax
  
  ; getting X, Y to display window in center
  mov eax, [screenWidth]
  sub eax, [windowWidth]
  sar eax, 1 ; divide on 2     
  mov [windowX], eax

  mov eax, [screenHeight]
  sub eax, [windowHeight]
  sar eax, 1 ; divide on 2
  mov [windowY], eax

  invoke CreateWindowEx, 0, className, windowTitle, WS_VISIBLE + WS_CAPTION + WS_SYSMENU, [windowX], [windowY], [windowWidth], [windowHeight], NULL, NULL, NULL, NULL

; loop
cycle:
  invoke GetMessage, msg, NULL, 0, 0
  cmp eax, 0
  je exit
  
  invoke TranslateMessage, msg
  invoke DispatchMessage, msg
  
  jmp cycle 
; end loop 

exit:
  invoke ExitProcess, [msg.wParam]

; READY
proc WindowProc uses ebx esi edi, window, msg, wParam, lParam
  cmp [msg], WM_CREATE
  je .wmcreate
  cmp [msg], WM_COMMAND
  je .wmcommand
  cmp [msg], WM_CLOSE
  je .wmclose
  cmp [msg], WM_DESTROY
  je .wmdestroy
  
.defwindowproc:
  invoke DefWindowProc, [window], [msg], [wParam], [lParam]
  jmp .finish

.wmcreate:      
  stdcall RegisterField, [window]
  
  stdcall RegisterButtonD, [window], 0, buttonP, 0 + 1, 60 * 1 - 10  
  stdcall RegisterButtonD, [window], 1, buttonE, 60 + 1, 60 * 1 - 10  
  stdcall RegisterButton, [window], 2, buttonC, 120 + 1, 60 * 1 - 10  
  stdcall RegisterButton, [window], 3, buttonFactorial, 180 + 1, 60 * 1 - 10
  stdcall RegisterButtonD, [window], 4, buttonInverse, 0 + 1, 60 * 2 - 10  
  stdcall RegisterButton, [window], 5, buttonSquare, 60 + 1, 60 * 2 - 10  
  stdcall RegisterButtonD, [window], 6, buttonSquareRoot, 120 + 1, 60 * 2 - 10  
  stdcall RegisterButton, [window], 7, buttonDivide, 180 + 1, 60 * 2 - 10
  stdcall RegisterButton, [window], 8, button7, 0 + 1, 60 * 3 - 10  
  stdcall RegisterButton, [window], 9, button8, 60 + 1, 60 * 3 - 10  
  stdcall RegisterButton, [window], 10, button9, 120 + 1, 60 * 3 - 10  
  stdcall RegisterButton, [window], 11, buttonMultiple, 180 + 1, 60 * 3 - 10 
  stdcall RegisterButton, [window], 12, button4, 0 + 1, 60 * 4 - 10  
  stdcall RegisterButton, [window], 13, button5, 60 + 1, 60 * 4 - 10  
  stdcall RegisterButton, [window], 14, button6, 120 + 1, 60 * 4 - 10  
  stdcall RegisterButton, [window], 15, buttonMinus, 180 + 1, 60 * 4 - 10
  stdcall RegisterButton, [window], 16, button1, 0 + 1, 60 * 5 - 10  
  stdcall RegisterButton, [window], 17, button2, 60 + 1, 60 * 5 - 10  
  stdcall RegisterButton, [window], 18, button3, 120 + 1, 60 * 5 - 10  
  stdcall RegisterButton, [window], 19, buttonPlus, 180 + 1, 60 * 5 - 10 
  stdcall RegisterButton, [window], 20, buttonUnaryMinus, 0 + 1, 60 * 6 - 10  
  stdcall RegisterButton, [window], 21, button0, 60 + 1, 60 * 6 - 10  
  stdcall RegisterButtonD, [window], 22, buttonDot, 120 + 1, 60 * 6 - 10  
  stdcall RegisterButton, [window], 23, buttonEquals, 180 + 1, 60 * 6 - 10

  jmp .finish

.wmcommand:
  mov ax, word[wParam]
  mov [buttonId], ax
  
  cmp [buttonId], 0 ; pi
  jne @F
  stdcall PushSymbolWrapper, buttonPValue 
  jmp .finish
@@:
  cmp [buttonId], 1 ; euler
  jne @F 
  stdcall PushSymbolWrapper, buttonEValue  
  jmp .finish
@@:
  cmp [buttonId], 2 ; clear
  jne @F   
  invoke SetWindowText, [field], empty 
  ; TODO 
  jmp .finish
@@:
  cmp [buttonId], 3 ; factorial
  jne @F 
  stdcall PushOperation, buttonFactorial 
  jmp .finish
@@:
  cmp [buttonId], 4 ; inverse
  jne @F 
  stdcall PushOperation, buttonInverse 
  jmp .finish
@@:
  cmp [buttonId], 5 ; square
  jne @F 
  stdcall PushOperation, buttonSquare 
  jmp .finish
@@:
  cmp [buttonId], 6 ; square root
  jne @F 
  stdcall PushOperation, buttonSquareRoot 
  jmp .finish
@@:
  cmp [buttonId], 7 ; divide
  jne @F 
  stdcall PushOperation, buttonDivide  
  jmp .finish
@@:
  cmp [buttonId], 8 ; num 7
  jne @F 
  stdcall PushSymbolWrapper, button7
  jmp .finish
@@:
  cmp [buttonId], 9 ; num 8
  jne @F 
  stdcall PushSymbolWrapper, button8
  jmp .finish
@@:
  cmp [buttonId], 10 ; num 9
  jne @F 
  stdcall PushSymbolWrapper, button9 
  jmp .finish
@@:
  cmp [buttonId], 11 ; multiple
  jne @F 
  stdcall PushOperation, buttonMultiple  
  jmp .finish
@@:
  cmp [buttonId], 12 ; num 4
  jne @F 
  stdcall PushSymbolWrapper, button4 
  jmp .finish
@@:
  cmp [buttonId], 13 ; num 5
  jne @F 
  stdcall PushSymbolWrapper, button5
  jmp .finish
@@:
  cmp [buttonId], 14 ; num 6
  jne @F 
  stdcall PushSymbolWrapper, button6 
  jmp .finish
@@:
  cmp [buttonId], 15 ; minus
  jne @F 
  stdcall PushOperation, buttonMinus
  jmp .finish
@@:
  cmp [buttonId], 16 ; num 1
  jne @F 
  stdcall PushSymbolWrapper, button1
  jmp .finish
@@:
  cmp [buttonId], 17 ; num 2
  jne @F 
  stdcall PushSymbolWrapper, button2 
  jmp .finish
@@:
  cmp [buttonId], 18 ; num 3
  jne @F 
  stdcall PushSymbolWrapper, button3 
  jmp .finish
@@:
  cmp [buttonId], 19 ; plus
  jne @F 
  stdcall PushOperation, buttonPlus
  jmp .finish
@@:
  cmp [buttonId], 20 ; unary minus
  jne @F 
  stdcall PushSymbolWrapper, buttonUnaryMinus 
  jmp .finish
@@:
  cmp [buttonId], 21 ; num 0
  jne @F 
  stdcall PushSymbolWrapper, button0 
  jmp .finish
@@:
  cmp [buttonId], 22 ; dot
  jne @F 
  stdcall PushSymbolWrapper, buttonDot
  jmp .finish
@@:
  cmp [buttonId], 23 ; equals
  jne @F
  stdcall CalculateWrapper
@@:   
  jmp .finish

.wmclose:  
  invoke DestroyWindow, [window]
  jmp .finish
  
.wmdestroy:
  invoke PostQuitMessage, 0
  xor eax, eax
  
.finish:
  ret
endp
     
; TODO
proc CalculateWrapper  
  invoke GetWindowText, [field], buffer, 255
  cmp [storagePresence0], 1
  jne .resetData
  cmp [storagePresence1], 1
  jne .resetData 

  invoke lstrcmp, storage1, buttonPlus
  cmp eax, 0
  je .twoOperandAction 

  invoke lstrcmp, storage1, buttonMinus
  cmp eax, 0
  je .twoOperandAction 

  invoke lstrcmp, storage1, buttonMultiple
  cmp eax, 0
  je .twoOperandAction

  invoke lstrcmp, storage1, buttonDivide
  cmp eax, 0
  je .twoOperandAction 

  invoke lstrcmp, storage1, buttonFactorial
  cmp eax, 0
  je .oneOperandAction   

  invoke lstrcmp, storage1, buttonSquare
  cmp eax, 0
  je .oneOperandAction  

  invoke lstrcmp, storage1, buttonSquareRoot
  cmp eax, 0
  je .oneOperandAction 

  invoke lstrcmp, storage1, buttonInverse
  cmp eax, 0
  je .oneOperandAction 
  
  jmp .resetData
  
.twoOperandAction: 
  stdcall CountBufferDot
  cmp [quantity], 0
  jmp .pushAsIs ; REPLACE WITH JNE AFTER FLOAT SUPPORT
  
  invoke lstrcat, buffer, float

.pushAsIs:  
  stdcall PushItem, buffer
  
  invoke lstrcmp, storage1, buttonPlus
  cmp eax, 0
  je .doPlus 

  invoke lstrcmp, storage1, buttonMinus
  cmp eax, 0
  je .doMinus 

  invoke lstrcmp, storage1, buttonMultiple
  cmp eax, 0
  je .doMultiple

  invoke lstrcmp, storage1, buttonDivide
  cmp eax, 0
  je .doDivide

  jmp .resetData 

.doPlus:  
  stdcall ConvertFirstAtoI
  stdcall ConvertSecondAtoI
        
  finit
  fild dword[intStorage0]
  fiadd dword[intStorage2]
  fistp dword[intRes]

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData 
  
.doMinus:
  stdcall ConvertFirstAtoI
  stdcall ConvertSecondAtoI

  finit
  fild dword[intStorage0]
  fisub dword[intStorage2]
  fistp dword[intRes]

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData   
  
.doMultiple:
  stdcall ConvertFirstAtoI
  stdcall ConvertSecondAtoI

  finit
  fild dword[intStorage0]
  fimul dword[intStorage2]
  fistp dword[intRes]

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData
  
.doDivide: 
  stdcall ConvertFirstAtoI
  stdcall ConvertSecondAtoI
        
  finit
  fild dword[intStorage0]
  fidiv dword[intStorage2]
  fistp dword[intRes]

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData        

.oneOperandAction:

  invoke lstrcmp, storage1, buttonSquare
  cmp eax, 0
  je .doSquare 

  invoke lstrcmp, storage1, buttonInverse
  cmp eax, 0
  je .doInverse

  invoke lstrcmp, storage1, buttonFactorial
  cmp eax, 0
  je .doFactorial

  invoke lstrcmp, storage1, buttonSquareRoot
  cmp eax, 0
  je .doSquareRoot 

  jmp .resetData
  
.doSquare: 
  stdcall ConvertFirstAtoI
        
  finit
  fild dword[intStorage0]
  fimul dword[intStorage0]
  fistp dword[intRes]

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData

.doFactorial:
  stdcall ConvertFirstAtoI
	mov ecx, [intStorage0]
	mov eax, ecx
	
.cycle:
	cmp ecx, 1
	jle .brk
	
	sub ecx, 1
	mul ecx
	jmp .cycle
  
.brk:
  mov [intRes], eax  

  stdcall ConvertResItoA 
        
  invoke SetWindowText, [field], buffer
  jmp .resetData
  
.doInverse: 
  ; TODO 
  jmp .resetData
    
.doSquareRoot:
  ; TODO 
  
.resetData:
  mov [storagePresence0], 0
  mov [storagePresence1], 0  
  mov [storagePresence2], 0
  ret 
endp

; READY
proc PushSymbolWrapper uses eax, symbol 
  invoke GetWindowText, [field], buffer, 255
   
  invoke lstrcmp, [symbol], buttonPValue 
  cmp eax, 0
  je .checkFirst
  
  invoke lstrcmp, [symbol], buttonEValue 
  cmp eax, 0
  je .checkFirst 
  
  invoke lstrcmp, [symbol], buttonUnaryMinus 
  cmp eax, 0
  je .checkFirst 
  
  invoke lstrcmp, [symbol], buttonDot 
  cmp eax, 0
  je .dot
  
  jmp .other
   
.checkFirst: 
  invoke lstrlen, buffer    
  cmp eax, 0
  je .allow
  jmp .finish
   
.dot:
  invoke lstrlen, buffer               
  cmp eax, 0
  je .finish
                 
  stdcall CountBufferDot
  cmp [quantity], 0
  je .allow
  jmp .finish
  
.other: 
  cmp [storagePresence0], 0
  jne .checkOp
  
  jmp .allow

.checkOp:
  invoke lstrcmp, storage1, buttonFactorial
  cmp eax, 0
  je .finish  
  
  invoke lstrcmp, storage1, buttonSquare
  cmp eax, 0
  je .finish    
  
  invoke lstrcmp, storage1, buttonSquareRoot
  cmp eax, 0
  je .finish 
  
  invoke lstrcmp, storage1, buttonInverse
  cmp eax, 0
  je .finish

.allow:  
  stdcall PushSymbol, [symbol]
  
.finish:
  ret          
endp

; READY
proc PushSymbol uses eax, symbol
  invoke GetWindowText, [field], buffer, 255  
  invoke lstrcmp, buffer, error
  cmp eax, 0
  jne .concat
    
  invoke SetWindowText, [field], [symbol]
  jmp .finish
  
.concat:     
  invoke lstrcat, buffer, [symbol]
  invoke SetWindowText, [field], buffer
  
.finish:
  ret          
endp
    
; READY
proc PushItem, item
  cmp [storagePresence0], 0
  jne @F
  
  invoke lstrcpy, storage0, [item]
  mov [storagePresence0], 1
  jmp .finish
  
@@:
  cmp [storagePresence1], 0
  jne @F
  
  invoke lstrcpy, storage1, [item]
  mov [storagePresence1], 1
  jmp .finish

@@:
  cmp [storagePresence2], 0
  jne .finish
  
  invoke lstrcpy, storage2, [item]
  mov [storagePresence2], 1
  
.finish: 
  ret
endp

; READY
proc PushOperation, operation
  invoke GetWindowText, [field], buffer, 255
  cmp [storagePresence0], 0
  jne .error
  
  stdcall CountBufferDot
  cmp [quantity], 0
  jmp .pushAsIs ; REPLACE WITH JNE AFTER FLOAT SUPPORT
  
  invoke lstrcat, buffer, float

.pushAsIs:  
  stdcall PushItem, buffer
  stdcall PushItem, [operation]
  invoke SetWindowText, [field], empty
  jmp .finish     
  
.error:   
  mov [storagePresence0], 0
  mov [storagePresence1], 0  
  mov [storagePresence2], 0
  invoke SetWindowText, [field], error  
  
.finish:
  ret
endp  

; READY
proc RegisterButtonD, window, id, text, gridX, gridY   
  invoke CreateWindowEx, WS_EX_CLIENTEDGE, buttonClassName, [text], WS_TABSTOP + WS_VISIBLE + WS_CHILD + WS_DISABLED, [gridX], [gridY], 60, 60, [window], [id], NULL, NULL  
  ret
endp

; READY
proc RegisterButton, window, id, text, gridX, gridY   
  invoke CreateWindowEx, WS_EX_CLIENTEDGE, buttonClassName, [text], WS_TABSTOP + WS_VISIBLE + WS_CHILD, [gridX], [gridY], 60, 60, [window], [id], NULL, NULL  
  ret
endp

; READY
proc RegisterField uses eax, window   
  invoke CreateWindowEx, WS_EX_CLIENTEDGE, fieldClassName, fieldText, WS_TABSTOP + WS_VISIBLE + WS_CHILD, 1, 1, 239, 48, [window], NULL, NULL, NULL 
  mov [field], eax 
  ret
endp

; READY
proc CountBufferDot uses eax edi ecx  
  mov [quantity], 0 
   
  invoke lstrlen, buffer
  mov ecx, eax      
  mov edi, buffer 
  mov al, '.'     

; loop: find symbol
.cycle:
  repne scasb
  jnz .finish

  inc [quantity]
  jmp .cycle
; end loop

.finish:
  ret
endp

section '.data' data readable writeable

  className        db 'Hummel009''s Calculator', 0
  windowTitle      db 'Hummel009''s Calculator', 0
  buttonClassName  db 'BUTTON', 0      
  fieldClassName   db 'STATIC', 0      
  fieldText        db '', 0     
     
  quantity         db 0
       
  screenWidth      dd 0
  screenHeight     dd 0   
  windowWidth      dd 248
  windowHeight     dd 440
  windowX          dd 0
  windowY          dd 0

  buttonP          db 'P', 0      
  buttonE          db 'E', 0 

  buttonPValue     db '3.14', 0      
  buttonEValue     db '2.72', 0 
  
  buttonC          db 'C', 0
  
  buttonDot        db '.', 0 
  buttonUnaryMinus db '-', 0
  
  buttonFactorial  db 'x!', 0
  buttonInverse    db '1/x', 0
  buttonSquare     db 'x^2', 0
  buttonSquareRoot db 'sqrt(x)', 0
  
  buttonDivide     db '/', 0
  buttonMultiple   db '*', 0
  buttonMinus      db '-', 0
  buttonPlus       db '+', 0
  
  buttonEquals     db '=', 0
  
  button0          db '0', 0
  button1          db '1', 0
  button2          db '2', 0
  button3          db '3', 0
  button4          db '4', 0
  button5          db '5', 0
  button6          db '6', 0
  button7          db '7', 0
  button8          db '8', 0
  button9          db '9', 0  

  buffer           db 255 dup(?) 
  empty            db '', 0 
  error            db 'Error!', 0
                               
  radix            dd 10
  storage0            db 255 dup(?) 
  storage1            db 255 dup(?)
  storage2            db 255 dup(?)
  storagePresence0    db 0  
  storagePresence1    db 0  
  storagePresence2    db 0
  intStorage0         dd 0      
  intStorage2         dd 0
  intRes           dd 0
  
  localstorage0       db 255 dup(?)  
  localstorage2       db 255 dup(?)  
  
  float            db '.0', 0  
  
  buttonId         dw 0 
  field            dd 0

  itoa.digits                   db '0123456789ABCDEF', 0
  ftoa.ten                      dd 10           
  atof.ten                      dd 10

  wc  WNDCLASS 0, WindowProc, 0, 0, NULL, NULL, NULL, COLOR_WINDOW + 1, NULL, className
  
  msg MSG   

section '.idata' import data readable writeable

  library kernel32, 'KERNEL32.DLL', user32, 'USER32.DLL'

  include 'api\kernel32.inc'
  include 'api\user32.inc'