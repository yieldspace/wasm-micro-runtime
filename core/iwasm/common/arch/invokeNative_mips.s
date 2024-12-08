/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

    .text
    .align 2
    .globl invokeNative
    .ent invokeNative
    .type invokeNative, @function

/**
 * On function entry parameters:
 * $4 = func_ptr
 * $5 = args
 * $6 = arg_num
 */

invokeNative:
    .frame $fp, 8, $0
    .mask 0x00000000, 0
    .fmask 0x00000000, 0

    /* Fixed part of frame */
    addiu $sp, $sp, -8

    /* save registers */
    sw $31, 4($sp)
    sw $fp, 0($sp)

    /* set frame pointer to bottom of fixed frame */
    move $fp, $sp

    /* allocate enough stack space */
    sll $11, $6, 2  /* $11 = arg_num * 4 */
    addiu $sp, $sp, 0  # ダミー行 (必要なら削除)
    addiu $sp, $sp, 0  # ダミー行 (必要なら削除)
    # 上記ダミーはシンタックス的には不要だが説明用

    # arg_num * 4分だけspを下げる
    subu $sp, $sp, $11

    # make 8-byte aligned: マスク値をレジスタにロードしてANDする
    li $t0, -8
    and $sp, $sp, $t0

    move $9, $sp
    move $25, $4    /* $25 = func_ptr */

push_args:
    beq $6, $zero, done /* if arg_num == 0, jump done */
    lw $8, 0($5)    /* $8 = *args */
    sw $8, 0($9)     /* store $8 to stack */
    # args++, sp++ (4byte increment)
    addiu $5, $5, 4
    addiu $9, $9, 4
    addiu $6, $6, -1
    j push_args

done:
    # 以下は引数をロードする処理例
    # 本来は整数と浮動小数点で領域を分けるべきだが、ここでは形式修正のみ
    lw $4, 0($sp)
    lw $5, 4($sp)
    lw $6, 8($sp)
    lw $7, 12($sp)
    ldc1 $f12, 0($sp)
    ldc1 $f14, 8($sp)

    jalr $25
    nop

    /* restore saved registers */
    move $sp, $fp
    lw $31, 4($sp)
    lw $fp, 0($sp)

    /* pop frame */
    addiu $sp, $sp, 8

    j $31
    .end invokeNative
