; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %loopCounterOuter = alloca double, align 8
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %while.cond.l3

while.cond.l3:                                    ; preds = %if.end.l4, %entry.l1
  %0 = load double, ptr %loopCounterOuter, align 8
  %1 = fcmp olt double %0, 1.000000e+01
  %2 = alloca i1, align 1
  store i1 %1, ptr %2, align 1
  %3 = load i1, ptr %2, align 1
  br i1 %3, label %while.l3, label %while.end.l3

while.l3:                                         ; preds = %while.cond.l3
  %4 = load double, ptr %loopCounterOuter, align 8
  %5 = fcmp olt double %4, 4.000000e+00
  %6 = alloca i1, align 1
  store i1 %5, ptr %6, align 1
  %7 = load i1, ptr %6, align 1
  br i1 %7, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %while.l3
  %loopCounterInner = alloca i16, align 2
  store i16 10, ptr %loopCounterInner, align 2
  br label %while.cond.l6

while.cond.l6:                                    ; preds = %if.then.l4
  %8 = load i16, ptr %loopCounterInner, align 2
  %9 = sext i16 %8 to i32
  %10 = icmp sgt i32 %9, 0
  %11 = alloca i1, align 1
  store i1 %10, ptr %11, align 1
  %12 = load i1, ptr %11, align 1
  br i1 %12, label %while.l6, label %while.end.l6

while.l6:                                         ; preds = %while.cond.l6
  %13 = load double, ptr %loopCounterOuter, align 8
  %14 = load i16, ptr %loopCounterInner, align 2
  %15 = sext i16 %14 to i32
  %16 = call i32 (ptr, ...) @printf(ptr @0, double %13, i32 %15)
  %17 = load i16, ptr %loopCounterInner, align 2
  %18 = sub i16 %17, 1
  store i16 %18, ptr %loopCounterInner, align 2
  %19 = alloca i16, align 2
  store i16 %17, ptr %19, align 2
  br label %while.end.l3

while.end.l6:                                     ; preds = %while.cond.l6
  br label %if.end.l4

if.end.l4:                                        ; preds = %while.end.l6, %while.l3
  %20 = load double, ptr %loopCounterOuter, align 8
  %21 = fadd double %20, 1.500000e-01
  store double %21, ptr %loopCounterOuter, align 8
  br label %while.cond.l3

while.end.l3:                                     ; preds = %while.l6, %while.cond.l3
  %22 = load i32, ptr %result, align 4
  ret i32 %22
}

declare i32 @printf(ptr, ...)
