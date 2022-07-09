; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [14 x i8] c"Result 1: %d\0A\00", align 1
@1 = private unnamed_addr constant [14 x i8] c"Result 2: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %2 = add i32 %1, 2
  store i32 %2, i32* %0, align 4
  %3 = load i32, i32* %0, align 4
  %4 = mul i32 %3, 2
  store i32 %4, i32* %0, align 4
  %5 = load i32, i32* %0, align 4
  %6 = sdiv i32 %5, 2
  store i32 %6, i32* %0, align 4
  %7 = load i32, i32* %0, align 4
  %8 = sub i32 %7, 2
  store i32 %8, i32* %0, align 4
  %9 = load i32, i32* %0, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0, i32 0, i32 0), i32 %9)
  %11 = load i32, i32* %0, align 4
  %12 = add i32 %11, 494665727
  store i32 %12, i32* %0, align 4
  %13 = load i32, i32* %0, align 4
  %14 = sdiv i32 %13, 2
  store i32 %14, i32* %0, align 4
  %15 = load i32, i32* %0, align 4
  %16 = mul i32 %15, 2
  store i32 %16, i32* %0, align 4
  %17 = load i32, i32* %0, align 4
  %18 = sub i32 %17, 494665727
  store i32 %18, i32* %0, align 4
  %19 = load i32, i32* %0, align 4
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @1, i32 0, i32 0), i32 %19)
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}

declare i32 @printf(i8*, ...)
