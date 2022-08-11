; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@intArray = global [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512]
@0 = private unnamed_addr constant [17 x i8] c"intArray[3]: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"intArray[7]: %d\0A\00", align 1
@2 = private unnamed_addr constant [17 x i8] c"intArray[9]: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = load [10 x i32], ptr @intArray, align 4
  %1 = load i32, ptr getelementptr inbounds ([10 x i32], ptr @intArray, i32 0, i32 3), align 4
  %2 = call i32 (ptr, ...) @printf(ptr @0, i32 %1)
  %3 = load [10 x i32], ptr @intArray, align 4
  %4 = load i32, ptr getelementptr inbounds ([10 x i32], ptr @intArray, i32 0, i32 7), align 4
  %5 = call i32 (ptr, ...) @printf(ptr @1, i32 %4)
  %6 = load [10 x i32], ptr @intArray, align 4
  %7 = load i32, ptr getelementptr inbounds ([10 x i32], ptr @intArray, i32 0, i32 9), align 4
  %8 = call i32 (ptr, ...) @printf(ptr @2, i32 %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

declare i32 @printf(ptr, ...)
