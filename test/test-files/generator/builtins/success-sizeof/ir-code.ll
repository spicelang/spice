; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Size of double: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Size of long: %d\0A\00", align 1
@4 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@5 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@6 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@7 = private unnamed_addr constant [13 x i8] c"Hello Spice!\00", align 1
@8 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@9 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1
@anonymous.0 = constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
@10 = private unnamed_addr constant [18 x i8] c"Size of int*: %d\0A\00", align 1
@11 = private unnamed_addr constant [29 x i8] c"Size of struct instance: %d\0A\00", align 1

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca double, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  %8 = alloca i8, align 1
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca [7 x i32], align 4
  %13 = alloca i32, align 4
  %intVariable = alloca i32, align 4
  %14 = alloca ptr, align 8
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store double 0x40335992641B328B, ptr %0, align 8
  %17 = load double, ptr %0, align 8
  %18 = fmul double %17, -1.000000e+00
  store double %18, ptr %1, align 8
  %19 = load double, ptr %1, align 8
  store i32 64, ptr %2, align 4
  %20 = load i32, ptr %2, align 4
  %21 = call i32 (ptr, ...) @printf(ptr @0, i32 %20)
  store i32 32, ptr %3, align 4
  %22 = load i32, ptr %3, align 4
  %23 = call i32 (ptr, ...) @printf(ptr @1, i32 %22)
  store i32 16, ptr %4, align 4
  %24 = load i32, ptr %4, align 4
  %25 = call i32 (ptr, ...) @printf(ptr @2, i32 %24)
  store i32 64, ptr %5, align 4
  %26 = load i32, ptr %5, align 4
  %27 = call i32 (ptr, ...) @printf(ptr @3, i32 %26)
  store i8 13, ptr %6, align 1
  %28 = load i8, ptr %6, align 1
  store i32 8, ptr %7, align 4
  %29 = load i32, ptr %7, align 4
  %30 = call i32 (ptr, ...) @printf(ptr @4, i32 %29)
  store i8 65, ptr %8, align 1
  %31 = load i8, ptr %8, align 1
  store i32 8, ptr %9, align 4
  %32 = load i32, ptr %9, align 4
  %33 = call i32 (ptr, ...) @printf(ptr @5, i32 %32)
  store i32 64, ptr %10, align 4
  %34 = load i32, ptr %10, align 4
  %35 = call i32 (ptr, ...) @printf(ptr @6, i32 %34)
  store i32 1, ptr %11, align 4
  %36 = load i32, ptr %11, align 4
  %37 = call i32 (ptr, ...) @printf(ptr @8, i32 %36)
  store [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], ptr %12, align 4
  %38 = load [7 x i32], ptr %12, align 4
  store i32 224, ptr %13, align 4
  %39 = load i32, ptr %13, align 4
  %40 = call i32 (ptr, ...) @printf(ptr @9, i32 %39)
  store i32 123, ptr %intVariable, align 4
  store ptr %intVariable, ptr %14, align 8
  %41 = load ptr, ptr %14, align 8
  store i32 64, ptr %15, align 4
  %42 = load i32, ptr %15, align 4
  %43 = call i32 (ptr, ...) @printf(ptr @10, i32 %42)
  store i32 128, ptr %16, align 4
  %44 = load i32, ptr %16, align 4
  %45 = call i32 (ptr, ...) @printf(ptr @11, i32 %44)
  %46 = load i32, ptr %result, align 4
  ret i32 %46
}

declare i32 @printf(ptr, ...)
