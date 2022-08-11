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
@anonymous.0 = global [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
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
  %12 = alloca i32, align 4
  %intVariable = alloca i32, align 4
  %13 = alloca ptr, align 8
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store double 0x40335992641B328B, ptr %0, align 8
  %16 = load double, ptr %0, align 8
  %17 = fmul double %16, -1.000000e+00
  store double %17, ptr %1, align 8
  %18 = load double, ptr %1, align 8
  store i32 64, ptr %2, align 4
  %19 = load i32, ptr %2, align 4
  %20 = call i32 (ptr, ...) @printf(ptr @0, i32 %19)
  store i32 32, ptr %3, align 4
  %21 = load i32, ptr %3, align 4
  %22 = call i32 (ptr, ...) @printf(ptr @1, i32 %21)
  store i32 16, ptr %4, align 4
  %23 = load i32, ptr %4, align 4
  %24 = call i32 (ptr, ...) @printf(ptr @2, i32 %23)
  store i32 64, ptr %5, align 4
  %25 = load i32, ptr %5, align 4
  %26 = call i32 (ptr, ...) @printf(ptr @3, i32 %25)
  store i8 13, ptr %6, align 1
  %27 = load i8, ptr %6, align 1
  store i32 8, ptr %7, align 4
  %28 = load i32, ptr %7, align 4
  %29 = call i32 (ptr, ...) @printf(ptr @4, i32 %28)
  store i8 65, ptr %8, align 1
  %30 = load i8, ptr %8, align 1
  store i32 8, ptr %9, align 4
  %31 = load i32, ptr %9, align 4
  %32 = call i32 (ptr, ...) @printf(ptr @5, i32 %31)
  store i32 64, ptr %10, align 4
  %33 = load i32, ptr %10, align 4
  %34 = call i32 (ptr, ...) @printf(ptr @6, i32 %33)
  store i32 1, ptr %11, align 4
  %35 = load i32, ptr %11, align 4
  %36 = call i32 (ptr, ...) @printf(ptr @8, i32 %35)
  %37 = load [7 x i32], ptr @anonymous.0, align 4
  store i32 224, ptr %12, align 4
  %38 = load i32, ptr %12, align 4
  %39 = call i32 (ptr, ...) @printf(ptr @9, i32 %38)
  store i32 123, ptr %intVariable, align 4
  store ptr %intVariable, ptr %13, align 8
  %40 = load ptr, ptr %13, align 8
  store i32 64, ptr %14, align 4
  %41 = load i32, ptr %14, align 4
  %42 = call i32 (ptr, ...) @printf(ptr @10, i32 %41)
  store i32 128, ptr %15, align 4
  %43 = load i32, ptr %15, align 4
  %44 = call i32 (ptr, ...) @printf(ptr @11, i32 %43)
  %45 = load i32, ptr %result, align 4
  ret i32 %45
}

declare i32 @printf(ptr, ...)
