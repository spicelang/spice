; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %val = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %val, align 4
  store i32 1, i32* %0, align 4
  %5 = load i32, i32* %0, align 4
  %6 = add i32 %5, 1
  store i32 %6, i32* %0, align 4
  store i32 %5, i32* %1, align 4
  %7 = load i32, i32* %1, align 4
  %8 = sub i32 %7, 1
  store i32 %8, i32* %1, align 4
  store i32 %7, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = mul i32 %9, 2
  store i32 %10, i32* %3, align 4
  %11 = load i32, i32* %3, align 4
  %12 = shl i32 %11, 2
  store i32 %12, i32* %4, align 4
  %13 = load i32, i32* %4, align 4
  %14 = load i32, i32* %val, align 4
  %15 = add i32 %14, %13
  store i32 %15, i32* %val, align 4
  %16 = load i32, i32* %val, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %16)
  %18 = load i32, i32* %result, align 4
  ret i32 %18
}

declare i32 @printf(i8*, ...)
