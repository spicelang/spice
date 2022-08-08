; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %val = alloca i32, align 4
  %val1 = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %val, align 4
  store i32 1, ptr %val1, align 4
  %4 = load i32, ptr %val1, align 4
  %5 = add i32 %4, 1
  store i32 %5, ptr %val1, align 4
  store i32 %4, ptr %0, align 4
  %6 = load i32, ptr %0, align 4
  %7 = sub i32 %6, 1
  store i32 %7, ptr %0, align 4
  store i32 %6, ptr %1, align 4
  %8 = load i32, ptr %1, align 4
  %9 = mul i32 %8, 2
  store i32 %9, ptr %2, align 4
  %10 = load i32, ptr %2, align 4
  %11 = shl i32 %10, 2
  store i32 %11, ptr %3, align 4
  %12 = load i32, ptr %3, align 4
  %13 = load i32, ptr %val, align 4
  %14 = add i32 %13, %12
  store i32 %14, ptr %val, align 4
  %15 = load i32, ptr %val, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @0, i32 %15)
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

declare i32 @printf(ptr, ...)
