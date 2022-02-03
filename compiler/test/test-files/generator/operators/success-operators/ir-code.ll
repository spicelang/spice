; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %val = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %6 = load i32, i32* %0, align 4
  store i32 %6, i32* %val, align 4
  store i32 1, i32* %1, align 4
  %7 = load i32, i32* %1, align 4
  %8 = add i32 %7, 1
  store i32 %8, i32* %1, align 4
  %9 = load i32, i32* %1, align 4
  %10 = sub i32 %9, 1
  store i32 %10, i32* %1, align 4
  %11 = load i32, i32* %1, align 4
  store i32 2, i32* %2, align 4
  %12 = load i32, i32* %2, align 4
  %13 = mul i32 %11, %12
  store i32 %13, i32* %3, align 4
  store i32 2, i32* %4, align 4
  %14 = load i32, i32* %3, align 4
  %15 = load i32, i32* %4, align 4
  %16 = shl i32 %14, %15
  store i32 %16, i32* %5, align 4
  %17 = load i32, i32* %5, align 4
  %18 = load i32, i32* %val, align 4
  %19 = add i32 %18, %17
  store i32 %19, i32* %val, align 4
  %20 = load i32, i32* %val, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %20)
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}
