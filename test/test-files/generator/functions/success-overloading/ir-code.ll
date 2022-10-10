; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Test func 1\0A\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1

define internal i32 @_f__void__testFunc() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  store i32 1, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  ret i32 %2
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_f__void__testFunc()
  store i32 %1, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  store i32 %2, ptr %res, align 4
  %3 = load i32, ptr %res, align 4
  %4 = call i32 (ptr, ...) @printf(ptr @1, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}
