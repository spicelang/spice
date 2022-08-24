; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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
  store i32 0, ptr %result, align 4
  %0 = alloca i32, align 4
  store i32 64, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = call i32 (ptr, ...) @printf(ptr @0, i32 %1)
  %3 = alloca i32, align 4
  store i32 32, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @1, i32 %4)
  %6 = alloca i32, align 4
  store i32 16, ptr %6, align 4
  %7 = load i32, ptr %6, align 4
  %8 = call i32 (ptr, ...) @printf(ptr @2, i32 %7)
  %9 = alloca i32, align 4
  store i32 64, ptr %9, align 4
  %10 = load i32, ptr %9, align 4
  %11 = call i32 (ptr, ...) @printf(ptr @3, i32 %10)
  %12 = alloca i8, align 1
  store i8 13, ptr %12, align 1
  %13 = load i8, ptr %12, align 1
  %14 = alloca i32, align 4
  store i32 8, ptr %14, align 4
  %15 = load i32, ptr %14, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @4, i32 %15)
  %17 = alloca i8, align 1
  store i8 65, ptr %17, align 1
  %18 = load i8, ptr %17, align 1
  %19 = alloca i32, align 4
  store i32 8, ptr %19, align 4
  %20 = load i32, ptr %19, align 4
  %21 = call i32 (ptr, ...) @printf(ptr @5, i32 %20)
  %22 = alloca i32, align 4
  store i32 64, ptr %22, align 4
  %23 = load i32, ptr %22, align 4
  %24 = call i32 (ptr, ...) @printf(ptr @6, i32 %23)
  %25 = alloca i32, align 4
  store i32 1, ptr %25, align 4
  %26 = load i32, ptr %25, align 4
  %27 = call i32 (ptr, ...) @printf(ptr @8, i32 %26)
  %28 = load [7 x i32], ptr @anonymous.0, align 4
  %29 = alloca i32, align 4
  store i32 224, ptr %29, align 4
  %30 = load i32, ptr %29, align 4
  %31 = call i32 (ptr, ...) @printf(ptr @9, i32 %30)
  %intVariable = alloca i32, align 4
  store i32 123, ptr %intVariable, align 4
  %32 = alloca ptr, align 8
  store ptr %intVariable, ptr %32, align 8
  %33 = load ptr, ptr %32, align 8
  %34 = alloca i32, align 4
  store i32 64, ptr %34, align 4
  %35 = load i32, ptr %34, align 4
  %36 = call i32 (ptr, ...) @printf(ptr @10, i32 %35)
  %37 = alloca i32, align 4
  store i32 128, ptr %37, align 4
  %38 = load i32, ptr %37, align 4
  %39 = call i32 (ptr, ...) @printf(ptr @11, i32 %38)
  %40 = load i32, ptr %result, align 4
  ret i32 %40
}

declare i32 @printf(ptr, ...)
