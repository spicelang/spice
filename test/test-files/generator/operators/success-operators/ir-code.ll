; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %val = alloca i32, align 4
  store i32 1, ptr %val, align 4
  %val1 = alloca i32, align 4
  store i32 1, ptr %val1, align 4
  %0 = load i32, ptr %val1, align 4
  %1 = add i32 %0, 1
  store i32 %1, ptr %val1, align 4
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = sub i32 %3, 1
  store i32 %4, ptr %2, align 4
  %5 = alloca i32, align 4
  store i32 %3, ptr %5, align 4
  %6 = load i32, ptr %5, align 4
  %7 = mul i32 %6, 2
  %8 = alloca i32, align 4
  store i32 %7, ptr %8, align 4
  %9 = load i32, ptr %8, align 4
  %10 = shl i32 %9, 2
  %11 = alloca i32, align 4
  store i32 %10, ptr %11, align 4
  %12 = load i32, ptr %11, align 4
  %13 = load i32, ptr %val, align 4
  %14 = add i32 %13, %12
  store i32 %14, ptr %val, align 4
  %15 = load i32, ptr %val, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @0, i32 %15)
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

declare i32 @printf(ptr, ...)
