; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@intArray = constant [2 x i32] [i32 1, i32 2]
@0 = private unnamed_addr constant [17 x i8] c"intArray[1]: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [2 x i32], align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store [2 x i32] zeroinitializer, [2 x i32]* %intArray, align 4
  store i32 1, i32* %0, align 4
  store i32 2, i32* %1, align 4
  %2 = load [2 x i32], [2 x i32]* @intArray, align 4
  store [2 x i32] %2, [2 x i32]* %intArray, align 4
  %3 = load [2 x i32], [2 x i32]* %intArray, align 4
  %4 = getelementptr inbounds [2 x i32], [2 x i32]* %intArray, i32 0, i32 1
  %5 = load i32, i32* %4, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i32 %5)
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

declare i32 @printf(i8*, ...)
