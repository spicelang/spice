; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@testIntArray = global [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 0, i32 0, i32 0]
@0 = private unnamed_addr constant [18 x i8] c"Array length: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %testIntArray = alloca [7 x i32], align 4
  %0 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = load [7 x i32], ptr @testIntArray, align 4
  store [7 x i32] %1, ptr %testIntArray, align 4
  %2 = load [7 x i32], ptr %testIntArray, align 4
  store i32 7, ptr %0, align 4
  %3 = load i32, ptr %0, align 4
  %4 = call i32 (ptr, ...) @printf(ptr @0, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

declare i32 @printf(ptr, ...)
