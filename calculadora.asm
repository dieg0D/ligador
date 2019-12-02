; ------------------------------------------
; Trabajo 2 de Software Basico
; Alumnos: Dieguito y Icarito
; "Peteca es peteca, yuca es yuca"
; "Unos jugan de frente, otros jugan de espaldas"
; ------------------------------------------


; ------------------------------------------
; Seção que contém os dados inicializados,
; Aqui colocamos as mensagens que serão mostradas ao usuário
; ------------------------------------------
section .data
  start_message         DB 'Ola, entre com seu nome: ', 0
  start_message_len     EQU $-start_message
  hola_message          DB 0xA, 0xD, '¡Hola!, '
  hola_message_len      EQU $-hola_message
  well_come_message     DB 'Bem-vindo ao programa CALC IA-32', 0xA, 0xD
  well_come_message_len EQU $-well_come_message
  choose_opt_message    DB 0xA, 0xD, 'Escolha uma opcao:', 0xA, 0xD
  choose_opt_message_len EQU $-choose_opt_message
  add_message           DB '- (1) SOMA', 0xA, 0xD
  add_message_len       EQU $-add_message
  sub_message           DB '- (2) SUBSTRACAO', 0xA, 0xD
  sub_message_len       EQU $-sub_message
  mult_message          DB '- (3) MULTIPLICACAO', 0xA, 0xD
  mult_message_len      EQU $-mult_message
  div_message           DB '- (4) DIVISAO', 0xA, 0xD
  div_message_len       EQU $-div_message
  mod_message           DB '- (5) MODULO', 0xA, 0xD
  mod_message_len       EQU $-mod_message
  zero_meia_message     DB '- (6) SAIR', 0xA, 0xD
  zero_meia_message_len EQU $-zero_meia_message
  menu_opt_message      DB 0xA, 0xD, 'Digite a opcao: ', 0
  menu_opt_message_len  EQU $-menu_opt_message
  input1_message        DB 'Digite o numero 1: ', 0
  input1_message_len    EQU $-input1_message
  input2_message        DB 'Digite o numero 2: ', 0
  input2_message_len    EQU $-input2_message
  result_message        DB 'O resultado eh: ', 0
  result_message_len    EQU $-result_message
; ------------------------------------------
; Seção que contém os dados não inicializados,
; Aqui colocamos os valores de input
; ------------------------------------------
section .bss
  user_name      RESB 50
  option         RESB 2
  enter_key      RESB 1
  input1         RESB 10
  input2         RESB 10
  result         RESB 10
; ------------------------------------------
; Section text contém o código do programa
; ------------------------------------------
section .text
  global _start:
; ------------------------------------------
; Início do Programa
; Mostra as mensagens de boas vindas
; ------------------------------------------
    _start:
      mov ECX, start_message
      mov EDX, start_message_len
      call @print

      mov ECX, user_name
      mov EDX, 50
      call @read

      mov ECX, hola_message
      mov EDX, hola_message_len
      call @print

      mov ECX, user_name
      mov EDX, 50
      call @print
; ------------------------------------------
; Menu Principal do Programa
; Executado logo após a execução das boas vindas
; ------------------------------------------
    _menu:
      mov ECX, well_come_message
      mov EDX, well_come_message_len
      call @print

      mov ECX, choose_opt_message
      mov EDX, choose_opt_message_len
      call @print

      mov ECX, add_message
      mov EDX, add_message_len
      call @print

      mov ECX, sub_message
      mov EDX, sub_message_len
      call @print

      mov ECX, mult_message
      mov EDX, mult_message_len
      call @print

      mov ECX, div_message
      mov EDX, div_message_len
      call @print

      mov ECX, mod_message
      mov EDX, mod_message_len
      call @print

      mov ECX, zero_meia_message
      mov EDX, zero_meia_message_len
      call @print

      mov ECX, menu_opt_message
      mov EDX, menu_opt_message_len
      call @print

      mov ECX, option                 ; Pega o Input da entrada
      mov EDX, 2
      call @read

      cmp BYTE [option], '1'          ; "switch case" do menu
      je _soma

      cmp BYTE [option], '2'
      je _substracao

      cmp BYTE [option], '3'
      je _multiplicacao

      cmp BYTE [option], '4'
      je _divisao

      cmp BYTE [option], '5'
      je _modulo

      cmp BYTE [option], '6'
      je _start_exit
; ------------------------------------------
; Fim do Menu
; ------------------------------------------


; ------------------------------------------
; Operações da calculadora
; ------------------------------------------
    _soma:
      call @get_numbers      

      mov ESI, input1
      call @to_integer     ; eax = input1

      mov EBX, EAX   ; ebx = input1

      mov ESI, input2
      call @to_integer     ; eax = input2

      add EAX, EBX   ; eax = input1 + input2

      call @show_result

      call @lixeiro
      jmp _menu

    _substracao:
      call @get_numbers

      mov ESI, input1
      call @to_integer     ; eax = input1

      mov EBX, EAX   ; ebx = input1

      mov ESI, input2
      call @to_integer     ; eax = input2

      sub EBX, EAX   ; ebx = input1 + input2
      mov EAX, EBX

      call @show_result

      call @lixeiro
      jmp _menu

    _multiplicacao:
      call @get_numbers
      
      mov ESI, input1
      call @to_integer     ; eax = input1

      mov EBX, EAX   ; ebx = input1

      mov ESI, input2
      call @to_integer     ; eax = input2

      mul EBX        ; eax = eax * ebx -> input2 = input2 * input1

      call @show_result

      call @lixeiro
      jmp _menu

    _divisao:
      call @get_numbers

      mov ESI, input1
      call @to_integer    ; eax = input1

      mov EBX, EAX  ; ebx = input1
      
      mov ESI, input2
      call @to_integer    ; eax = input2
      
      mov ECX, EAX  ; ecx = input2
      mov EAX, EBX  ; eax = input1

      xor EDX, EDX

      div ECX       ; eax = input1 / input2

      call @show_result

      call @lixeiro
      jmp _menu

    _modulo:
      call @get_numbers

      mov ESI, input1
      call @to_integer    ; eax = input1

      mov EBX, EAX  ; ebx = input1
      
      mov ESI, input2
      call @to_integer    ; eax = input2
      
      mov ECX, EAX  ; ecx = input2
      mov EAX, EBX  ; eax = input1

      xor EDX, EDX

      div ECX       ; eax = input1 / input2
      movzx EAX, DL

      call @show_result

      call @lixeiro
      jmp _menu

    _start_exit:
      mov EAX, 1
      mov EBX, 0
      int 80h
; ------------------------------------------
; Daqui pra baixo estão as funções auxiliares
; ------------------------------------------
; ------------------------------------------
; Função que pega os inputs para as ops
; ------------------------------------------
    @get_numbers:
      mov ECX, input1_message
      mov EDX, input1_message_len
      call @print

      mov ECX, input1
      mov EDX, 10
      call @read

      mov ECX, input2_message
      mov EDX, input2_message_len
      call @print

      mov ECX, input2
      mov EDX, 10
      call @read

      ret
; ------------------------------------------
; Função que mostra o resultado da operação
; ------------------------------------------
    @show_result:
      lea ESI, [result]
      mov ECX, 10
      call @to_string

      mov ECX, result_message
      mov EDX, result_message_len
      call @print

      mov ECX, result
      mov EDX, 10
      call @print

      mov ECX, enter_key
      mov EDX, 1
      call @read

      ret
; ------------------------------------------
; Função que Tira o lixo da memória
; ------------------------------------------
    @lixeiro:
      mov ECX,10           ;Tamanho do result na memoria
      lea EDX,[result]     ;aponta edx pro início do bloco de result  
      xor EAX,EAX          ;limpa EAX
      __loop:
      mov [EDX+ECX*4],EAX  ;enche o result com 0 até o tamanho dele
      dec ECX              ;diminui o contador e tambem muda a posição 
      jnz __loop           ;if ECX != 0 repete.
      mov [EDX+ECX*4],EAX  ;if ECX = 0 limpa a ultima posicao.
      ret                  ;CABOU.
  ; ---------------------------------------------------------
  ; Função generica para printar coisas XD
  ; ---------------------------------------------------------
    @print:
      mov EAX, 4
      mov EBX, 1
      int 80h

      ret
  ; ---------------------------------------------------------
  ; Função generica para leitura de dados
  ; ---------------------------------------------------------
    @read:
      mov EAX, 3
      mov EBX, 0
      int 80h
      ret
  ; ------------------------------------------------------
  ; Função que converte uma string para um número inteiro 
  ; ------------------------------------------------------
    @to_integer:
     
      sub EAX, EAX
      sub ECX, ECX

      .to_integer_convert:
        movzx EDX, BYTE [ESI] ; get char
        inc ESI

        cmp EDX, '0'
        jl .to_integer_exit
        cmp EDX, '9'
        jg .to_integer_exit

        sub EDX, '0' ; char - '0'
        imul EAX, 10 ; eax = eax * 10
        add EAX, EDX ; eax = eax + char - '0'

        jmp .to_integer_convert

      .to_integer_exit:
        ret
  ; ------------------------------------------
  ; Função que converte um número para uma string
  ; ------------------------------------------
    @to_string:
      add ESI, ECX
      dec ESI
      mov BYTE [ESI], 0
      mov EBX, 10

      .to_string_convert:
        xor EDX, EDX
        div EBX
        add DL, '0'
        dec ESI
        mov [ESI], DL
        test EAX, EAX
        jnz .to_string_convert

        ret