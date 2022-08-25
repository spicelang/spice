; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@anonymous.0 = global [2 x ptr] [ptr @0, ptr @1]

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca double, align 8
  store double 1.123000e+00, ptr %0, align 8
  %1 = load double, ptr %0, align 8
  call void @_p__void__printFormat__double(double %1)
  %2 = alloca i1, align 1
  store i1 true, ptr %2, align 1
  %3 = alloca i32, align 4
  store i32 543, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  call void @_p__void__printFormat__int(i32 %4)
  %5 = alloca i1, align 1
  store i1 true, ptr %5, align 1
  %6 = load [2 x ptr], ptr @anonymous.0, align 8
  call void @_p__void__printFormat__stringarray([2 x ptr] %6)
  %7 = alloca i1, align 1
  store i1 true, ptr %7, align 1
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare void @_p__void__printFormat__double(double)

declare void @_p__void__printFormat__int(i32)

declare void @_p__void__printFormat__stringarray([2 x ptr])
