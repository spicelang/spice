; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %1 = call i32 @_f__void__test()
  store i32 %1, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %2)
  %4 = load i32, i32* %result, align 4
  ret i32 %4
}

define internal i32 @_f__void__test() {
entry:
  %result = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 1, i32* %res, align 4
  %0 = load i32, i32* %res, align 4
  ret i32 %0
}

declare i32 @printf(i8*, ...)
