//
// Created by al24254 on 07/11/2025.
//

#ifndef NOVA_TOKEN_H
#define NOVA_TOKEN_H

enum class Token {
    String,
    Integer,
    Float,

    Type,
    Identifier,
    Keyword,

    Plus,
    Subtract,
    Multiply,
    Divide,
    Modulus,


};

enum class Instructions {
    Add,
    Sub,
    Mul,
    Div,

    AccessIn,
    StoreIn,
    LoadIn,

    Ret,
    Call,
    PrintInt,
    PrintFloat,
    PrintString,
};

enum class Types {
    u8, u16, u32, u64, u128, u256,
    i8, i16, i32, i64, i128, i256,
    f16, f32, f64, f128, f256,
    character, boolean, pointer, null // null = void, pointer = ptr type, I know that may be confusing
};

#endif //NOVA_TOKEN_H