; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [12 x i8] c"F1 called.\0A\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"F2 called.\0A\00", align 1
@2 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define internal i1 @_f__void__bool__f1() {
entry.l1:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  store i1 false, ptr %0, align 1
  %2 = load i1, ptr %0, align 1
  ret i1 %2
}

declare i32 @printf(ptr, ...)

define internal i1 @_f__void__bool__f2() {
entry.l6:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @1)
  store i1 true, ptr %0, align 1
  %2 = load i1, ptr %0, align 1
  ret i1 %2
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %2 = call i1 @_f__void__bool__f1()
  store i1 %2, ptr %0, align 1
  %3 = call i1 @_f__void__bool__f2()
  store i1 %3, ptr %1, align 1
  %4 = load i1, ptr %0, align 1
  %5 = select i1 %4, ptr %0, ptr %1
  %6 = load i1, ptr %5, align 1
  %7 = zext i1 %6 to i32
  %8 = call i32 (ptr, ...) @printf(ptr @2, i32 %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}
